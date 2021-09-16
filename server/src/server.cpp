#include <server.h>
#include <netinet/in.h>
#include <sys/unistd.h>
#include <iostream>
#include <sstream>

HttpServer::HttpServer(RequestHandler handler) : handler(handler) {}

std::thread * HttpServer::listen(std::string host, unsigned port) {
    sockaddr_in channel = {
        .sin_family = AF_INET,
        .sin_port = htons(port),
        .sin_addr = {
            .s_addr = htonl(INADDR_ANY)
        }
    };

    auto sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (bind(sock, (sockaddr *) &channel, sizeof(channel)) < 0) {
        return nullptr;
    }

    int on = 1;

    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char *) &on, sizeof(on));

    if (::listen(sock, 10) < 0) {
        return nullptr;
    }

    return new std::thread([this, sock]() {
        while (true) {
            auto conn = accept(sock, 0, 0);

            auto conn_thread = std::thread([this, conn]() {
                auto req = HttpRequest(conn);
                auto res = HttpResponse(conn);

                this->handler(req, res);

                close(conn);
            });

            conn_thread.detach();
        }
    });
}
