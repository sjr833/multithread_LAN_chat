//聊天业务逻辑
#pragma once
#include "../repository/mysql_helper.h"
#include "../repository/redis_helper.h"
#include "../net/server_socket.h"
#include "../model/data.h"
#include <sys/socket.h>
class ChatService {
public:
    MysqlHelper mysql;
    RedisHelper redis;
    // 登录：注册/拉取用户uid，标记上线
    User login(ServerSocket& server,int fd,std::string nick) {
        int uid = mysql.get_or_create_user(nick);
        User user{uid,nick,fd};
        server.fd_to_user[fd]=user;
        redis.user_online(uid,fd);
        log_print("用户登录："+nick+" uid:"+std::to_string(uid));
        return user;
    }
    // 群聊广播消息
    void broadcast_msg(ServerSocket& server,Message msg) {
        mysql.insert_msg(msg.sender_uid,-1,msg.content);
        std::string pack_msg = "[群聊]"+std::to_string(msg.sender_uid)+"："+msg.content;
        for(auto& item:server.fd_to_user) {
            send(item.first,pack_msg.c_str(),pack_msg.size(),0);
        }
    }
    // 用户下线清理缓存
    void offline(ServerSocket& server,int fd) {
        User u = server.fd_to_user[fd];
        redis.user_offline(u.uid,fd);
        server.fd_to_user.erase(fd);
        close(fd);
        log_print("用户下线："+u.nick_name);
    }
};