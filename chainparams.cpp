// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2018 The Bitcoin Core developers
// Copyright (c) 2014-2017 The Dash Core developers
// Copyright (c) 2018 PM-Tech
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <chainparams.h>
#include <consensus/merkle.h>

#include <tinyformat.h>
#include <util.h>
#include <utilstrencodings.h>

#include <assert.h>

#include <chainparamsseeds.h>

static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    CBlock genesis;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(MakeTransactionRef(std::move(txNew)));
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}

/**
 * Build the genesis block. Note that the output of its generation
 * transaction cannot be spent since it did not originally exist in the
 * database.
 *
 * CBlock(hash=00000ffd590b14, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=e0028e, nTime=1390095618, nBits=1e0ffff0, nNonce=28917698, vtx=1)
 *   CTransaction(hash=e0028e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
 *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d01044c5957697265642030392f4a616e2f3230313420546865204772616e64204578706572696d656e7420476f6573204c6976653a204f76657273746f636b2e636f6d204973204e6f7720416363657074696e6720426974636f696e73)
 *     CTxOut(nValue=50.00000000, scriptPubKey=0xA9037BAC7050C479B121CF)
 *   vMerkleTree: e0028e
 */
static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "05-10-2018 - Europe rolled out its general data protection regulation laws";  // PM-Tech: ChainCoin
    const CScript genesisOutputScript = CScript() << ParseHex("04678afdb0fe5548271967f1a67130b7105cd6a828e03909a67962e0ea1f61deb649f6bc3f4cef38c4f35504e51ec112de5c384df7ba0b8d578a4c702b6bf11d5f") << OP_CHECKSIG; // PM-Tech: ChainCoin
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}

void CChainParams::UpdateVersionBitsParameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    consensus.vDeployments[d].nStartTime = nStartTime;
    consensus.vDeployments[d].nTimeout = nTimeout;
}

/**
 * Main network
 */
/**
 * What makes a good checkpoint block?
 * + Is surrounded by blocks with reasonable timestamps
 *   (no blocks before with a timestamp after, none after with
 *    timestamp before)
 * + Contains no strange transactions
 */


