// # pragma once

// #include "CryptoTypes.h"

// typedef uint8 Crypto_KeyType;

// typedef enum {
//     CRYPTO_JOB_IDLE,
//     CRYPTO_JOB_PENDING,
//     CRYPTO_JOB_PROCESSING,
//     CRYPTO_JOB_DONE,
//     CRYPTO_JOB_ERROR
// } Crypto_JobState;

// typedef struct {
//     uint8 material[16];
//     uint8 length;
//     Crypto_KeyState state;
// } Crypto_Key;

// typedef enum {
//     CRYPTO_ALG_CMAC,
//     CRYPTO_ALG_RANDOM
// } Crypto_Algorithm;

// typedef struct {
//     Crypto_Algorithm alg;
//     Crypto_OperationMode mode; // SINGLECALL / STREAMING
//     Crypto_Key* key;
//     uint8* input;
//     uint16 inputLen;
//     uint8* output;
//     uint16 outputLen;
//     Crypto_JobState state;
// } Crypto_Job;


// void Crypto_Init(void);
// void Crypto_MainFunction(void);
// Std_ReturnType Crypto_CMAC(Crypto_KeyType *key);
// Std_ReturnType Crypto_Random(Crypto_KeyType *key);