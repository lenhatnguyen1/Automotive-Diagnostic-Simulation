/**
 * @file Diag.h
 * @author Nguyen Le
 * @brief 
 * @version 0.1
 * @date 2025-11-18
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#pragma once

#include "DiagType.h"
#include "S27_SecurityAccess/SecurityAccess.h"

#include "SecurityAccess.h"

#define DIAG_INITIALIZED 1u
#define DIAG_UNINITIALIZED 0u

typedef Std_ReturnType (*Diag_ServiceType)(DiagMsgType* Msg);

void Diag_Init (void);

void Diag_MainFunction (void);

Std_ReturnType Diag_Services_Entry(DiagMsgType* Msg);

