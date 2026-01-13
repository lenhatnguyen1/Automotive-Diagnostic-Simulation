#include <stdio.h>

#include "test.h"
#include "S27_SecurityAccess.h"

/* ================= Utilities ================= */

void Test_PrintFrame(const char* prefix, const uint8* data, uint32 len)
{
    printf("%s", prefix);
    for (uint32 i = 0; i < len; i++)
    {
        printf("%02X ", data[i]);
    }
    printf("\n");
}

void Test_PrintSecurityStatus(void)
{
    SA_SecurityStatusReturnType status;

    if (Diag_SecurityAccess_GetSecurityStatus(&status) != E_OK)
    {
        printf("Security Status: ERROR\n");
        return;
    }

    if (status == SECURITY_UNLOCKED)
        printf("Security Status: UNLOCKED\n");
    else if (status == SECURITY_LOCKED)
        printf("Security Status: LOCKED\n");
    else
        printf("Security Status: PENDING\n");
}

/* ================= Low level ================= */

Std_ReturnType Test_SendDiagRequest(
    const uint8* reqData,
    uint32 reqLen,
    DiagMsgType* msg
)
{
    if ((reqData == NULL) || (msg == NULL))
    {
        return E_NOT_OK;
    }

    msg->reqDataLen = reqLen;

    for (uint32 i = 0; i < reqLen; i++)
    {
        msg->reqData[i] = reqData[i];
    }

    Test_PrintFrame("Tester: ", msg->reqData, msg->reqDataLen);

    return Diag_Services_Entry(msg);
}

/* ================= Service 27 helpers ================= */

Std_ReturnType Test_RequestSeed(DiagMsgType* msg)
{
    uint8 req[2] = {0x27, 0x01};

    if (Test_SendDiagRequest(req, 2, msg) != E_OK)
    {
        return E_NOT_OK;
    }

    Test_PrintFrame("ECU:    ", msg->resData, msg->resDataLen);
    Test_PrintSecurityStatus();

    return E_OK;
}

Std_ReturnType Test_SendKey(const uint8* key, uint32 keyLen, DiagMsgType* msg)
{
    uint8 req[18];

    if (keyLen != 16)
    {
        return E_NOT_OK;
    }

    req[0] = 0x27;
    req[1] = 0x02;

    for (uint32 i = 0; i < 16; i++)
    {
        req[i + 2] = key[i];
    }

    if (Test_SendDiagRequest(req, 18, msg) != E_OK)
    {
        return E_NOT_OK;
    }

    Test_PrintFrame("ECU:    ", msg->resData, msg->resDataLen);
    Test_PrintSecurityStatus();

    return E_OK;
}

/* ================= High level scenario ================= */

Std_ReturnType Test_UnlockECU(void)
{
    DiagMsgType msg;
    uint8 dummyKey[16] = {0}; /* test key */

    if (Test_RequestSeed(&msg) != E_OK) 
    {
        printf("Unlock ECU: RequestSeed FAILED\n");
        return E_NOT_OK;
    }

    if (Test_SendKey(dummyKey, 16, &msg) != E_OK)
    {
        printf("Unlock ECU: SendKey FAILED\n");
        return E_NOT_OK;
    }

    printf("Unlock ECU: DONE\n");
    return E_OK;
}
