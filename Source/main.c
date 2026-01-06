/**
 * @file main.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2026-01-06
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#include <stdio.h>

#include "Diag.h"
#include "test.h"

int main(void)
{
    Diag_Init();

    (void)Test_UnlockECU();

    return 0;
}
