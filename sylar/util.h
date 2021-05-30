#ifndef __STLAR_UTIL_H__
#define __STLAR_UTIL_H__
#include <sys/types.h>
#include <sys/syscall.h>
#include <unistd.h>  
#include <pthread.h>
#include <stdint.h>

namespace sylar{

pid_t GetThreadId();
u_int32_t GetFiberId();

}
#endif