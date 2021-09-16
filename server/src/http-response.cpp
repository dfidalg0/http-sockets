#include <http-response.h>
#include <sys/unistd.h>

HttpResponse::HttpResponse(int conn) {
    this->conn = conn;
}

HttpResponse & HttpResponse::write (std::string value) {
    ::write(this->conn, value.c_str(), value.length());
    return *this;
}
