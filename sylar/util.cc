#include "util.h"
#include <execinfo.h>
#include "log.h"
#include "fiber.h"

namespace sylar{

sylar::Logger::ptr g_logger = SYLAR_LOG_NAME("system");

/*
 * @brief 返回当前线程的ID
 */
pid_t GetThreadId()
{
    return syscall(SYS_gettid);
}

u_int32_t GetFiberId() {
    return sylar::Fiber::GetFiberId();
}

void Backtrace(std::vector<std::string> &bt, int size, int skip ){
    void **array = (void **)malloc((sizeof(void *) * size));
    size_t s = ::backtrace(array, size);

    char **strings = backtrace_symbols(array, s);
    if(strings == NULL) {
        SYLAR_LOG_ERROR(g_logger) << "backtrace_symbols error";
        return;
    }
    for(size_t i = skip; i < s; i++) {
        bt.push_back(strings[i]);
    }
    free(strings);
    free(array);
}

std::string BacktraceToString(int size, int skip, const std::string& prefix){
    std::vector<std::string> bt;
    Backtrace(bt, size, skip);
    std::stringstream ss;
    for (auto &i : bt) {
        ss << prefix << i << std::endl;
    }
    return ss.str();
}
}