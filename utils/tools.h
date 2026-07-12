//线程安全日志工具
#pragma once
#include <mutex>   //提供互斥锁std::mutex,做多线程临界区加锁
#include <iostream>
#include <ctime>    //提供系统时间接口time(),ctime(),给日志拼接时间戳
std::mutex global_log_mtx;  //全局互斥锁,保证日志打印线程安全
inline void log_print(const std::string& content) {
    //若锁空闲则加锁,若锁被占用则阻塞等待,直到锁空闲
    global_log_mtx.lock();
    //读取系统UNIX时间戳,（从1970-01-01到当下的时间所经过的秒数）
    time_t now = time(nullptr);
    //将整形秒级时间戳转换成人类可读的字符串格式,并打印到日志中
    std::cout << "[" << ctime(&now) << "] LOG: " << content << "\n";
    //解锁,允许其他线程访问
    global_log_mtx.unlock();
}