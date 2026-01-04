/**
 * @file SecurityAccess.h
 * @author NguyenLe
 * @brief 
 * @version 0.1
 * @date 2025-12-07
 * 
 * @copyright Copyright (c) 2025
 * 
 */


#pragma once

#include "DiagType.h"

typedef enum
{
    DIAG_SEC_LOCKED = 0x00,
    DIAG_SEC_WAITING_FOR_KEY = 0x01,
    DIAG_SEC_UNLOCKED = 0x02,
    DIAG_SEC_UNLOCKED_AND_WAITING_FOR_KEY = 0x03,
    
} Diag_SecurityStatusType;

#define SECURITY_ACCESS_INITIALIZED 1u
#define SECURITY_ACCESS_UNINITIALIZED 0u

#define SECURITY_ACCESS_SEED_LENGTH 16u
#define SECURITY_ACCESS_KEY_LENGTH 16u


void Diag_SecurityAccess_Init (void);
Std_ReturnType Diag_SecurityAccess_Proccessor (DiagMsgType* Msg);