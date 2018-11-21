#ifndef GRUUT_ENTERPRISE_MERGER_SIGNATURE_REQUESTER_HPP
#define GRUUT_ENTERPRISE_MERGER_SIGNATURE_REQUESTER_HPP

#include <vector>
#include <memory>
#include <boost/asio.hpp>

#include "../chain/block.hpp"
#include "../chain/message.hpp"
#include "../chain/merkle_tree.hpp"
#include "../application.hpp"

namespace gruut {
    class Transaction;

    const int SIGNATURE_COLLECTION_INTERVAL = 10000;
    using Transactions = std::vector<Transaction>;

    class SignatureRequester {
    public:
        SignatureRequester();

        bool requestSignatures();

    private:
        void startSignatureCollectTimer(Transactions &transactions);

        Transactions fetchTransactions();

        PartialBlock makePartialBlock(Transactions &transactions);

        Message makeMessage(PartialBlock &block);

        std::unique_ptr<boost::asio::deadline_timer> m_timer;
        MerkleTree m_merkle_tree;
    };
}
#endif
