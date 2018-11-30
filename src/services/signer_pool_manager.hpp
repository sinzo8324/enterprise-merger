#ifndef GRUUT_ENTERPRISE_MERGER_SIGNER_MANAGER_HPP
#define GRUUT_ENTERPRISE_MERGER_SIGNER_MANAGER_HPP

#include <memory>
#include <nlohmann/json.hpp>
#include <set>
#include <unordered_map>
#include <vector>

#include "../chain/join_temporary_data.hpp"
#include "../chain/signer.hpp"
#include "../chain/types.hpp"
#include "signer_pool.hpp"

using namespace std;

namespace gruut {
using RandomSignerIndices = std::set<int>;

class SignerPoolManager {
public:
  SignerPoolManager() = default;
  // TODO: SignerPool 구조가 변경됨에 따라 나중에 수정
  //  SignerPool getSelectedSignerPool();
  void handleMessage(MessageType &message_type, signer_id_type receiver_id,
                     nlohmann::json message_body_json);

private:
  RandomSignerIndices generateRandomNumbers(unsigned int size);
  bool verifySignature(signer_id_type signer_id,
                       nlohmann::json message_body_json);
  string getCertificate();
  string signMessage(vector<uint8_t>, vector<uint8_t>, vector<uint8_t>,
                     vector<uint8_t>, vector<uint8_t>);
  bool isJoinable();

  // A temporary table for connection establishment.
  unordered_map<signer_id_type, unique_ptr<JoinTemporaryData>>
      join_temporary_table;
};
} // namespace gruut
#endif
