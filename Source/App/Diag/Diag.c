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


void Diag_MainFunction ()
{


}

Std_ReturnType Diag_Services_Router (DiagMsgType* Msg)
{
    Std_ReturnType ret = NOT_OK;

    switch (Msg->reqData[0])
    {
        case SERVICE_27:
            ret = Diag_SecurityAccess_Proccessor(Msg);
            break;
        
        default:
            // Handle unsupported service
            ret = NOT_OK;
            break;
    }

    return ret;
}

// int main()
// {
//     DiagMsgType msg;
//     msg.reqData[0] = SERVICE_27;
//     msg.reqData[1] = 0x01; // subfunction: request seed level 1
//     msg.reqDataLen = 2;
//     Diag_SecurityAccess_Proccessor(&msg);
//     printf("Response for Seed Request:\n");
//     for (uint8 i = 0; i < msg.resDataLen; i++)
//     {
//         printf("0x%02X ", msg.resData[i]);
//     }

//     msg.reqData[0] = SERVICE_27;
//     msg.reqData[1] = 0x02; // subfunction: send key level 1
//     // prepare key based on the seed received previously
//     for (uint8 i = 0; i < 8; i++)
//     {
//         msg.reqData[i + 2] = 0x01; // Example key, should be computed based on the seed
//     }
//     msg.reqDataLen = 10;
//     Diag_SecurityAccess_Proccessor(&msg);
//     printf("\nResponse for Key Comparison:\n");
//     for (uint8 i = 0; i < msg.resDataLen; i++)
//     {
//         printf("0x%02X ", msg.resData[i]);
//     }
//     return 0;
// }
