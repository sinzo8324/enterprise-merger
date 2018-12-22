#include "transaction_pool.hpp"

namespace gruut {
void TransactionPool::push(Transaction &transaction) {
  std::lock_guard<std::mutex> guard(m_mutex);
  m_transaction_pool.emplace_back(transaction);
}

Transaction TransactionPool::pop() {
  std::lock_guard<std::mutex> guard(m_mutex);
  auto transaction = m_transaction_pool.front();
  m_transaction_pool.pop_front();
  m_mutex.unlock();

  return transaction;
}

void TransactionPool::removeDuplicatedTransactions(
    std::vector<transaction_id_type> &tx_ids) {
  if (tx_ids.empty())
    return;

  std::lock_guard<std::mutex> guard(m_mutex);
  for (auto &tx_id : tx_ids) {
    m_transaction_pool.remove_if(
        [&](Transaction &tx) { return (tx.transaction_id == tx_id); });
  }
  m_mutex.unlock();
}

size_t TransactionPool::size() { return m_transaction_pool.size(); }
} // namespace gruut
