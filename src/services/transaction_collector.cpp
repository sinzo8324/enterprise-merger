#include "transaction_collector.hpp"
#include "../application.hpp"
#include "easy_logging.hpp"

using namespace std;

namespace gruut {
TransactionCollector::TransactionCollector() {

  el::Loggers::getLogger("TXCO");

  m_storage = Storage::getInstance();
  auto setting = Setting::getInstance();

  std::vector<ServiceEndpointInfo> service_endpoints =
      setting->getServiceEndpointInfo();
  for (auto &srv_point : service_endpoints) {
    m_cert_map.insert({srv_point.id, srv_point.cert});
  }

  std::vector<MergerInfo> mergers = setting->getMergerInfo();
  for (auto &merger : mergers) {
    m_cert_map.insert({merger.id, merger.cert});
  }

  m_timer.reset(
      new boost::asio::deadline_timer(Application::app().getIoService()));
}

void TransactionCollector::handleMessage(json &msg_body_json) {
  if (!isRunnable()) {
    CLOG(ERROR, "TXCO") << "TX dropped (not timing)";
    return;
  }

  std::string txid_b64 = Safe::getString(msg_body_json, "txid");
  auto new_txid =
      TypeConverter::base64ToArray<TRANSACTION_ID_TYPE_SIZE>(txid_b64);

  auto &transaction_pool = Application::app().getTransactionPool();

  if (transaction_pool.isDuplicated(new_txid)) {
    CLOG(ERROR, "TXCO") << "TX dropped (duplicated in pool)";
    return;
  }

  if (m_storage->isDuplicatedTx(txid_b64)) {
    CLOG(ERROR, "TXCO") << "TX dropped (duplicated in storage)";
    return;
  }

  Transaction new_tx;
  new_tx.setJson(msg_body_json);

  id_type requester_id = Safe::getBytesFromB64(msg_body_json, "rID");

  auto it_result = m_cert_map.find(requester_id);
  if (it_result == m_cert_map.end()) {
    CLOG(ERROR, "TXCO") << "TX dropped (unknown requester)";
    return;
  }

  if (new_tx.isValid(it_result->second)) {
    transaction_pool.push(new_tx);
  } else {
    CLOG(ERROR, "TXCO") << "TX dropped (invalid)";
  }
}

bool TransactionCollector::isRunnable() {
  return (m_current_tx_status == BpStatus::PRIMARY ||
          m_current_tx_status == BpStatus::SECONDARY);
}

void TransactionCollector::setTxCollectStatus(BpStatus stat) {
  m_next_tx_status = stat;
  if (!m_timer_running) {
    turnOnTimer();
  }

  if (m_current_tx_status == BpStatus::PRIMARY) {
    if (m_next_tx_status == BpStatus::PRIMARY) {
      m_bpjob_sequence[1] = BpJobStatus::DO;
    } else if (m_next_tx_status == BpStatus::SECONDARY) {
      m_bpjob_sequence[1] = BpJobStatus::DONT;
      m_bpjob_sequence[2] = BpJobStatus::DO;
    } else {
      m_bpjob_sequence[1] = BpJobStatus::DONT;
    }
  } else if (m_current_tx_status == BpStatus::SECONDARY) {
    if (m_next_tx_status == BpStatus::PRIMARY) {
      m_bpjob_sequence[1] = BpJobStatus::DO;
    }
  } else {
    if (m_next_tx_status ==
        BpStatus::PRIMARY) { // the case when only 1 merger exists in network
      m_bpjob_sequence[1] = BpJobStatus::DO;
    } else if (m_next_tx_status == BpStatus::SECONDARY) {
      m_bpjob_sequence[1] = BpJobStatus::DONT;
      m_bpjob_sequence[2] = BpJobStatus::DO;
    }
  }
}

void TransactionCollector::turnOnTimer() {

  m_timer_running = true;

  m_bpjob_sequence.push_back(BpJobStatus::DONT);
  m_bpjob_sequence.push_back(BpJobStatus::UNKNOWN);
  m_bpjob_sequence.push_back(BpJobStatus::UNKNOWN);

  updateStatus();
}

void TransactionCollector::updateStatus() {

  // CLOG(INFO, "TXCO") << "called updateStatus()";

  size_t current_slot = Time::now_int() / BP_INTERVAL;
  time_t next_slot_begin = (current_slot + 1) * BP_INTERVAL;

  boost::posix_time::ptime task_time =
      boost::posix_time::from_time_t(next_slot_begin);

  m_timer->expires_at(task_time);
  m_timer->async_wait([this](const boost::system::error_code &ec) {
    if (ec == boost::asio::error::operation_aborted) {
    } else if (ec.value() == 0) {

      postJob();
      updateStatus();

    } else {
      throw;
    }
  });
}

void TransactionCollector::postJob() {
  auto &io_service = Application::app().getIoService();

  io_service.post([this]() {
    m_current_tx_status = m_next_tx_status;

    BpJobStatus this_job = m_bpjob_sequence.front();
    m_bpjob_sequence.pop_front();
    m_bpjob_sequence.push_back(BpJobStatus::UNKNOWN);
    if (this_job == BpJobStatus::DO &&
        Application::app().getTransactionPool().size() > 0) {
      m_signature_requester.requestSignatures();
    }
  });
}

} // namespace gruut