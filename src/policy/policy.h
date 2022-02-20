// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2019 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_POLICY_POLICY_H
#define BITCOIN_POLICY_POLICY_H

#include <consensus/consensus.h>
#include <policy/feerate.h>
#include <script/interpreter.h>
#include <script/standard.h>

#include <string>

class CCoinsViewCache;
class CTxOut;

/** Default for -blockmaxsize, which controls the maximum size of block the mining code will create **/
static const unsigned int DEFAULT_BLOCK_MAX_SIZE = 750000;
/** Default for -blockmintxfee, which sets the minimum feerate for a transaction in blocks created by mining code **/
static const unsigned int DEFAULT_BLOCK_MIN_TX_FEE = 10000;
/** The maximum size for transactions we're willing to relay/mine */
static const unsigned int MAX_STANDARD_TX_SIZE = 100000;
/** The minimum size for transactions we're willing to relay/mine */
static const unsigned int MIN_STANDARD_TX_SIZE = 82;
/** Maximum number of signature check operations in an IsStandard() P2SH script */
static const unsigned int MAX_P2SH_SIGOPS = 15;
/** The maximum number of sigops we're willing to relay/mine in a single tx */
static const unsigned int MAX_STANDARD_TX_SIGOPS = MAX_BLOCK_SIGOPS_LEGACY/5;
/** Default for -maxmempool, maximum megabytes of mempool memory usage */
static const unsigned int DEFAULT_MAX_MEMPOOL_SIZE = 300;
/** Default for -bytespersigop */
static const unsigned int DEFAULT_BYTES_PER_SIGOP = 20;
/** Default for -permitbaremultisig */
static const bool DEFAULT_PERMIT_BAREMULTISIG = true;
/** Min feerate for defining dust. Historically this has been based on the
 * minRelayTxFee, however changing the dust limit changes which transactions are
 * standard and should be done with care and ideally rarely. It makes sense to
 * only increase the dust limit after prior releases were already not creating
 * outputs below the new threshold */
static const unsigned int DUST_RELAY_TX_FEE = 10000;
/**
 * Standard script verification flags that standard transactions will comply
 * with. However scripts violating these flags may still be present in valid
 * blocks and we must accept those blocks.
 */
static constexpr unsigned int STANDARD_SCRIPT_VERIFY_FLAGS = MANDATORY_SCRIPT_VERIFY_FLAGS |
                                                             SCRIPT_VERIFY_STRICTENC |
                                                             SCRIPT_VERIFY_MINIMALDATA |
                                                             SCRIPT_VERIFY_DISCOURAGE_UPGRADABLE_NOPS |
                                                             SCRIPT_VERIFY_CLEANSTACK |
                                                             SCRIPT_VERIFY_MINIMALIF |
                                                             SCRIPT_VERIFY_NULLFAIL |
                                                             SCRIPT_VERIFY_CHECKSEQUENCEVERIFY |
                                                             SCRIPT_VERIFY_CONST_SCRIPTCODE;

/** For convenience, standard but not mandatory verify flags. */
static constexpr unsigned int STANDARD_NOT_MANDATORY_VERIFY_FLAGS = STANDARD_SCRIPT_VERIFY_FLAGS & ~MANDATORY_SCRIPT_VERIFY_FLAGS;

/** Used as the flags parameter to sequence and nLocktime checks in non-consensus code. */
static constexpr unsigned int STANDARD_LOCKTIME_VERIFY_FLAGS = LOCKTIME_VERIFY_SEQUENCE |
                                                               LOCKTIME_MEDIAN_TIME_PAST;

CAmount GetDustThreshold(const CTxOut& txout, const CFeeRate& dustRelayFee);

bool IsDust(const CTxOut& txout, const CFeeRate& dustRelayFee);

bool IsStandard(const CScript& scriptPubKey, txnouttype& whichType);
    /**
     * Check for standard transaction types
     * @return True if all outputs (scriptPubKeys) use only standard transaction forms
     */
bool IsStandardTx(const CTransaction& tx, bool permit_bare_multisig, const CFeeRate& dust_relay_fee, std::string& reason);
    /**
     * Check for standard transaction types
     * @param[in] mapInputs    Map of previous transactions that have outputs we're spending
     * @return True if all inputs (scriptSigs) use only standard transaction forms
     */
bool AreInputsStandard(const CTransaction& tx, const CCoinsViewCache& mapInputs);

/** Compute the virtual transaction size (size, or more if sigops are too dense). */
int64_t GetVirtualTransactionSize(int64_t nSize, int64_t nSigOpCount, unsigned int bytes_per_sigop);
int64_t GetVirtualTransactionSize(const CTransaction& tx, int64_t nSigOpCount, unsigned int bytes_per_sigop);
int64_t GetVirtualTransactionInputSize(const CTxIn& tx, int64_t nSigOpCount, unsigned int bytes_per_sigop);

static inline int64_t GetVirtualTransactionSize(const CTransaction& tx)
{
    return GetVirtualTransactionSize(tx, 0, 0);
}

static inline int64_t GetVirtualTransactionInputSize(const CTxIn& tx)
{
    return GetVirtualTransactionInputSize(tx, 0, 0);
}

#endif // BITCOIN_POLICY_POLICY_H
