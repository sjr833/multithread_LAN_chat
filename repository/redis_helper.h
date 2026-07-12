//Redis缓存封装(维护在线用户集合)
#pragma once
#include <hiredis/hiredis.h>
#include "../utils/tools.h"
#include <string>
class RedisHelper {
private:
    redisContext* ctx;
public:
    RedisHelper() {
        ctx = redisConnect("127.0.0.1",6379);
        if(ctx->err){
            log_print("Redis连接失败");
            exit(1);
        }
    }
    // 用户上线：加入在线集合
    void user_online(int uid,int fd) {
        redisReply* rep = (redisReply*)redisCommand(ctx,"SADD online_user %d:%d",uid,fd);
        freeReplyObject(rep);
    }
    // 用户下线：移出在线集合
    void user_offline(int uid,int fd) {
        redisReply* rep = (redisReply*)redisCommand(ctx,"SREM online_user %d:%d",uid,fd);
        freeReplyObject(rep);
    }
    ~RedisHelper(){
        redisFree(ctx);
    }
};