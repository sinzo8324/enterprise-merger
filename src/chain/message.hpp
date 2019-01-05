#ifndef GRUUT_ENTERPRISE_MERGER_MESSAGE_HPP
#define GRUUT_ENTERPRISE_MERGER_MESSAGE_HPP

#include "nlohmann/json.hpp"
#include "types.hpp"
#include <tuple>

using namespace std;

namespace gruut {
struct MessageHeader {
  uint8_t identifier;
  message_version_type version;
  MessageType message_type;
  MACAlgorithmType mac_algo_type;
  CompressionAlgorithmType compression_algo_type;
  uint8_t dummy;
  uint8_t total_length[4];
  local_chain_id_type local_chain_id;
  id_type sender_id;
  uint8_t reserved_space[6];
};

struct Message : public MessageHeader {
  Message() = delete;

  Message(MessageHeader &header) : MessageHeader(header) {}

  json data;
};
} // namespace gruut
#endif
