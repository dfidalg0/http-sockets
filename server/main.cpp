#include <iostream>
#include <experimental/filesystem>
#include <fstream>
#include <sys/fcntl.h>
#include <sys/unistd.h>

#include <server.h>

namespace fs = std::experimental::filesystem;

using std::cout;
using std::endl;

int main(int argc, char const * argv[]) {
    if (argc != 4) {
        cout << "Usage: " << argv[0] << " [host] [port] [dir]" << endl;
        return 1;
    }

    const std::string dir = argv[3];

    const fs::path path(dir);

    if (!fs::is_directory(path)) {
        cout << argv[3] << ": is not a directory" << endl;
        return 1;
    }

    auto server = HttpServer([&path](HttpRequest& req, HttpResponse& res) {
        auto filepath = path / fs::path(req.path());

        if (!fs::is_regular_file(filepath)) {
            res
                << "HTTP/" << req.version() << " 404 Not Found\r\n"
                << "Content-Type: text/plain\r\n"
                << "\r\n"
                << "File not found\n";

            return;
        }

        auto size = fs::file_size(filepath);

        int fd = open(filepath.c_str(), O_RDONLY);

        res
            << "HTTP/" << req.version() << " 200 Ok\r\n"
            << "Content-length: " << size << "\r\n"
            << "\r\n";

        char buffer[4096];
        int n_bytes;

        while ((n_bytes = read(fd, buffer, 4096)) > 0) {
            res << std::string(buffer, buffer + n_bytes);
        }
    });

    auto * server_thread = server.listen(argv[1], atoi(argv[2]));

    if (!server_thread) {
        cout << "Error when trying to listen on " << argv[1] << ':' << argv[2] << endl;
        return 2;
    }

    std::cout << "Server started at " << argv[1] << ':' << argv[2] << endl;

    server_thread->join();

    return 0;
}
