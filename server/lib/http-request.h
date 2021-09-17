#ifndef __REQUEST__
#define __REQUEST__

#include <unordered_map>
#include <string>

class HttpRequest {
private:
    static const uint BUFF_SIZE = 4096;

    int conn;
    int content_length;
    int bytes_read;
    int bytes_buffered;

    std::unordered_map<std::string, std::string> headers;
    std::string _path;
    std::string _method;
    double _version;

    char buffer[BUFF_SIZE];
public:
    HttpRequest(int conn);

    std::string path();
    std::string method();
    double version();
    std::string header(std::string _name);

    int read(std::ostream& stream);
};

#endif // __REQUEST__
