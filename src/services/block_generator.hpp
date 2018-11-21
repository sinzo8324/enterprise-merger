#ifndef GRUUT_ENTERPRISE_MERGER_BLOCK_GENERATOR_HPP
#define GRUUT_ENTERPRISE_MERGER_BLOCK_GENERATOR_HPP

#include <vector>
#include "../chain/block.hpp"
#include "../chain/transaction.hpp"
#include "../chain/types.hpp"

#include "signature_requester.hpp"

using namespace std;

namespace gruut {
    using Transactions = vector<Transaction>;
    class BlockGenerator {
    public:
        PartialBlock generatePartialBlock(sha256 transaction_root_id);
    };
}

#endif
