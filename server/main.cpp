#include <iostream>
#include <experimental/filesystem>
#include <fstream>
#include <sys/fcntl.h>
#include <sys/unistd.h>

#include <server.h>

namespace fs = std::experimental::filesystem;

using std::cout;
using std::endl;

int main(int argc, char const * argv[]) {
    if (argc != 4) {
        cout << "Usage: " << argv[0] << " [host] [port] [dir]" << endl;
        return 1;
    }

    // Criação do caminho do diretório
    const fs::path path(argv[3]);

    // Verificação da existência do diretório
    if (!fs::is_directory(path)) {
        cout << argv[3] << ": is not a directory" << endl;
        return 1;
    }

    // Criação do servidor com a função de processamento de requisição
    auto server = HttpServer([&path](HttpRequest& req, HttpResponse& res) {
        // Criação do caminho completo para o arquivo;
        auto filepath = path / fs::path(req.path());

        // Verificação de erro 404
        // Nesse servidor específico, as únicas rotas existentes são GETs
        // para caminhos válidos de arquivo.
        if (req.method() != "GET" || !fs::is_regular_file(filepath)) {
            res
                << "HTTP/" << req.version() << " 404 Not Found\r\n"
                << "Content-Type: text/plain\r\n"
                << "\r\n"
                << "File not found\n";

            return;
        }

        // Determinação do tamanho do arquivo para preencher o header
        // Content-Length
        auto size = fs::file_size(filepath);

        // Criação do fd do arquivo para transmití-lo para a conexão TCP
        int fd = open(filepath.c_str(), O_RDONLY);

        // Envio dos headers de resposta
        res
            << "HTTP/" << req.version() << " 200 Ok\r\n"
            << "Content-length: " << size << "\r\n"
            << "\r\n";

        // Leitura do arquivo
        char buffer[4096];
        int n_bytes;

        while ((n_bytes = read(fd, buffer, 4096)) > 0) {
            // Transmissão do arquivo
            res << std::string(buffer, buffer + n_bytes);
        }
    });

    // Criação da thread de escuta do servidor
    auto * server_thread = server.listen(argv[1], atoi(argv[2]));

    // Checagem de sucesso na abertura do servidor
    if (!server_thread) {
        cout << "Error when trying to listen on " << argv[1] << ':' << argv[2] << endl;
        return 2;
    }

    // Mensagem informativa de inicialização
    std::cout << "Server started at " << argv[1] << ':' << argv[2] << endl;

    // Acoplamento da thread do servidor à linha de execução principal
    server_thread->join();

    return 0;
}
