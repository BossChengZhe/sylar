#ifndef __STLAR_UTIL_H__
#define __STLAR_UTIL_H__
#include <sys/types.h>
#include <sys/syscall.h>
#include <unistd.h>  
#include <pthread.h>
#include <stdint.h>
#include <string>
#include <vector>

namespace sylar{

pid_t GetThreadId();
u_int32_t GetFiberId();

void Backtrace(std::vector<std::string> &bt, int size, int skip = 1);
std::string BacktraceToString(int size, int skip=2, const std::string& prefix="");
}
#endif