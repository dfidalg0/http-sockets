#include <iostream>
#include <server.h>

using std::cout;
using std::endl;

int main(int argc, char const * argv[]) {
    if (argc != 4) {
        cout << "Usage: " << argv[0] << " [host] [port] [dir]" << endl;
        return 1;
    }

    auto server = HttpServer([](HttpRequest& req, HttpResponse& res) {
        cout << "Connection established" << endl;

        res
            .write("HTTP/1.1 200 Ok\r\n")
            .write("Content-length: 3\r\n")
            .write("Content-type: text/plain\r\n")
            .write("\r\n")
            .write(":D\n");
    });

    auto * server_thread = server.listen("", 8080);

    if (!server_thread) {
        cout << "Error when trying to listen on " << argv[1] << ':' << argv[2] << endl;
        return 2;
    }

    server_thread->join();

    return 0;
}
