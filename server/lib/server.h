#ifndef __SERVER__
#define __SERVER__

#include <string>
#include <thread>
#include <http-request.h>
#include <http-response.h>

typedef void (*RequestHandler)(HttpRequest &, HttpResponse &);

class HttpServer {
private:
    RequestHandler handler;
public:
    HttpServer(RequestHandler handler);

    std::thread * listen (std::string host, unsigned port);
};

#endif // __SERVER__
