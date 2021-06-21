#include <iostream>
#include "sylar/log.h"
#include "sylar/util.h"


int main(int argc, char const *argv[])
{
    sylar::Logger::ptr logger(new sylar::Logger);
    logger->addAppender(sylar::LogAppender::ptr(new sylar::StdOutLogAppender));

    sylar::FileLogAppender::ptr fileappender(new sylar::FileLogAppender("./log.txt"));

    sylar::LogFormatter::ptr fmt(new sylar::LogFormatter("%d%T%p%T%m%n"));         // 自定义一种新的日志格式
    fileappender->setFormatter(fmt);
    fileappender->setLevel(sylar::LogLevel::ERROR);
    logger->addAppender(fileappender);

    // sylar::LogEvent::ptr event(new sylar::LogEvent(__FILE__, __LINE__, 0, sylar::GetThreadId()
    //                                               , sylar::GetFiberId(), time(0)));
    // event->getSS() << "I love you!";

    // logger->log(sylar::LogLevel::DEBUG, event);

    SYLAR_LOG_FMT_INFO(logger, "test macro fmt error %s", "aa");

    // SYLAR_LOG_INFO(logger) << "test macro";
    // SYLAR_LOG_ERROR(logger) << "test macro";

    auto l = sylar::LoggerMgr::GetInstance()->getLogger("xx");
    SYLAR_LOG_INFO(l) << "xxx";
    
    return 0;
}
