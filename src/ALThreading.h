#pragma once
#if defined (_WIN32) || defined (_WIN64)

    #define AL_MUTEX HANDLE

    #define AL_MUTEX_INIT(m) m = CreateMutex(NULL, FALSE, NULL)

    #define AL_MUTEX_LOCK(m) WaitForSingleObject(m, INFINITE)

    #define AL_MUTEX_UNLOCK(m) ReleaseMutex(m)

    #define AL_MUTEX_DESTROY(m) CloseHandle(m)

    inline void sleep(unsigned milliseconds)
    {
        Sleep(milliseconds);
    }

#else

    #define AL_MUTEX pthread_mutex_t

    #define AL_MUTEX_INIT(m) pthread_mutex_init(&m, NULL)

    #define AL_MUTEX_LOCK(m) pthread_mutex_lock(&m)

    #define AL_MUTEX_UNLOCK(m) pthread_mutex_unlock(&m)

    #define AL_MUTEX_DESTROY(m) pthread_mutex_destroy(&m)

    #include <unistd.h>
    void sleep(unsigned milliseconds)
    {
        usleep(milliseconds * 1000); // takes microseconds
    }
#endif