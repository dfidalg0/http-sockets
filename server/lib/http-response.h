#ifndef __RESPONSE__
#define __RESPONSE__

#include <sstream>

class HttpResponse {
private:
    static const uint BUFF_SIZE = 4096;

    int conn;
    bool _open;
    char buf[BUFF_SIZE];
    uint buffered_bytes;
public:
    HttpResponse(int conn);

    bool open();

    HttpResponse & write(std::string value);

    HttpResponse & operator<<(std::string value);
    HttpResponse & operator<<(double value);
    HttpResponse & operator<<(uintmax_t value);

    void end();
};

#endif  // __RESPONSE__
