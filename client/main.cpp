#include <iostream>
#include <fstream>
#include "request.h"

using std::cout;
using std::endl;

int main (int argc, char const * argv[]) {
    if (argc != 2) {
        cout << "Usage: " << argv[0] << " [url]" << endl;
        return 1;
    }

    auto response = request(argv[1]);

    if (!response) {
        cout << "Invalid URL" << endl;
        return 2;
    }

    auto status = response->status();

    if (status >= 400) {
        cout << "Request failed with status " << status << endl;
        return 3;
    }

    auto path = response->path();

    auto filename = path.substr(path.rfind('/') + 1);

    if (filename.empty()) {
        filename = "index.html";
    }

    std::ofstream file(filename);

    while (response->read(file) > 0);

    file.close();

    cout << "Downloaded file " << filename << endl;

    return 0;
}
