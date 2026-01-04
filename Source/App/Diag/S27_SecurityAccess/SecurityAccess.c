/**
 * @file SecurityAccess.c
 * @author NguyenLe
 * @brief 
 * @version 0.1
 * @date 2025-12-07
 * 
 * @copyright Copyright (c) 2025
 * 
 */


#include "SecurityAccess.h"

static uint8 Diag_SecurityAccess_L1_InternalSeed_buffer[SECURITY_ACCESS_SEED_LENGTH];
static uint8 Diag_SecurityAccess_L1_InternalKey_buffer[SECURITY_ACCESS_KEY_LENGTH];

static uint8 Diag_SecurityAccess_Init_State = SECURITY_ACCESS_UNINITIALIZED;

static Diag_SecLevelType Diag_SecurrityAccess_CurrentLevel = DIAG_SEC_LOCKED;


static Std_ReturnType Diag_SecurityAccess_Algorithm (const Seed_Type* data, uint8 key);

static Std_ReturnType Diag_SecurityAccess_GetSeed (Seed_Type* Seed);

static Std_ReturnType Diag_SecurityAccess_CompareKey (Key_Type Key[]);

static Std_ReturnType Diag_SecurityAccess_ComputeKey (Seed_Type Seed[], Key_Type* Key);

static Std_ReturnType Diag_SecurityAccess_Lock(void);

void Diag_SecurityAccess_Init (void)
{
    Diag_SecurityAccess_Init_State = SECURITY_ACCESS_INITIALIZED;
    Diag_SecurrityAccess_CurrentLevel = DIAG_SEC_LOCKED;
}

static Std_ReturnType Diag_SecurityAccess_Algorithm (const Seed_Type* data, uint8 len)
{
    for (int i = 0; i < len; i++)
    {
        Diag_SecurityAccess_L1_InternalKey_buffer[i] = data[i] ^ 0xAA;
        Diag_SecurityAccess_L1_InternalKey_buffer[i] = 0x01;
    }
    return OK;
}

static Std_ReturnType Diag_SecurityAccess_GetSeed (Seed_Type* Seed)
{
    Seed_Type challengeSeed;  
    for (uint8 i = 0; i < 8; i++)
    {
        challengeSeed = 0xFF;
        Seed[i] = challengeSeed;
        Diag_SecurityAccess_L1_InternalSeed_buffer[i] = Seed[i]; // write to global buffer
    }

    return OK;
}

static Std_ReturnType Diag_SecurityAccess_CompareKey (Key_Type Key[])
{
    Std_ReturnType result = OK;
    for (uint8 i = 0; i < SECURITY_ACCESS_KEY_LENGTH; i++)
    {
        if(Key[i] != Diag_SecurityAccess_L1_InternalKey_buffer[i])
        {
            result = NOT_OK;
        }
    } 
    return result;
}

static Std_ReturnType Diag_SecurityAccess_ComputeKey (Seed_Type Seed[], Key_Type* Key)
{
    uint8 calculatedKey[SECURITY_ACCESS_KEY_LENGTH];

    (void)Diag_SecurityAccess_Algorithm(Seed, SECURITY_ACCESS_KEY_LENGTH);

    return OK;
}



Std_ReturnType Diag_SecurityAccess_Proccessor (DiagMsgType* Msg)
{
    uint8 seedLv1[SECURITY_ACCESS_SEED_LENGTH];
    uint8 keyLv1[SECURITY_ACCESS_KEY_LENGTH];
    uint8 subFuntion = Msg->reqData[1];

    if (subFuntion == 0x01)
    {
        (void)Diag_SecurityAccess_GetSeed(seedLv1);
        Msg->resData[0] = 0x67;
        Msg->resData[1] = 0x01;
        
        for (uint8 i = 0; i < SECURITY_ACCESS_SEED_LENGTH; i++)
        {
            Msg->resData[i + 2] = seedLv1[i];
        }

        Msg->resDataLen = 10;

        Diag_SecurrityAccess_CurrentLevel = DIAG_SEC_WAITING_FOR_KEY;
    }
    else if (subFuntion == 0x02)
    {
        for (uint8 i = 0; i < SECURITY_ACCESS_KEY_LENGTH; i++)
        {
            keyLv1[i] = Msg->reqData[i+2];
        }
        (void)Diag_SecurityAccess_ComputeKey(Diag_SecurityAccess_L1_InternalSeed_buffer, Diag_SecurityAccess_L1_InternalKey_buffer);
        if (Diag_SecurrityAccess_CurrentLevel == DIAG_SEC_WAITING_FOR_KEY)
        if (Diag_SecurityAccess_CompareKey(keyLv1) == OK)
        {
            Msg->resData[0] = SERVICE_27_POSITIVE_RESPONSE;
            Msg->resData[1] = 0x02;
            Msg->resDataLen = 2;
            Diag_SecurrityAccess_CurrentLevel = DIAG_SEC_UNLOCKED;
        }
        else
        if (Diag_SecurityAccess_CompareKey(keyLv1) != OK)
        {
            Msg->resData[0] = NEGATIVE_RESPONSE;
            Msg->resData[1] = SERVICE_27;
            Msg->resData[2] = NRC_35;  // NRC: InvalidKey
            Msg->resDataLen = 3;
            Diag_SecurrityAccess_CurrentLevel = DIAG_SEC_LOCKED;
        }
    }
    return OK;
}

static Std_ReturnType Diag_SecurityAccess_Lock(void)
{
    Diag_SecurrityAccess_CurrentLevel = DIAG_SEC_LOCKED;
    return OK;
}
