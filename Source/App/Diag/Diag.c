/*
@brief This file is for Diagnostic 
*/

#include "Diag.h"

static uint8 Diag_SecurityAccess_L1_InternalSeed_buffer[8];
static uint8 Diag_SecurityAccess_L1_InternalKey_buffer[8];

static uint8 simple_Algo (uint8 input);

Std_ReturnType Diag_SecurityAccess_L1_GetSeed (Seed_Type* Seed);

Std_ReturnType Diag_SecurityAccess_L1_CompareKey (Key_Type Key[]);

Std_ReturnType Diag_SecurityAccess_L1_CalculateKey (Seed_Type Seed[], Key_Type* Key);

Std_ReturnType Diag_SecurityAccess_Handler (DiagMsgContextType* Msg);

static uint8 simple_Algo (uint8 input)
{
    uint8 output;
    if (input == 255)
    {
        output = 0x00;
        return 0;
    }

    output = 0xFF;

    return output;
}

Std_ReturnType Diag_SecurityAccess_L1_GetSeed (Seed_Type* Seed)
{
    Seed_Type challengeSeed;  
    for (uint8 i = 0; i < 8; i++)
    {
        challengeSeed = 0xFF;
        Seed[i] = challengeSeed;
        Diag_SecurityAccess_L1_InternalSeed_buffer[i] = Seed[i]; // write to global buffer
    }

    return E_OK;
}

Std_ReturnType Diag_SecurityAccess_L1_CompareKey (Key_Type Key[])
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

Std_ReturnType Diag_SecurityAccess_L1_CalculateKey (Seed_Type Seed[], Key_Type* Key)
{
    for (uint8 i = 0; i < 8; i++)
    {
        Key[i] = simple_Algo(Seed[i]);
    }

    return E_OK;
}

Std_ReturnType Diag_SecurityAccess_Handler (DiagMsgContextType* Msg)
{
    uint8 seedLv1[8];
    uint8 keyLv1[8];
    uint8 subFuntion = Msg->reqData[1];
    if (subFuntion == 0x01)
    {
        (void)Diag_SecurityAccess_L1_GetSeed(seedLv1);
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
        (void)Diag_SecurityAccess_L1_CalculateKey(Diag_SecurityAccess_L1_InternalSeed_buffer, Diag_SecurityAccess_L1_InternalKey_buffer);
        if (Diag_SecurityAccess_L1_CompareKey(keyLv1) == E_OK)
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

void Diag_MainFunction ()
{

}
