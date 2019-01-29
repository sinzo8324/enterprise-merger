#include "transaction_collector.hpp"
#include "../application.hpp"
#include "easy_logging.hpp"

using namespace std;

namespace gruut {
TransactionCollector::TransactionCollector() {

  el::Loggers::getLogger("TXCO");

  m_cert_pool = CertificatePool::getInstance();
  m_storage = Storage::getInstance();

  auto &io_service = Application::app().getIoService();

  m_update_status_scheduler.setIoService(io_service);
  m_update_status_scheduler.setTaskFunction([this]() { checkBpJob(); });
  m_update_status_scheduler.setTime(0, config::BP_INTERVAL * 1000);
  m_update_status_scheduler.setStrandMod();
}

void TransactionCollector::handleMessage(json &msg_body_json) {
  if (!isRunnable()) {
    // CLOG(ERROR, "TXCO") << "TX dropped (not timing)";
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

  std::string pk_cert = m_cert_pool->getCert(requester_id);

  if (pk_cert.empty()) {
    CLOG(ERROR, "TXCO") << "TX dropped (unknown requester)";
    return;
  }

  if (Application::app().getCustomLedgerManager().isValidTransaction(new_tx)) {
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
  turnOnTimer();

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

  std::call_once(m_timer_once_flag, [this]() {
    m_bpjob_sequence.push_back(BpJobStatus::DONT);
    m_bpjob_sequence.push_back(BpJobStatus::UNKNOWN);
    m_bpjob_sequence.push_back(BpJobStatus::UNKNOWN);

    m_update_status_scheduler.runTaskOnTime();
  });
}

void TransactionCollector::checkBpJob() {
  m_current_tx_status = m_next_tx_status;

  BpJobStatus this_job = m_bpjob_sequence.front();
  m_bpjob_sequence.pop_front();
  m_bpjob_sequence.push_back(BpJobStatus::UNKNOWN);
  if (this_job == BpJobStatus::DO &&
      Application::app().getTransactionPool().size() > 0) {
    m_signature_requester.requestSignatures();
  }
}

} // namespace gruut