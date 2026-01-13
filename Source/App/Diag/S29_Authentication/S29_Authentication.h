/**
 * @file Diag_Authentication.h
 * @brief Service 0x29 - ACR-style Authentication (Challenge-Response)
 * @author NguyenLe
 * @version 0.4
 * @date 2026-12-13
 *
 * @note Non-AUTOSAR implementation
 *       Entry-based design, similar to Service 0x27.
 */

#pragma once

#include "DiagTypes.h"     /* DiagMsgType */

/* ================================
 * Authentication Constants
 * ================================ */
#define AUTH_MAX_ALGO_INDICATOR_SIZE   16U
#define AUTH_MAX_PROOF_SIZE            256U
#define AUTH_MAX_CHALLENGE_SIZE        256U
#define AUTH_MAX_ADDITIONAL_PARAM      64U

/* ================================
 * Authentication SubFunctions
 * ================================ */
#define AUTH_SUBFUNC_RCFA   0x05U  /* requestChallengeForAuthentication */
#define AUTH_SUBFUNC_VPOW   0x06U  /* verifyProofOfOwnershipUnidirectional */

/* ================================
 * Internal Return/Error Codes
 * (used inside .c only)
 * ================================ */
typedef enum
{
    AUTH_OK = 0,
    AUTH_ERR_INVALID_SESSION,
    AUTH_ERR_INVALID_SECURITY_LEVEL,
    AUTH_ERR_INVALID_KEY,
    AUTH_ERR_NOT_INITIALIZED,
    AUTH_ERR_INTERNAL,
    AUTH_ERR_INVALID_LENGTH,
    AUTH_ERR_UNKNOWN_SUBFUNCTION
} Auth_ReturnType;

/* ================================
 * Public API
 * ================================ */

/**
 * @brief Initialize Authentication service
 */
void Diag_Authentication_Init(void);

/**
 * @brief Deinitialize Authentication service
 */
void Diag_Authentication_DeInit(void);

/**
 * @brief Main function for asynchronous handling (optional)
 *        Should be called cyclically by scheduler
 */
void Diag_Authentication_MainFunction(void);

/**
 * @brief Entry point for Diagnostic Authentication service (SID 0x29)
 *
 * @param Msg Pointer to diagnostic message context
 * @return Std_ReturnType E_OK / E_NOT_OK
 *
 * @note Called directly by Diagnostic Router
 */
Std_ReturnType Diag_Authentication_Entry(DiagMsgType* Msg);
