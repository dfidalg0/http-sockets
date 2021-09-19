# Lab1 - CES-35 - Sockets HTTP

- Diego Teixeira Nogueira Fidalgo
- Pedro Henrique Freitas Silva

## Estrutura de arquivos

Estão inclusas as pastas server e client que contém:

- server
  - http-request.cpp/.h
  - http-response.cpp/.h
  - server.cpp/.h
  - bad-request.h
  - main.cpp

- client
  - http.cpp/.h
  - request.cpp/.h
  - utils.cpp/.h
  - main.cpp

## Compilação

Para compilar os arquivos, basta usar o comando

```bash
$ make
```

ou, para compilar um dos executáveis individualmente,

```bash
$ make [client|server]
```

Todos os executáveis são salvos na pasta `bin` na raiz do projeto.

## Utilização

Para executar o servidor, basta usar o comando

```bash
$ ./bin/server [host] [port] [dir]
```

Para executar o cliente, basta usar o comando

```bash
$ ./bin/client [URL]
```

## Limpeza

Para apagar todos os arquivos compilados, basta usar o comando

```bash
$ make clean
```
