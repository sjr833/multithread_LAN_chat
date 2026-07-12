//项目根目录，客户端入口
#include "net/client_socket.h"
#include "ui/client_ui.h"
int main() {
    // 本地自测写127.0.0.1；局域网联机改成虚拟机真实IP
    ClientSocket sock("127.0.0.1",8888);
    run_ui(sock);
    return 0;
}