//MySQL封装类
#pragma once
#include <mysql/mysql.h>
#include "../model/data.h"
#include "../utils/tools.h"
#include <string>
class MysqlHelper {
private:
    MYSQL* conn;
public:
    MysqlHelper() {
        conn = mysql_init(nullptr);
        // 数据库地址、账号、密码、库名，和前面SQL配置匹配
        if(!mysql_real_connect(conn,"127.0.0.1","chat_user","123456","chat_db",3306,nullptr,0)){
            log_print("MySQL连接失败:"+std::string(mysql_error(conn)));
            exit(1);
        }
    }
    // 昵称注册/查询用户，返回uid，不存在就插入新用户
    int get_or_create_user(const std::string& nick) {
        std::string sql = "SELECT id FROM chat_user WHERE nick_name='"+nick+"'";
        mysql_query(conn,sql.c_str());
        MYSQL_RES* res = mysql_store_result(conn);
        MYSQL_ROW row;
        if((row=mysql_fetch_row(res))!= nullptr) {
            int uid = std::stoi(row[0]);
            mysql_free_result(res);
            return uid;
        }
        mysql_free_result(res);
        sql = "INSERT INTO chat_user(nick_name) VALUES('"+nick+"')";
        mysql_query(conn,sql.c_str());
        return (int)mysql_insert_id(conn);
    }
    // 存入聊天消息
    void insert_msg(int sender,int receiver,const std::string& msg) {
        std::string sql = "INSERT INTO chat_msg(sender_id,receiver_id,msg_content) VALUES("
        +std::to_string(sender)+","+(receiver==-1?"NULL":std::to_string(receiver))+",'"+msg+"')";
        mysql_query(conn,sql.c_str());
    }
    ~MysqlHelper(){
        mysql_close(conn);
    }
};
        
