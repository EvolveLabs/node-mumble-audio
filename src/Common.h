#pragma once
#if defined (_WIN32) || defined (_WIN64)

    inline void sleep(unsigned milliseconds)
    {
        Sleep(milliseconds);
    }

#else

    #include <unistd.h>

#endif