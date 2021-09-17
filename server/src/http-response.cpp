#include <http-response.h>
#include <sys/unistd.h>
#include <iostream>

HttpResponse::HttpResponse(int conn):
    conn(conn),
    _open(true),
    buffered_bytes(0) {}

bool HttpResponse::open() {
    return _open;
}

HttpResponse & HttpResponse::write (std::string value) {
    const uint size = value.length();

    uint n_bytes;

    for (uint i = 0; i < size; i += n_bytes) {
        n_bytes = std::min(
            BUFF_SIZE - buffered_bytes,
            size - i
        );

        value.copy(buf + buffered_bytes, n_bytes, i);

        buffered_bytes += n_bytes;

        if (buffered_bytes >= BUFF_SIZE) {
            ::write(conn, buf, buffered_bytes);
            buffered_bytes = 0;
        }
    }

    return *this;
}

HttpResponse & HttpResponse::operator<<(std::string value) {
    return write(value);
}

HttpResponse& HttpResponse::operator<<(double value) {
    std::stringstream s;

    s << value;

    return write(s.str());
}

HttpResponse & HttpResponse::operator<<(uintmax_t value) {
    std::stringstream s;

    s << value;

    return write(s.str());
}

void HttpResponse::end() {
    if (!_open) {
        return;
    }

    if (buffered_bytes) {
        ::write(conn, buf, buffered_bytes);
        buffered_bytes = 0;
    }

    _open = false;
    ::close(conn);
}
