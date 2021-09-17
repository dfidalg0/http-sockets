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
    HttpServer(RequestHandler handler);

    std::thread * listen (std::string host, unsigned port);
};

#endif // __SERVER__
