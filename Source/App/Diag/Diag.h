#pragma once

#include "Type.h"

typedef uint32 MsgLenType;
typedef uint8 MsgType;
#define NEGATIVE_RES_CODE 0x7F
#define SERVICE_27_CODE 0x27

#define NRC_12 0x12
#define NRC_13 0x13
#define NRC_24 0x24
#define NRC_35 0x35
#define NRC_36 0x36
#define NRC_37 0x37

typedef uint8 Seed_Type;
typedef uint8 Key_Type;


typedef struct
{
    MsgType reqData[128];      // Request buffer (input)
    MsgLenType  reqDataLen;   // Length of request

    MsgType resData[128];      // Response buffer (output)
    MsgLenType  resDataLen;   // Current response length

    MsgType  sid;          // Service Identifier (extracted from reqData[0])
} DiagMsgContextType;

