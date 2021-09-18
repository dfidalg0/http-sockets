#include <sys/unistd.h>
#include <string.h>
#include <algorithm>
#include <sstream>
#include <iostream>
#include <regex>

#include <http-request.h>
#include <bad-request.h>

enum ReadStatus {
    FIRST_LINE,
    HEADERS,
    BODY,
};

HttpRequest::HttpRequest(int conn) {
    // Inicialização do objeto HttpRequest
    this->conn = conn;
    this->content_length = 0;
    this->bytes_read = 0;
    this->bytes_buffered = 0;

    // Estado atual na máquina de estados de leitura da resposta
    ReadStatus read_status = FIRST_LINE;

    // Variáveis auxiliares para percorrer o buffer de leitura
    unsigned i, j, n_bytes;
    std::smatch results;

    // stream de leitura para as linhas da resposta
    std::stringstream read_stream;

    // Loop externo - consome o socket e guarda a leitura no buffer
    while (read_status != BODY) {
        // Inicialização de i e j para o início do buffer
        i = j = 0;

        // Leitura do socket para o buffer de leitura
        n_bytes = ::read(conn, buffer, BUFF_SIZE);

        // Teste de fim de leitura
        if (n_bytes <= 0) break;

        // Loop interno - processa cada linha da resposta individualmente
        while (read_status != BODY) {
            // Loop para atingir o fim da linha atual (ou do buffer)
            while (buffer[j] != '\n' && j < n_bytes) ++j;

            // Teste para checar se chegou ao fim do buffer
            if (j == n_bytes) {
                // No caso de fim do buffer, gravamos, na stream de leitura,
                // os caracteres lidos ANTES do fim do buffer, posição na qual
                // há um '\0' para nos atormentar durante a noite
                read_stream.write(buffer + i, j - i);

                // E o loop externo deve ser executado novamente
                // para obter mais caracteres da linha atual
                break;
            }

            // Se o buffer não foi finalizado, a posição j corresponde ao \n
            // e, portanto, podemos gravar todos os caracteres na stream de
            // leitura
            read_stream.write(buffer + i, j - i + 1);

            // E então extrair a string gravada nessa stream como a linha
            // atual sendo processada
            std::string line = read_stream.str();

            if (read_status == FIRST_LINE) {
                // No caso de ser a primeira linha, devemos extrair o método,
                // o path e a versão do HTTP

                // Checagem se a primeira linha está no formado esperado
                if(!std::regex_match(line, results, std::regex("^([A-Z]+) ((?:\\/[^\\/]+)+\\/?) HTTP/(\\d+(\\.\\d+)?)\\r\\n$")))
                    throw BadRequestException();

                // Se o formato estiver correto, salvar os grupos de busca nas variáveis
                _method = results[1];
                _path = results[2];
                _version = std::stod(results[3]);

                // E avançar para o próximo estado
                read_status = HEADERS;
            }
            else if (line[0] != '\r') {
                // Se chegamos aqui, o estado com certeza é HEADERS e a linha
                // não começa com um '\r', portanto, estamos lendo um header

                // Checagem se as linhas de header estão no formato esperado
                if(!std::regex_match(line, results, std::regex("^([A-Za-z-]+): +([^\\r\\n]+)\\r\\n$")))
                    throw BadRequestException();

                // Se o formato estiver correto, salvar os grupos de busca nas variáveis
                std::string name = results[1];
                std::string value = results[2];

                // No entanto, é importante normalizar o nome do header,
                // pois estes são case-insensitive
                std::transform(
                    name.begin(), name.end(),  // limites da transformação
                    name.begin(),              // onde o resultado será salvo
                    tolower                    // função de transformação
                );

                // E, assim, podemos salvar o header no mapa de headers
                // da resposta
                headers[name] = value;

                // Além disso, caso o header seja o Content-Length, já podemos
                // salvá-lo como número no content_length
                if (name == "content-length") {
                    content_length = std::stoi(value);
                }
            }
            else {
                // Se chegamos aqui, a linha começa no caractere '\r', o que
                // significa que chegamos ao fim da leitura dos headers
                read_status = BODY;
            }

            // Após o término do processamento da linha, devemos esvaziar
            // a stream de leitura
            read_stream.str(std::string());

            // Além de atualizar as posições i e j para o início da próxima
            // linha tal como está armazenada no buffer
            i = ++j;
        }
    }

    // Ao finalizar a leitura, certamente devemos ter algum pedaço do corpo da
    // resposta armazenado no buffer. Tratemos de alocar esse valor no início
    strcpy(buffer, buffer + i);

    // E registrar o número de bytes do corpo que já está armazenado no buffer
    bytes_buffered = n_bytes - i;
}

std::string HttpRequest::path() {
    return _path;
}

std::string HttpRequest::method() {
    return _method;
}

double HttpRequest::version() {
    return _version;
}

std::string HttpRequest::header(std::string __name) {
    // Como o nome do header é case-insensitive, devemos fazer algumas
    // transformações

    // Primeiro, criamos uma string auxiliar para guardar o nome
    // transformado sem modificar a string original
    std::string name;
    name.resize(__name.length());

    // Depois transformamos a string original
    std::transform(
        __name.begin(), __name.end(),  // limites da transformação
        name.begin(),                  // onde o resultado será salvo
        tolower                        // função de transformação
    );

    // E buscamos o header no mapa de headers
    auto p = headers.find(name);

    if (p == headers.end()) {
        // Se o header não existe, retornamos uma string vazia
        return std::string();
    }

    // Caso exista, retornamos o seu valor
    return p->second;
}

int HttpRequest::read(std::ostream& stream) {
    // Verificação de fim da leitura
    if (bytes_read >= content_length) return -1;

    // Verificação da necessidade de ler diretamente do socket
    if (bytes_read + bytes_buffered < content_length) {
        // Leitura de bytes para completar o buffer
        int n_bytes = ::read(
            conn,
            buffer + bytes_buffered,    // Posição inicial para a leitura
            BUFF_SIZE - bytes_buffered  // No de bytes para completar o buffer
        );

        // Atualização do número de bytes buferizado
        bytes_buffered += n_bytes;
    }

    // Transmissão dos bytes lidos para a stream de saída
    stream.write(buffer, bytes_buffered);

    // Atualização do número total de bytes lidos
    bytes_read += bytes_buffered;

    // Número de bytes que foram transimitidos nessa leitura
    int bytes_streamed = bytes_buffered;

    // Atualização do número de bytes buferizados (o buffer já foi transmitido)
    bytes_buffered = 0;

    // Retorno do número de bytes transimitido
    return bytes_streamed;
}
