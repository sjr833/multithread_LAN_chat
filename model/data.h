//结构体统一存放
#pragma once
#include <string>
//用户结构体
struct User {
    int uid;
    std::string nick_name;
    int fd; //客户端socket文件描述符
};
//消息结构体
// target_uid=-1：群聊广播；其余数值代表私聊目标用户id
struct Message {
    int sender_uid;
    int target_uid;
    std::string content;
};