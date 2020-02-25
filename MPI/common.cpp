#include "common.h"






void spin_forever()
{
	while (true)
	{
		;
	}
}


void log(const char *format, ...)
{

    return;
	//pthread_mutex_lock(&debug_mutex);
    va_list args;
    va_start(args, format);

    fflush(stderr);
    fprintf(stderr, "pid#%d\t", my_rank);
    vfprintf(stderr, format, args);
    fflush(stderr);

    va_end(args);
    //pthread_mutex_unlock(&debug_mutex);
}
