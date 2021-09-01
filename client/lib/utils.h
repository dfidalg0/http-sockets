#ifndef __UTILS__
#define __UTILS__

struct http_addr {
    const char* host;
    unsigned short port;
    const char* path;
};

http_addr* parse_url(const char* url);
bool starts_with(const char* src, const char* test);

#endif // __UTILS__
