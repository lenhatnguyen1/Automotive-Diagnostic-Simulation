/**
 * @brief Crypto module
 * 
 */



void Crypto_MainFunction(void)
{
    if (job.state == CRYPTO_JOB_PENDING)
    {
        job.state = CRYPTO_JOB_PROCESSING;
        Crypto_ExecuteSingleCall(&job);
        job.state = CRYPTO_JOB_DONE;
    }
}
