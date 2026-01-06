#pragma once

#include "Types.h"
#include "Diag.h"

/* ===== Low level ===== */
Std_ReturnType Test_SendDiagRequest(
    const uint8* reqData,
    uint32 reqLen,
    DiagMsgType* msg
);

/* ===== Service 27 helpers ===== */
Std_ReturnType Test_RequestSeed(DiagMsgType* msg);
Std_ReturnType Test_SendKey(const uint8* key, uint32 keyLen, DiagMsgType* msg);

/* ===== High level scenario ===== */
Std_ReturnType Test_UnlockECU(void);

/* ===== Utilities ===== */
void Test_PrintFrame(const char* prefix, const uint8* data, uint32 len);
void Test_PrintSecurityStatus(void);
