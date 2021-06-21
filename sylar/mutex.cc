#include "mutex.h"
namespace sylar {

Semaphore::Semaphore(uint32_t count){
    /*
    * int sem_init(sem_t *sem, int pshared, unsigned int value);
    * 在指向sem的指针，初始化一个未命名的信号量指针
    * pshared：是否线程间共享
    * value 信号量的初始值
    */
    if(sem_init(&m_semaphore, 0, count)) {
        throw std::logic_error("sem_init error");
    }
}

Semaphore::~Semaphore(){
    sem_destroy(&m_semaphore);  
}

void Semaphore::wait(){
    if(sem_wait(&m_semaphore)) {
        throw std::logic_error("sem wait error");
    }

}

void Semaphore::notify(){
    if(sem_post(&m_semaphore)) {
            throw std::logic_error("sem post error");
    }
}
}