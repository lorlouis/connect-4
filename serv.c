#ifdef _WIN32

#include <winsock2.h>
#include <io.h>
#include <ws2tcpip.h>

#define SO_REUSEPORT 0

#else

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#endif

#include <string.h>

#include "net.h"
#include "con4.h"

/* returns -1 on error, check perror for more info */
/* shit abi modeled after strtok so you know it is shit */
int listen_for_sock_fd(int port) {

    static int server_fd = 0xdeadbeef;
    static int lasport = 0;
    int new_sock;
    static struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    if(server_fd == 0xdeadbeef && (port != 0 && port != lasport)) {

        if(!(server_fd = socket(AF_INET, SOCK_STREAM, 0))) {
            return -1;
        }
        /* bind */
        if(setsockopt(
                server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                &opt, sizeof(opt))) {
            return -1;
        }

        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(port);

        if(bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
            return -1;
        }
        if (listen(server_fd, 3) < 0)
        {
            return -1;
        }
        lasport = port;
    }
    if((new_sock = accept(server_fd, (struct sockaddr *)&address,
                          (socklen_t*)&addrlen))<0) {
        return -1;
    }
    return new_sock;
}
