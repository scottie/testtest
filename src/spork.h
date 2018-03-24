// Copyright (c) 2014-2016 The Dash developers
// Copyright (c) 2016-2017 The PIVX developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef SPORK_H
#define SPORK_H

#include "base58.h"
#include "key.h"
#include "main.h"
#include "net.h"
#include "sync.h"
#include "util.h"

#include "obfuscation.h"
#include "protocol.h"
#include <boost/lexical_cast.hpp>

using namespace std;
using namespace boost;

/*
    Don't ever reuse these IDs for other sporks
    - This would result in old clients getting confused about which spork is for what

    Sporks 11,12, and 16 to be removed with 1st zerocoin release
*/
#define SPORK_START 1
#define SPORK_END 15

#define SPORK_2_SWIFTTX 1
#define SPORK_3_SWIFTTX_BLOCK_FILTERING 2
#define SPORK_5_MAX_VALUE 4
#define SPORK_7_MASTERNODE_SCANNING 6
#define SPORK_8_MASTERNODE_PAYMENT_ENFORCEMENT 7
#define SPORK_9_MASTERNODE_BUDGET_ENFORCEMENT 8
#define SPORK_10_MASTERNODE_PAY_UPDATED_NODES 9
#define SPORK_11_LOCK_INVALID_UTXO 10
//#define SPORK_12_RECONSIDER_BLOCKS 10011
#define SPORK_13_ENABLE_SUPERBLOCKS 12
#define SPORK_14_NEW_PROTOCOL_ENFORCEMENT 13
#define SPORK_15_NEW_PROTOCOL_ENFORCEMENT_2 14
#define SPORK_16_ZEROCOIN_MAINTENANCE_MODE 15

#define SPORK_2_SWIFTTX_DEFAULT 4070908800                         //2001-1-1
#define SPORK_3_SWIFTTX_BLOCK_FILTERING_DEFAULT 4070908800        //2015-2-18
#define SPORK_5_MAX_VALUE_DEFAULT 1000                            //1000 ZIX
#define SPORK_7_MASTERNODE_SCANNING_DEFAULT 4070908800             //2001-1-1
#define SPORK_8_MASTERNODE_PAYMENT_ENFORCEMENT_DEFAULT 4070908800 //OFF
#define SPORK_9_MASTERNODE_BUDGET_ENFORCEMENT_DEFAULT 4070908800  //OFF
#define SPORK_10_MASTERNODE_PAY_UPDATED_NODES_DEFAULT 4070908800  //OFF
#define SPORK_11_LOCK_INVALID_UTXO_DEFAULT 4070908800             //OFF - NOTE: this is block height not time!
#define SPORK_13_ENABLE_SUPERBLOCKS_DEFAULT 4070908800            //OFF
#define SPORK_14_NEW_PROTOCOL_ENFORCEMENT_DEFAULT 4070908800      //OFF
#define SPORK_15_NEW_PROTOCOL_ENFORCEMENT_2_DEFAULT 4070908800    //OFF
#define SPORK_16_ZEROCOIN_MAINTENANCE_MODE_DEFAULT 4070908800     //OFF

class CSporkMessage;
class CSporkManager;

extern std::map<uint256, CSporkMessage> mapSporks;
extern std::map<int, CSporkMessage> mapSporksActive;
extern CSporkManager sporkManager;

void LoadSporksFromDB();
void ProcessSpork(CNode* pfrom, std::string& strCommand, CDataStream& vRecv);
int64_t GetSporkValue(int nSporkID);
bool IsSporkActive(int nSporkID);
void ReprocessBlocks(int nBlocks);

//
// Spork Class
// Keeps track of all of the network spork settings
//

class CSporkMessage
{
public:
    std::vector<unsigned char> vchSig;
    int nSporkID;
    int64_t nValue;
    int64_t nTimeSigned;

    uint256 GetHash()
    {
        uint256 n = XEVAN(BEGIN(nSporkID), END(nTimeSigned));
        return n;
    }

    ADD_SERIALIZE_METHODS;

    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream& s, Operation ser_action, int nType, int nVersion)
    {
        READWRITE(nSporkID);
        READWRITE(nValue);
        READWRITE(nTimeSigned);
        READWRITE(vchSig);
    }
};


class CSporkManager
{
private:
    std::vector<unsigned char> vchSig;
    std::string strMasterPrivKey;

public:
    CSporkManager()
    {
    }

    std::string GetSporkNameByID(int id);
    int GetSporkIDByName(std::string strName);
    bool UpdateSpork(int nSporkID, int64_t nValue);
    bool SetPrivKey(std::string strPrivKey);
    bool CheckSignature(CSporkMessage& spork);
    bool Sign(CSporkMessage& spork);
    void Relay(CSporkMessage& msg);
};

#endif
