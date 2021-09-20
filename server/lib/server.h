#ifndef __SERVER__
#define __SERVER__

#include <string>
#include <thread>
#include <functional>
#include <http-request.h>
#include <http-response.h>

typedef std::function<void(HttpRequest &, HttpResponse &)> RequestHandler;

class HttpServer {
private:
    RequestHandler handler;
public:
    /**
     * Construtor da classe HttpServer
     *
     * @param handler Função de processamento de solicitações HTTP
     */
    HttpServer(RequestHandler handler);

    /**
     * Inicia o servidor no host e porta especificados
     *
     * @param hostname Nome do host em que o servidor irá ouvir
     * @param port Porta em que o servidor irá ouvir
     *
     * @returns Ponteiro para a thread principal que executa o loop de conexões
     */
    std::thread * listen (std::string hostname, unsigned port);
};

#endif // __SERVER__
