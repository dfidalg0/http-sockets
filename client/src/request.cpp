#include <string.h>
#include <netdb.h>
#include <sstream>
#include <sys/socket.h>
#include <unistd.h>

#include <iostream>

#include "request.h"
#include "utils.h"
#include "http.h"

HttpResponse * request(const char * url) {
    auto addr = parse_url(url);

    if (!addr) {
        return nullptr;
    }

    auto host = gethostbyname(addr->host);

    sockaddr_in channel = {
        .sin_family = AF_INET,
        .sin_port = htons(addr->port)
    };

    memcpy(
        &channel.sin_addr.s_addr,
        host->h_addr,
        host->h_length
    );

    auto sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    connect(sock, (sockaddr *) &channel, sizeof(channel));

    std::stringstream stream;

    stream
        << "GET "   << addr->path << " HTTP/1.1\r\n"
        << "Host: " << addr->host << "\r\n"
        << "\r\n";

    auto msg = stream.str();

    write(sock, msg.data(), msg.length());

    return new HttpResponse(sock, addr->path);
}
