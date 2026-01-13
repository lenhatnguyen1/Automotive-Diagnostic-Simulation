#pragma once

#include "Types.h"


#define NEGATIVE_RESPONSE 0x7F

#define SERVICE_27 0x27
#define SERVICE_27_POSITIVE_RESPONSE 0x67

#define SERVICE_29 0x29
#define SERVICE_29_POSITIVE_RESPONSE 0x69

#define NRC_10 0x10
#define NRC_11 0x11
#define NRC_12 0x12
#define NRC_13 0x13
#define NRC_24 0x24
#define NRC_31 0x31
#define NRC_35 0x35
#define NRC_36 0x36
#define NRC_37 0x37


typedef uint32 MsgLenType;
typedef uint8 MsgType;

typedef struct
{
    MsgType reqData[128];      // Request buffer (input)
    MsgLenType  reqDataLen;   // Length of request

    MsgType resData[128];      // Response buffer (output)
    MsgLenType  resDataLen;   // Current response length

    MsgType  sid;          // Service Identifier (extracted from reqData[0])
} DiagMsgType;