#pragma once
#if defined (_WIN32) || defined (_WIN64)

    inline void sleep_ms(unsigned milliseconds)
    {
        Sleep(milliseconds);
    }

#else

    #include <unistd.h>

    inline void sleep_ms(unsigned miliseconds)
    {
    	sleep(miliseconds * 1000);
    }

#endif