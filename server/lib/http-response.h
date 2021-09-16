#ifndef __RESPONSE__
#define __RESPONSE__

#include <sstream>

class HttpResponse {
private:
    int conn;
public:
    HttpResponse(int conn);

    HttpResponse & write(std::string value);
};

#endif  // __RESPONSE__
