#Lab1 - CES-35
Alunos:
Diego Teixeira Nogueira Fidalgo
Pedro Henrique Freitas Silva

Estão inclusas as pastas server, client e bin que contém:

server:
-http-request.cpp/.h
-http-response.cpp/.h
-server.cpp/.h
-bad-request.h
-main.cpp

client:
-http.cpp/.h
-request.cpp/.h
-utils.cpp/.h
-main.cpp

bin:
-Contém os executáveis de client and server

Se for necessário compilar os arquivos, basta usar o comando 
```
$ make 
```
em uma pasta que contém todos os arquivos.

Para executar o server, basta usar o comando
```
$ ./bin/server [host] [port] [dir]
```

Para executar o client, basta usar o comando
```
$ ./bin/server [URL]
```

