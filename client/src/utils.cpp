#include <iostream>
#include <string.h>

#include "utils.h"

http_addr* parse_url(const char* __url) {
    unsigned short port = 80;
    char* host = nullptr;
    char* path = nullptr;

    const int len = strlen(__url);

    char* url = new char[len];
    strcpy(url, __url);

    unsigned i = 0;

    if (starts_with(url, "http://")) {
        port = 80;
        i = 7;
    } else if (starts_with(url, "https://")) {
        port = 443;
        i = 8;
    }

    host = url + i;

    while (url[i] != '/' && url[i] != '\0' && url[i] != ':') {
        ++i;
    }

    if (url[i] == ':') {
        url[i++] = '\0';

        sscanf(url + i, "%hu", &port);

        while (url[i] != '/' && url[i] != '\0') {
            if (url[i] < '0' || url[i] > '9') {
                return nullptr;
            }

            ++i;
        }
    }

    if (url[i] == '\0') {
        path = new char[2];
        strcpy(path, "/");
    } else {
        int len = strlen(url + i);
        path = new char[len];
        strcpy(path, url + i);
        path[len] = '\0';
        url[i] = '\0';
    }

    return new http_addr({
        .host = host,
        .port = port,
        .path = path,
    });
}

bool starts_with(const char* src, const char* test) {
    bool result = true;

    for (int i = 0; test[i] != '\0'; ++i) {
        if (src[i] != test[i]) {
            result = false;
            break;
        }
    }

    return result;
}
