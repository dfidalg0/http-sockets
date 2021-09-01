#ifndef __HTTP__
#define __HTTP__

#include <sstream>
#include <unordered_map>

#define BUFF_SIZE 4096

class HttpResponse {
private:
    int _status;
    std::string _path;
    int content_length;
    int bytes_read;
    int bytes_buffered;
    char buffer[BUFF_SIZE];
    int sock;
    std::unordered_map<std::string, std::string> headers;

public:
    /**
     * Construtor da classe HttpResponse
     *
     * @param sock ID do socket de leitura
     * @param path Caminho do recurso solicitado
     */
    HttpResponse(int sock, std::string filename);

    /**
     * @returns Código do status HTTP da resposta
     */
    int status();

    /**
     * @returns Caminho do recurso solicitado
     */
    std::string path();

    /**
     * @param name Nome do header buscado
     * @returns Valor do header pedido ou string vazia caso este não exista
     */
    std::string header(std::string name);

    /**
     * Obtém uma parcela do corpo da requisição
     * @param stream Stream de escrita onde será salvo o resultado da leitura
     *
     * @returns Número de bytes transmitido para a stream ou -1 em caso de EOF
     */
    int read(std::ostream & stream);
};

#endif  // __HTTP__
