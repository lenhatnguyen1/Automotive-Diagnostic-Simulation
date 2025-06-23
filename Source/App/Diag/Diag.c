/*
@brief This file is for Diagnostic 
*/

#include "Diag.h"

static uint8 Diag_SecurityAccess_L1_InternalSeed_buffer[8];
static uint8 Diag_SecurityAccess_L1_InternalKey_buffer[8];

static uint8 simple_Algo (int a)
{
    if (a == 255)
    {
        return 0;
    }
    return (a + 1);
}

Std_ReturnType SecurityAccess_UnlockedL1_GetSeed (uint8* Seed)
{
    uint8 challengeSeed;  
    for (uint8 i = 0; i < 8; i++)
    {
        challengeSeed = (uint8)(rand() % 256); // random value in 0–255
        Seed[i] = challengeSeed;
        Diag_SecurityAccess_L1_InternalSeed_buffer[i] = Seed[i]; // write to global buffer
    }

    return E_OK;
}

Std_ReturnType SecurityAccess_UnlockedL1_CompareKey (uint8 Key[])
{
    for (uint8 i = 0; i < 8; i++)
    {
        if(Key[i] != Diag_SecurityAccess_L1_InternalKey_buffer[i])
        {
            return E_NOT_OK;
        }
    } 
    return E_OK;
}

Std_ReturnType SecurityAccess_UnlockedL1_CalculateKey (uint8 Seed[], uint8* Key)
{
    for (uint8 i = 0; i < 8; i++)
    {
        Key[i] = simple_Algo(Seed[i]);
    }

    return E_OK;
}

Std_ReturnType SecurityAccess_Handler (DiagMsgContextType* Msg)
{
    uint8 seedLv1[8];
    uint8 keyLv1[8];
    uint8 subFuntion = Msg->reqData[1];
    if (subFuntion == 0x01)
    {
        (void)SecurityAccess_UnlockedL1_GetSeed(seedLv1);
        Msg->resData[0] = 0x67;
        Msg->resData[1] = 0x01;
        
        for (uint8 i = 0; i < 8; i++)
        {
            Msg->resData[i + 2] = seedLv1[i];
        }

        Msg->resDataLen = 10;
    }
    else if (subFuntion == 0x02)
    {
        for (uint8 i = 0; i < 8; i++)
        {
            keyLv1[i] = Msg->reqData[i+2];
        }
        (void)SecurityAccess_UnlockedL1_CalculateKey(Diag_SecurityAccess_L1_InternalSeed_buffer, Diag_SecurityAccess_L1_InternalKey_buffer);
        if (SecurityAccess_UnlockedL1_CompareKey(keyLv1) == E_OK)
        {
            Msg->resData[0] = 0x67;
            Msg->resData[1] = 0x02;
            Msg->resDataLen = 2;
        }
        else
        {
            Msg->resData[0] = NEGATIVE_RES_CODE;
            Msg->resData[1] = SERVICE_27_CODE;
            Msg->resData[2] = NRC_35;  // NRC: InvalidKey
            Msg->resDataLen = 3;
        }
    }
    return E_OK;
}

void DiagMainFunction ()
{

}
