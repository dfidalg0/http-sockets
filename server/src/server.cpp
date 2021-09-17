#include <server.h>
#include <netinet/in.h>
#include <sys/unistd.h>
#include <string.h>
#include <netdb.h>

#include <bad-request.h>

HttpServer::HttpServer(RequestHandler handler) : handler(handler) {}

std::thread * HttpServer::listen(std::string hostname, unsigned port) {
    auto host = gethostbyname(hostname.c_str());

    if (!host) {
        return nullptr;
    }

    in_addr_t addr;

    memcpy(
        &addr,
        host->h_addr,
        host->h_length
    );

    sockaddr_in channel = {
        .sin_family = AF_INET,
        .sin_port = htons(port),
        .sin_addr = {
            .s_addr = addr
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
                auto res = HttpResponse(conn);

                try {
                    auto req = HttpRequest(conn);

                    this->handler(req, res);
                }
                catch (BadRequestException) {
                    res << "HTTP/1.1 400 Bad Request\r\n\r\n";
                }

                res.end();
            });

            conn_thread.detach();
        }
    });
}
