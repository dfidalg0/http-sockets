#include <server.h>
#include <netinet/in.h>
#include <sys/unistd.h>
#include <string.h>
#include <netdb.h>

#include <bad-request.h>

HttpServer::HttpServer(RequestHandler handler) : handler(handler) {}

std::thread * HttpServer::listen(std::string hostname, unsigned port) {
    // Resolução de IP do Host
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

    // Criação do canal do socket
    sockaddr_in channel = {
        .sin_family = AF_INET,
        .sin_port = htons(port),
        .sin_addr = {
            .s_addr = addr
        }
    };

    // Criação do socket TCP
    auto sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    // Reserva de uma porta para o socket
    if (bind(sock, (sockaddr *) &channel, sizeof(channel)) < 0) {
        return nullptr;
    }

    int on = 1;

    // Opções do socket
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char *) &on, sizeof(on));

    if (::listen(sock, 10) < 0) {
        return nullptr;
    }

    // Criação da thread do servidor
    return new std::thread([this, sock]() {
        // Loop de conexões
        while (true) {
            // Espera do servidor por uma conexão nova
            auto conn = accept(sock, 0, 0);

            // Criação de uma thread para atender a conexão
            auto conn_thread = std::thread([this, conn]() {
                // Criação do objeto de resposta
                auto res = HttpResponse(conn);

                try {
                    // Criação do objeto de requisição
                    auto req = HttpRequest(conn);

                    // Execução da função
                    this->handler(req, res);
                }
                catch (BadRequestException) {
                    // Em caso de má formatação da requisição, o status 400 é
                    // enviado
                    res
                        << "HTTP/1.1 400 Bad Request\r\n"
                        << "Content-Type: text/plain\r\n"
                        << "\r\n"
                        << "Bad Request\n";
                }

                // Fechamento da conexão
                res.end();
            });

            // Desvinculação da thread de conexão da linha de execução principal
            conn_thread.detach();
        }
    });
}