class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = "main";
        consensus.nSubsidyHalvingInterval = 700800; // PM-Tech: ChainCoin // 2 years
        consensus.nMasternodePaymentsStartBlock = 1572480; // PM-Tech: SegWit activation block
        consensus.nMasternodePaymentsIncreaseBlock = 2250000; // PM-Tech: estimation
        consensus.nMasternodePaymentsIncreasePeriod = 175000; // PM-Tech: approx. 6 months
        consensus.nSuperblockStartBlock = 1550000; // PM-Tech: to be defined
        consensus.nSuperblockCycle = 28800; // PM-Tech: to be defined
        consensus.nGovernanceMinQuorum = 10;
        consensus.nGovernanceFilterElements = 20000;
        consensus.nMasternodeMinimumConfirmations = 15;
        consensus.nMajorityEnforceBlockUpgrade = 750;
        consensus.nMajorityRejectBlockOutdated = 950;
        consensus.nMajorityWindow = 1000;
        consensus.BIP16Height = 1;
        consensus.BIP34Height = 1;
        consensus.BIP34Hash = uint256S("0x00000012f1c40ff12a9e6b0e9076fe4fa7ad27012e256a5ad7bcb80dc02c0409"); // PM-Tech: ChainCoin
        consensus.BIP65Height = 1551170;
        consensus.BIP66Height = 1000000;
        consensus.powLimit = uint256S("0"); // ~uint256(0) >> 20
        consensus.nPowTargetTimespan = 90; // PM-Tech: ChainCoin re-target at every block
        consensus.nPowTargetSpacing = 90; // PM-Tech: ChainCoin 90 seconds
        consensus.nInterval = 1; // PM-Tech: ChainCoin
        consensus.nAveragingInterval = 8; // PM-Tech: ChainCoin
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 10752; // 80% of 13440
        consensus.nMinerConfirmationWindow = 13440; // average of 2 weeks
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1527811200; // June 1st, 2018
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1577750400; // Dec 31st, 2019

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 1527811200; // June 1st, 2018.
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 1577750400; // Dec 31st, 2019.

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00"); // 1499485 // PM-Tech: ChainCoin

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00"); //1499485 // PM-Tech: ChainCoin

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = 0xb3; // PM-Tech: ChainCoin = random numbers, change this
        pchMessageStart[1] = 0xa2; // PM-Tech: ChainCoin
        pchMessageStart[2] = 0x2a; // PM-Tech: ChainCoin
        pchMessageStart[3] = 0xc3; // PM-Tech: ChainCoin
        nDefaultPort = 55675; // PM-Tech: ChainCoin
        nPruneAfterHeight = 100000;

        genesis = CreateGenesisBlock(1538680394, 2477623971, 0x1e0fffff, 1, 16 * COIN); // PM-Tech: ChainCoin
        consensus.hashGenesisBlock = genesis.GetHash();
        printf("genesis.GetHash = %s\n", genesis.GetHash().ToString().c_str());
        printf("MerkleRoot 0x%s\n", genesis.hashMerkleRoot.ToString().c_str());
        printf("nNounce %d\n",genesis.nNonce);
        printf("nBits %d\n", genesis.nBits);
        printf("nTime %d\n", genesis.nTime);
        assert(consensus.hashGenesisBlock == uint256S("0x0c111bb685983f1d9bd7acb9516f4fd8b90cc444389268a0d52a439044859ca9")); // PM-Tech: ChainCoin = Genesis block 0 
        assert(genesis.hashMerkleRoot == uint256S("0xb6d933a77f014c4f4df3e996e20ec03b912d902bfdba989c09294e9a1577ea53")); // PM-Tech: ChainCoin = Genesis block 0 hash

        // Note that of those which support the service bits prefix, most only support a subset of
        // possible options.
        // This is fine at runtime as we'll fall back to using them as a oneshot if they dont support the
        // service bits we want, but we should get them updated to support all service bits wanted by any
        // release ASAP to avoid it where possible.
        vSeeds.emplace_back("80.211.177.201");
       // vSeeds.emplace_back("ITZ2.hashunlimited.com");
       // vSeeds.emplace_back("seed1.interzone.pw");
       // vSeeds.emplace_back("seed2.interzone.pw");
       // vSeeds.emplace_back("seed3.interzone.pw");
       // vSeeds.emplace_back("seed4.interzone.pw");

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,28); // PM-Tech: ChainCoin was 28
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,4); // PM-Tech: ChainCoin
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,28 + 128); // PM-Tech: ChainCoin
        // Chaincoin BIP32 pubkeys start with 'xpub' (Bitcoin defaults)
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x88, 0xB2, 0x1E};
        // Chaincoin BIP32 prvkeys start with 'xprv' (Bitcoin defaults)
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x88, 0xAD, 0xE4};

        bech32_hrp = "ITZ";

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = true;
        fAllowMultiplePorts = false;

        nPoolMaxTransactions = 3;
        nFulfilledRequestExpireTime = 60*60; // fulfilled requests expire in 1 hour

        checkpointData = {  // PM-Tech: ChainCoin
            {
            //    {      0, uint256S("0x0c111bb685983f1d9bd7acb9516f4fd8b90cc444389268a0d52a439044859ca9")},
            //    {   6143, uint256S("0x0000000026fb51f5bc9943ed69d9ff7697ecf7fed419d88b417655f93a487ce1")},
            //    {  12797, uint256S("0x000000002c29644e179baa188fa6b9b9454721f1f21f2b9f31eebe9acc1a31db")},
            //    {  30092, uint256S("0x0000000098a23e1c503f71a6d61c333c5abaabb4c5fa1b474012e004db4bfbbe")},
            //    {  80998, uint256S("0x000000010ebcfe9a00a99f2b61104f4a141555a707f1c007aba8a978f6030cfb")},
            //    { 144759, uint256S("0x000000047e7b7bfd63b4f019a0a24c8d65b10afa6eb80721e10fa7c49ce6fb6e")},
            //    { 189046, uint256S("0x00000000bd507c435b46ee8a13b25b85ec38fdb0eb5b00faeaa0611cd6a483d3")},
            //    { 277316, uint256S("0x00000016a20503fe496e79d34fb85c33f633059315c046ffa1b4826d08a1e856")},
            //    { 483849, uint256S("0x000001eb7f8124282ab62296e63d3145ff6c84cf18afae4d4b8e02cd3182b6a8")},
            //    {1066428, uint256S("0x000000012dc5256d977b50270d1ca5642726308dcf26b6c219985edb8f2ab8f6")},
            //    {1300730, uint256S("0x0000000001fdf11c0b4238b448c9a9643c7862575124fe0d7ee6fe7b5e7dba30")},
             //   {1384154, uint256S("0x0000000000fb3c41fb8a955b3c9fca128e57e51834347ea368adbea309fcd265")},
           }
       };

       chainTxData = ChainTxData{
            1538680394  /*1518000378*/, // * UNIX timestamp of last checkpoint block
            0,    // * total number of transactions between genesis and last checkpoint
                        //   (the tx=... number in the SetBestChain debug.log lines)
            0.0142373   // * estimated number of transactions per second after that timestamp
        };
    }
};

