/**
 * @file Diag_Authentication.c
 * @brief Service 0x29 - Authentication (ACR, Challenge-Response)
 * @author NguyenLe
 * @version 0.1
 * @date 2026-12-13
 */

#include "S29_Authentication.h"
#include "Diag.h"
#include <string.h>

/* ============================================================
 *  Internal State Machine
 * ============================================================ */
typedef enum
{
    AUTH_STATE_IDLE = 0,
    AUTH_STATE_RCFA_PENDING,
    AUTH_STATE_VPOW_PENDING,
    AUTH_STATE_AUTHENTICATED,
    AUTH_STATE_CANCELLING
} Auth_StateType;

/* ============================================================
 *  Internal Context
 * ============================================================ */
typedef struct
{
    Auth_StateType state;
    uint8 algorithmIndicator[AUTH_MAX_ALGO_INDICATOR_SIZE];
    uint8 challenge[AUTH_MAX_CHALLENGE_SIZE];
    uint16 challengeLen;
    uint32 timeoutCounter;
} Auth_ContextType;

static Auth_ContextType Auth_Context;

/* ============================================================
 *  Internal Helpers
 * ============================================================ */

static void Auth_ResetContext(void)
{
    memset(&Auth_Context, 0, sizeof(Auth_Context));
    Auth_Context.state = AUTH_STATE_IDLE;
}

static void Auth_BuildNegativeResponse(
    DiagMsgType* Msg,
    uint8 nrc
)
{
    Msg->resData[0] = NEGATIVE_RESPONSE;
    Msg->resData[1] = SERVICE_29;
    Msg->resData[2] = nrc;
    Msg->resDataLen = 3;
}

static void Auth_BuildPositiveResponse(
    DiagMsgType* Msg,
    uint8 subFunc
)
{
    Msg->resData[0] = SERVICE_29_POSITIVE_RESPONSE;
    Msg->resData[1] = subFunc;
    Msg->resDataLen = 2;
}

/* ============================================================
 *  SubFunction Handlers
 * ============================================================ */

/**
 * requestChallengeForAuthentication (0x05)
 */
static Std_ReturnType Auth_Handle_RCFA(DiagMsgType* Msg)
{
    /* Basic length check: SID + SubFunc + commConfig + algoIndicator */
    if (Msg->reqDataLen < (2U + 1U + 1U))
    {
        Auth_BuildNegativeResponse(Msg, NRC_13); /* IncorrectMessageLength */
        return E_NOT_OK;
    }

    /* Copy algorithmIndicator (max 16 bytes) */
    uint16 algoLen = Msg->reqDataLen - 3U;
    if (algoLen > AUTH_MAX_ALGO_INDICATOR_SIZE)
    {
        Auth_BuildNegativeResponse(Msg, NRC_31); /* RequestOutOfRange */
        return E_NOT_OK;
    }

    memcpy(Auth_Context.algorithmIndicator,
           &Msg->reqData[3],
           algoLen);

    /* --- STUB: generate challenge --- */
    Auth_Context.challengeLen = 8U;
    for (uint8 i = 0; i < Auth_Context.challengeLen; i++)
    {
        Auth_Context.challenge[i] = (uint8)(0xA5U + i);
    }

    Auth_Context.state = AUTH_STATE_RCFA_PENDING;

    /* Build positive response:
     * SID + SubFunc + Challenge
     */
    Msg->resData[0] = SERVICE_29_POSITIVE_RESPONSE;
    Msg->resData[1] = AUTH_SUBFUNC_RCFA;
    memcpy(&Msg->resData[2],
           Auth_Context.challenge,
           Auth_Context.challengeLen);

    Msg->resDataLen = 2U + Auth_Context.challengeLen;

    return E_OK;
}

/**
 * verifyProofOfOwnershipUnidirectional (0x06)
 */
static Std_ReturnType Auth_Handle_VPOW(DiagMsgType* Msg)
{
    if (Auth_Context.state != AUTH_STATE_RCFA_PENDING)
    {
        Auth_BuildNegativeResponse(Msg, NRC_24); /* ConditionsNotCorrect */
        return E_NOT_OK;
    }

    /* Minimal length check */
    if (Msg->reqDataLen < 4U)
    {
        Auth_BuildNegativeResponse(Msg, NRC_13);
        return E_NOT_OK;
    }

    /* --- STUB: verify proof of ownership ---
     * TODO: call Crypto_VerifySignature(...)
     */
    uint8 proofValid = 1U;

    if (proofValid == 0U)
    {
        Auth_ResetContext();
        Auth_BuildNegativeResponse(Msg, NRC_35); /* InvalidKey */
        return E_NOT_OK;
    }

    Auth_Context.state = AUTH_STATE_AUTHENTICATED;

    Auth_BuildPositiveResponse(Msg, AUTH_SUBFUNC_VPOW);

    return E_OK;
}

/* ============================================================
 *  Public API
 * ============================================================ */

void Diag_Authentication_Init(void)
{
    Auth_ResetContext();
}

void Diag_Authentication_DeInit(void)
{
    Auth_ResetContext();
}

void Diag_Authentication_MainFunction(void)
{
    /* Placeholder for:
     * - timeout handling
     * - async crypto completion
     * - cancel handling
     */
    switch (Auth_Context.state)
    {
        case AUTH_STATE_IDLE:
        case AUTH_STATE_AUTHENTICATED:
        default:
            break;
    }
}

/**
 * @brief Entry point for Service 0x29
 */
Std_ReturnType Diag_Authentication_Entry(DiagMsgType* Msg)
{
    uint8 subFunc;

    if (Msg == NULL)
    {
        return E_NOT_OK;
    }

    /* SID already checked by router */
    if (Msg->reqDataLen < 2U)
    {
        Auth_BuildNegativeResponse(Msg, NRC_13);
        return E_NOT_OK;
    }

    subFunc = Msg->reqData[1];

    switch (subFunc)
    {
        case AUTH_SUBFUNC_RCFA:
            return Auth_Handle_RCFA(Msg);

        case AUTH_SUBFUNC_VPOW:
            return Auth_Handle_VPOW(Msg);

        default:
            Auth_BuildNegativeResponse(Msg, NRC_12); /* SubFunctionNotSupported */
            return E_NOT_OK;
    }
}
