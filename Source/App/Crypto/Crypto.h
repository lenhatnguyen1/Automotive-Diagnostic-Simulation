# pragma once

#include "Types.h"

typedef uint8 Crypto_KeyType;

void Crypto_Init(void);
void Crypto_MainFunction(void);
Std_ReturnType Crypto_CMAC(Crypto_KeyType *key);
Std_ReturnType Crypto_Random(Crypto_KeyType *key);