/**
 * Testnet (v4)
 */
class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
        strNetworkID = "test";
        consensus.nSubsidyHalvingInterval = 56600;
        consensus.nMasternodePaymentsStartBlock = 48384; // PM-Tech: SegWit activation block
        consensus.nMasternodePaymentsIncreaseBlock = 4030;
        consensus.nMasternodePaymentsIncreasePeriod = 10;
        consensus.nSuperblockStartBlock = 4200; // NOTE: Should satisfy nSuperblockStartBlock > nBudgetPeymentsStartBlock
        consensus.nSuperblockCycle = 24; // Superblocks can be issued hourly on testnet
        consensus.nGovernanceMinQuorum = 1;
        consensus.nGovernanceFilterElements = 500;
        consensus.nMasternodeMinimumConfirmations = 1;
        consensus.nMajorityEnforceBlockUpgrade = 51;
        consensus.nMajorityRejectBlockOutdated = 75;
        consensus.nMajorityWindow = 100;
        consensus.BIP16Height = 1;
        consensus.BIP34Height = 1;
        consensus.BIP34Hash = uint256S("0x00000352de593a01e0efcbaec00345ec80d20c7bd2024ec7c2beec048af0e6d9");
        consensus.BIP65Height = 48384;
        consensus.BIP66Height = 1;
        consensus.powLimit = uint256S("0"); // ~uint256(0) >> 20
        consensus.nPowTargetTimespan = 90; // PM-Tech: ChainCoin
        consensus.nPowTargetSpacing = 90; // PM-Tech: ChainCoin
        consensus.nInterval = 1; // PM-Tech: ChainCoin
        consensus.nAveragingInterval = 8; // PM-Tech: ChainCoin
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1512; // 75% for testchains
        consensus.nMinerConfirmationWindow = 2016; // nPowTargetTimespan / nPowTargetSpacing
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1517443200; // Feb 1st, 2018
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1548979200; // Feb 1st, 2019

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 1517443200; // Feb 1st, 2018.
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 1548979200; // Feb 1st, 2019.

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("000000000000000000000000000000000000000000000000000000060e06d35d"); //2947

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("000000d9bd4820c3f64f31fb69d520baa5698d2700b5addfa4f27b264f2bc298"); //2947

        pchMessageStart[0] = 0xab;
        pchMessageStart[1] = 0xb2;
        pchMessageStart[2] = 0x13;
        pchMessageStart[3] = 0xe2;
        nDefaultPort = 21994;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1538680394, 423087994, 0x1e0fffff, 1, 16 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        printf("genesis.GetHash = %s\n", genesis.GetHash().ToString().c_str());
        printf("MerkleRoot 0x%s\n", genesis.hashMerkleRoot.ToString().c_str());
        printf("nNounce %d\n",genesis.nNonce);
        printf("nBits %d\n", genesis.nBits);
        printf("nTime %d\n", genesis.nTime);
        assert(consensus.hashGenesisBlock == uint256S("0xabe6512220e01e9c7831f89cfd352e23b975db38336b4e7e1123ba13a5858540"));
        assert(genesis.hashMerkleRoot == uint256S("0xb6d933a77f014c4f4df3e996e20ec03b912d902bfdba989c09294e9a1577ea53"));

        vFixedSeeds.clear();
        vSeeds.clear();

        // nodes with support for servicebits filtering should be at the top
        vSeeds.emplace_back("testseed.hashunlimited.com");

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,80);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,44);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,88+128);
        // Testnet Chaincoin BIP32 pubkeys start with 'tpub' (Bitcoin defaults)
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        // Testnet Chaincoin BIP32 prvkeys start with 'tprv' (Bitcoin defaults)
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};

        bech32_hrp = "tITZ";

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;
        fAllowMultiplePorts = true;

        nPoolMaxTransactions = 3;
        nFulfilledRequestExpireTime = 5*60; // fulfilled requests expire in 5 minutes

        checkpointData = {
            {
              //  {      0, uint256S("0000082f5939c2154dbcba35f784530d12e9d72472fcfaf29674ea312cdf4c83")},
              //  {  66666, uint256S("0000001566591fd5eec0de52a7de96567edeb3ba2d78b41123d6a6b7dafab603")},
            }
        };

        chainTxData = ChainTxData{

            1526408675, // * UNIX timestamp of last checkpoint block
            67387,      // * total number of transactions between genesis and last checkpoint
                        //   (the tx=... number in the SetBestChain debug.log lines)
            0.01        // * estimated number of transactions per day after checkpoint
        };

    }
};

