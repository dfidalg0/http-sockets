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

    // Execução da requisição
    auto response = request(argv[1]);

    if (!response) {
        cout << "Invalid URL" << endl;
        return 2;
    }

    auto status = response->status();

    // Checagem de erro na resposta
    if (status >= 400) {
        cout << "Request failed with status " << status << endl;
        return 3;
    }

    // Criação do arquivo de gravação da resposta
    auto path = response->path();

    auto filename = path.substr(path.rfind('/') + 1);

    if (filename.empty()) {
        filename = "index.html";
    }

    std::ofstream file(filename);

    // Leitura da resposta HTTP
    while (response->read(file) > 0);

    file.close();

    // Mensagem informativa do nome do arquivo baixado
    cout << "Downloaded file " << filename << endl;

    return 0;
}
