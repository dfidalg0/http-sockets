#ifndef __RESPONSE__
#define __RESPONSE__

#include <sstream>

class HttpResponse {
private:
    static const uint BUFF_SIZE = 4096;

    int conn;
    bool _open;
    char buf[BUFF_SIZE];
    uint buffered_bytes;
public:
    /**
     * Construtor da classe HttpResponse
     *
     * @param conn ID do descritor de arquivo da conexão TCP
     */
    HttpResponse(int conn);

    /**
     * @returns Booleano indicando se a conexão está aberta
     */
    bool open();

    /**
     * Escreve um texto na resposta da solicitação
     *
     * @param value Valor a ser escrito
     *
     * @returns Referência para o objeto resposta original
     */
    HttpResponse & write(std::string value);

    // Auxiliares para o método write
    HttpResponse & operator<<(std::string value);
    HttpResponse & operator<<(double value);
    HttpResponse & operator<<(uintmax_t value);

    /**
     * Encerra a resposta HTTP
     */
    void end();
};

#endif  // __RESPONSE__