/**
 * Regression test taken straigt from Chaincoin except port and timing
 */
class CRegTestParams : public CChainParams {
public:
    CRegTestParams() {
        strNetworkID = "regtest";
        consensus.nSubsidyHalvingInterval = 150;
        consensus.nMasternodePaymentsStartBlock = 240;
        consensus.nMasternodePaymentsIncreaseBlock = 350;
        consensus.nMasternodePaymentsIncreasePeriod = 10;
        consensus.nSuperblockStartBlock = 1500;
        consensus.nSuperblockCycle = 10;
        consensus.nGovernanceMinQuorum = 1;
        consensus.nGovernanceFilterElements = 100;
        consensus.nMasternodeMinimumConfirmations = 1;
        consensus.nMajorityEnforceBlockUpgrade = 750;
        consensus.nMajorityRejectBlockOutdated = 950;
        consensus.nMajorityWindow = 1000;
        consensus.BIP16Height = 0; // always enforce P2SH BIP16 on regtest
        consensus.BIP34Height = -1; // BIP34 has not necessarily activated on regtest
        consensus.BIP34Hash = uint256();
        consensus.BIP65Height = 1351; // BIP65 activated on regtest (Used in rpc activation tests)
        consensus.BIP66Height = 1251; // BIP66 activated on regtest (Used in rpc activation tests)
        consensus.powLimit = uint256S("0");
        consensus.nPowTargetTimespan = 90; // PM-Tech: ChainCoin 90s
        consensus.nPowTargetSpacing = 90; // PM-Tech: ChainCoin 90s
        consensus.nInterval = 1; // PM-Tech: ChainCoin
        consensus.nAveragingInterval = 8; // PM-Tech: ChainCoin
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;
        consensus.nRuleChangeActivationThreshold = 108; // 75% for testchains
        consensus.nMinerConfirmationWindow = 144; // Faster than normal for regtest (144 instead of 2016)
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 28;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = Consensus::BIP9Deployment::ALWAYS_ACTIVE;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00");

        pchMessageStart[0] = 0xac;
        pchMessageStart[1] = 0xef;
        pchMessageStart[2] = 0xc7;
        pchMessageStart[3] = 0xb6;
        nDefaultPort = 18444;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1296688602, 3, 0x207fffff, 1, 16 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();

        vFixedSeeds.clear(); //! Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();  //! Regtest mode doesn't have any DNS seeds.

        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;
        fAllowMultiplePorts = true;

        nPoolMaxTransactions = 3;
        nFulfilledRequestExpireTime = 5*60; // fulfilled requests expire in 5 minutes

        checkpointData = {
        {
            { 0, uint256S("000008ca1832a4baf228eb1553c03d3a2c8e02399550dd6ea8d65cec3ef23d2e")},
    }
        };

        chainTxData = ChainTxData{
            0,
            0,
            0
        };

        //Regtest: Bitcoin Parameters
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,111);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};

        bech32_hrp = "ITZrt";
    }
};

static std::unique_ptr<CChainParams> globalChainParams;

const CChainParams &Params() {
    assert(globalChainParams);
    return *globalChainParams;
}

std::unique_ptr<CChainParams> CreateChainParams(const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN)
        return std::unique_ptr<CChainParams>(new CMainParams());
    else if (chain == CBaseChainParams::TESTNET)
        return std::unique_ptr<CChainParams>(new CTestNetParams());
    else if (chain == CBaseChainParams::REGTEST)
        return std::unique_ptr<CChainParams>(new CRegTestParams());
    throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network)
{
    SelectBaseParams(network);
    globalChainParams = CreateChainParams(network);
}

void UpdateVersionBitsParameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    globalChainParams->UpdateVersionBitsParameters(d, nStartTime, nTimeout);
}
