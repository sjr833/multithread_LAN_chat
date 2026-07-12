//项目根目录，服务端入口
#include "net/server_socket.h"
#include "service/chat_service.h"
#include <cstring>
int main() {
    ServerSocket server;
    ChatService service;
    epoll_event events[MAX_EVENTS];
    char buf[1024];
    while(true) {
        int event_cnt = server.wait_event(events);
        for(int i=0;i<event_cnt;i++) {
            int fd = events[i].data.fd;
            // 新客户端连接
            if(fd == server.listen_fd) {
                server.accept_client();
                continue;
            }
            memset(buf,0,sizeof(buf));
            int recv_len = recv(fd,buf,sizeof(buf),0);
            if(recv_len<=0) {
                service.offline(server,fd);
                continue;
            }
            std::string payload(buf);
            // 初次数据包=昵称，执行登录
            if(!server.fd_to_user.count(fd)) {
                service.login(server,fd,payload);
                continue;
            }
            User sender = server.fd_to_user[fd];
            Message msg{sender.uid,-1,payload};
            service.broadcast_msg(server,msg);
        }
    }
    return 0;
}