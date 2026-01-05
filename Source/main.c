#include <stdio.h>

#include "Diag.h"

int main()
{
    uint8 requestSeed[2] = {0x27, 0x01};
    uint8 sendKey[18] = {0x27, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    DiagMsgType msg;

    printf("Call main()\n");

    Diag_Init();

    msg.reqDataLen = 2;

    for (uint32 i = 0; i < 2; i++)
    {
        msg.reqData[i] = requestSeed[i];
    }

    Diag_Services_Entry(&msg);

    for (uint32 i = 0; i < msg.resDataLen; i++)
    {
        printf("Response Byte %d: 0x%02X\n", i, msg.resData[i]);
    }

    msg.reqDataLen = 18;

    for (uint32 i = 0; i < 18; i++)
    {
        msg.reqData[i] = sendKey[i];
    }

    Diag_Services_Entry(&msg);

    for (uint32 i = 0; i < msg.resDataLen; i++)
    {
        printf("Response Byte %d: 0x%02X\n", i, msg.resData[i]);
    }

    return 0;
}
