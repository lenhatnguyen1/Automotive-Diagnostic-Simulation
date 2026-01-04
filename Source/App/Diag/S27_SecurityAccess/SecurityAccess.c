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

static Diag_SecLevelType Diag_SecurrityAccess_CurrentState = DIAG_SEC_LOCKED;

/**
 * @brief get the algorithm to compute key from seed from Crypto module
 * 
 * @param data 
 * @param key 
 * @return Std_ReturnType 
 */
static Std_ReturnType Diag_SecurityAccess_Algorithm (const Seed_Type* seed, Key_Type* key, uint8 len);

/**
 * @brief get the seed from the Crypto module
 * 
 * @param Seed 
 * @return Std_ReturnType 
 */
static Std_ReturnType Diag_SecurityAccess_GetSeed (Seed_Type* Seed);

/**
 * @brief compare the received key with the computed key
 * 
 * @param Key 
 * @return Std_ReturnType 
 */
static Std_ReturnType Diag_SecurityAccess_CompareKey (Key_Type Key[]);

/**
 * @brief compute the key based on sent seed
 * 
 * @param Seed 
 * @param Key 
 * @return Std_ReturnType 
 */
static Std_ReturnType Diag_SecurityAccess_ComputeKey (const Seed_Type* Seed, Key_Type* Key);

/**
 * @brief reset the security access state
 * 
 * @return Std_ReturnType 
 */
static Std_ReturnType Diag_SecurityAccess_Reset(void);

/* Build positive response frame */

/**
 * @brief build a positive response frame
 * 
 * @param Msg 
 * @param subFunction 
 * @param data 
 * @param dataLen 
 */
static void Diag_SecurityAccess_BuildPositiveResponse(DiagMsgType* Msg, uint8 subFunction, const uint8* data, uint8 dataLen);

/**
 * @brief build a negative response frame
 * 
 * @param Msg 
 * @param nrc 
 */
static void Diag_SecurityAccess_BuildNegativeResponse(DiagMsgType* Msg, uint8 nrc);

/**
 * @brief handle request seed
 * 
 * @param Msg 
 */
static void Diag_SecurityAccess_RequestSeedHandler(DiagMsgType* Msg);

/**
 * @brief handle send key
 * 
 * @param Msg 
 */
static void Diag_SecurityAccess_SendKeyHandler(DiagMsgType* Msg);

void Diag_SecurityAccess_Init (void)
{
    Diag_SecurityAccess_Init_State = SECURITY_ACCESS_INITIALIZED;
    Diag_SecurrityAccess_CurrentState = DIAG_SEC_LOCKED;
}

static Std_ReturnType Diag_SecurityAccess_Reset(void)
{
    Diag_SecurrityAccess_CurrentState = DIAG_SEC_LOCKED;
    return OK;
}

static Std_ReturnType Diag_SecurityAccess_Algorithm (const Seed_Type* seed, Key_Type* key, uint8 len)
{
    for (int i = 0; i < len; i++)
    {
        key[i] = seed[i] + 1;
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

static Std_ReturnType Diag_SecurityAccess_ComputeKey (const Seed_Type* Seed, Key_Type* Key)
{
    (void)Diag_SecurityAccess_Algorithm(Seed, Key, SECURITY_ACCESS_KEY_LENGTH);

    return OK;
}

Std_ReturnType Diag_SecurityAccess_Proccessor(DiagMsgType* Msg)
{
    uint8 subFunction = Msg->reqData[1];

    switch (subFunction)
    {
        case SEC_LEVEL_1:           /* Request Seed */
            Diag_SecurityAccess_RequestSeedHandler(Msg);
            break;

        case (SEC_LEVEL_1 + 1):     /* Send Key */
            Diag_SecurityAccess_SendKeyHandler(Msg);
            break;

        default:                    /* Unsupported SubFunction */
            Diag_SecurityAccess_BuildNegativeResponse(Msg, NRC_12);
            break;
    }

    return OK;
}


static void Diag_SecurityAccess_BuildPositiveResponse(DiagMsgType* Msg, uint8 subFunction,
                                     const uint8* data, uint8 dataLen)
{
    Msg->resData[0] = SERVICE_27_POSITIVE_RESPONSE; /* 0x67 */
    Msg->resData[1] = subFunction;

    if (data != NULL && dataLen > 0)
    {
        for (uint8 i = 0; i < dataLen; i++)
        {
            Msg->resData[i + 2] = data[i];
        }
    }

    Msg->resDataLen = 2 + dataLen;
}

/* Build negative response frame */
static void Diag_SecurityAccess_BuildNegativeResponse(DiagMsgType* Msg, uint8 nrc)
{
    Msg->resData[0] = NEGATIVE_RESPONSE; /* 0x7F */
    Msg->resData[1] = SERVICE_27;
    Msg->resData[2] = nrc;
    Msg->resDataLen = 3;
}

/* Handle request Seed */
static void Diag_SecurityAccess_RequestSeedHandler(DiagMsgType* Msg)
{
    uint8 seed[SECURITY_ACCESS_SEED_LENGTH];

    if (Diag_SecurrityAccess_CurrentState != DIAG_SEC_LOCKED)
    {
        Diag_SecurityAccess_BuildNegativeResponse(Msg, NRC_24); /* RequestSequenceError */
        return;
    }

    (void)Diag_SecurityAccess_GetSeed(seed);
    Diag_SecurityAccess_BuildPositiveResponse(Msg, SEC_LEVEL_1, seed, SECURITY_ACCESS_SEED_LENGTH);

    Diag_SecurrityAccess_CurrentState = DIAG_SEC_WAITING_FOR_KEY;
}

/* Handle send Key */
static void Diag_SecurityAccess_SendKeyHandler(DiagMsgType* Msg)
{
    uint8 receivedKey[SECURITY_ACCESS_KEY_LENGTH];
    Std_ReturnType compareResult;

    if (Diag_SecurrityAccess_CurrentState != DIAG_SEC_WAITING_FOR_KEY)
    {
        Diag_SecurityAccess_BuildNegativeResponse(Msg, NRC_24); /* RequestSequenceError */
        return;
    }

    for (uint8 i = 0; i < SECURITY_ACCESS_KEY_LENGTH; i++)
    {
        receivedKey[i] = Msg->reqData[i + 2];
    }

    (void)Diag_SecurityAccess_ComputeKey(
        Diag_SecurityAccess_L1_InternalSeed_buffer,
        Diag_SecurityAccess_L1_InternalKey_buffer
    );

    compareResult = Diag_SecurityAccess_CompareKey(receivedKey);

    if (compareResult == OK)
    {
        Diag_SecurityAccess_BuildPositiveResponse(Msg, SEC_LEVEL_1, NULL, 0);
        Diag_SecurrityAccess_CurrentState = DIAG_SEC_UNLOCKED;
    }
    else
    {
        Diag_SecurityAccess_BuildNegativeResponse(Msg, NRC_35); /* InvalidKey */
        Diag_SecurrityAccess_CurrentState = DIAG_SEC_LOCKED;
    }
}

