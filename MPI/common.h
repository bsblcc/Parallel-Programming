#ifndef COMMON_H
#define COMMON_H

#include "buffer.h"
#include <cstring>
#include <cstdio>
#include "mpi.h"
#include <cstdarg>

extern int n_worker;

extern int my_rank;




extern void spin_forever();

extern void log(const char *format, ...);


#endif