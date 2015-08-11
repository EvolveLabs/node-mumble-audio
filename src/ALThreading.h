#pragma once
#if defined (_WIN32) || defined (_WIN64)

    inline void sleep(unsigned milliseconds)
    {
        Sleep(milliseconds);
    }

#else

    #include <unistd.h>
    void sleep(unsigned milliseconds)
    {
        usleep(milliseconds * 1000); // takes microseconds
    }
#endif