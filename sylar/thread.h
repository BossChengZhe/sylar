#ifndef __SYLAR_THREAD_H__
#define __SYLAR_THREAD_H__
#include <thread>
#include <functional>
#include <memory>
#include <pthread.h>
#include <semaphore.h>
#include <stdint.h>
#include <atomic>
#include "mutex.h"


// 为什么选择pthread而不是去选择std::thread，std::thread只有单一的锁，不存在读写分离的锁

namespace sylar {

class Thread {
public:
    typedef std::shared_ptr<Thread> ptr;
    Thread(std::function<void()> cb, const std::string &name);
    ~Thread();

    pid_t getID() const { return m_id; }
    const std::string &getName() const { return m_name; }

    void join();

    static Thread *GetThis();
    static const std::string &GetName();
    static void SetName(const std::string &name);

private:
    Thread(const Thread &) = delete;
    Thread(const Thread &&) = delete;
    Thread &operator=(const Thread &) = delete;
    static void *run(void* arg);

private:
    pid_t m_id = -1;
    pthread_t m_thread = 0;
    std::function<void()> m_cb;
    std::string m_name;

    Semaphore m_semaphore;
};
}

#endif