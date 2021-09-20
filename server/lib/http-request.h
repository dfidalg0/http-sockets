#ifndef __REQUEST__
#define __REQUEST__

#include <unordered_map>
#include <string>

class HttpRequest {
private:
    static const uint BUFF_SIZE = 4096;

    int conn;
    int content_length;
    int bytes_read;
    int bytes_buffered;

    std::unordered_map<std::string, std::string> headers;
    std::string _path;
    std::string _method;
    double _version;

    char buffer[BUFF_SIZE];
public:
    /**
     * Construtor da classe HttpRequest
     *
     * @param conn ID do descritor de arquivo da conexão TCP
     */
    HttpRequest(int conn);

    /**
     * @returns Caminho solicitado na requisição
     */
    std::string path();

    /**
     * @returns Método da requisição HTTP
     */
    std::string method();

    /**
     * @returns Versão do protocólo HTTP utilizada pela requisição
     */
    double version();

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
    int read(std::ostream& stream);
};

#endif // __REQUEST__
