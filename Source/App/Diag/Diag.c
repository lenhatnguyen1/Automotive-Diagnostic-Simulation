/**
 * @file Diag.c
 * @author Nguyen Le
 * @brief 
 * @version 0.1
 * @date 2025-11-18
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "Diag.h"
#include "SecurityAccess.h"

Std_ReturnType Diag_Services_Router(DiagMsgType* Msg);

uint8 Diag_SecurityAccess_Init_State = DIAG_UNINITIALIZED;

/* ============================================================
 *  Static service dispatch table
 *  Index = Service ID (0x00 – 0xFF)
 * ============================================================ */
static Diag_ServiceType Diag_ServiceTable[256] =
{
    [SERVICE_27] = Diag_SecurityAccess_Entry,

    /* future extension */
    /* [SERVICE_11] = Diag_ECUReset_Processor, */
    /* [SERVICE_22] = Diag_ReadDID_Processor, */
};

void Diag_Init (void)
{
    Diag_SecurityAccess_Init_State = DIAG_INITIALIZED;
    Diag_SecurityAccess_Init();
    printf("Call Diag_Init()\n");
}

void Diag_MainFunction (void)
{

}

Std_ReturnType Diag_Services_Entry(DiagMsgType* Msg)
{
    if (Diag_SecurityAccess_Init_State == DIAG_UNINITIALIZED)
    {
        return NOT_OK;
    }

    return Diag_Services_Router(Msg);
}

Std_ReturnType Diag_Services_Router(DiagMsgType* Msg)
{
    printf("Call Diag_Services_Router()\n");
    uint8 sid;

    sid = Msg->reqData[0];

    /* ---- Dispatch ---- */
    if (Diag_ServiceTable[sid] != NULL)
    {
        return Diag_ServiceTable[sid](Msg);
    }

    /* ---- Unsupported Service ---- */
    Msg->resData[0] = NEGATIVE_RESPONSE;
    Msg->resData[1] = sid;
    Msg->resData[2] = NRC_11; /* ServiceNotSupported */
    Msg->resDataLen = 3;

    return NOT_OK;
}
