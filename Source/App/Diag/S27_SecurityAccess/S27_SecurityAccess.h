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

#include "DiagTypes.h"

#define SECURITY_ACCESS_INITIALIZED 1u
#define SECURITY_ACCESS_E_UNINITIALIZED 0u

#define SECURITY_ACCESS_SEED_LENGTH 16u
#define SECURITY_ACCESS_KEY_LENGTH 16u

#define SEC_LEVEL_1 0x01u

typedef uint8 SA_SeedType;
typedef uint8 SA_KeyType;
typedef uint8 SA_SecLevelType;
typedef uint8 SA_CurrentSecurityLevel;

typedef enum
{
    SECURITY_LOCKED = 0,
    SECURITY_UNLOCKED = 1,
    SECURITY_PENDING = 2,   
} SA_SecurityStatusReturnType;

typedef enum
{
    DIAG_SEC_LOCKED = 0x00,
    DIAG_SEC_WAITING_FOR_KEY = 0x01,
    DIAG_SEC_UNLOCKED = 0x02,
    DIAG_SEC_UNLOCKED_AND_WAITING_FOR_KEY = 0x03,
    
} SA_SecurityStatusType;


/* Public API */

/**
 * @brief Init Security Access module
 * 
 */
void Diag_SecurityAccess_Init (void);

/**
 * @brief Entry of Security Access processs
 * 
 * @param Msg 
 * @return Std_ReturnType 
 */
Std_ReturnType Diag_SecurityAccess_Entry(DiagMsgType* Msg);

/**
 * @brief Get the security status for verifying protected object
 * 
 * @param status 
 * @return Std_ReturnType 
 */
Std_ReturnType Diag_SecurityAccess_GetSecurityStatus(SA_SecurityStatusReturnType* status);
