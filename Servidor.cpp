#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <bits/stdc++.h>

#define LIMITE_MENSAGEM 11

using namespace std;


int main(void) {

    int fd_servidor, fd_cliente;  //? fd_cliente será um array?
    sockaddr_in endereco_servidor, endereco_cliente;
    socklen_t tamanho_endereco;

    // Criação de buffer para as mensagens:
    char mensagem[LIMITE_MENSAGEM];

    // Criação do socket do servidor:
    fd_servidor = socket(AF_INET, SOCK_STREAM, 0);

    // Verificação se o socket foi devidamente criado:
    if (fd_servidor == -1) {
        cerr << "Erro ao criar o socket!\n";
        exit(-1);
    }
    cout << "Socket criado com sucesso!\n";

    // Configuração do endereço do servidor:
    endereco_servidor.sin_family = AF_INET;
    endereco_servidor.sin_port = htons(2000);
    endereco_servidor.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Atribuição do endereço ao socket:
    if (bind(fd_servidor, (sockaddr *) &endereco_servidor, sizeof(endereco_servidor)) == -1) {
        cerr << "Erro ao realizar bind do socket!\n";
        exit(-1);
    } else {
        cout << "Bind realizado com sucesso!\n";
    }

    // Marcação do socket para a escuta de um único cliente:
    if (listen(fd_servidor, 1) == -1) {
        cerr << "Erro na marcação de escuta por clientes!\n";
        exit(-1);
    } else {
        cout << "Servidor escutando por requisições!\n";
    }

    // Inicialização de variável de controle de estado do servidor:
    bool shut_down = false;

    while(!shut_down) {

        // Aceite de uma requisição:
        tamanho_endereco = sizeof(endereco_cliente);
        fd_cliente = accept(fd_servidor, (sockaddr*) &endereco_cliente, &tamanho_endereco);

        // Verificação se o aceite foi válido:
        if (fd_cliente < 0){
            cerr << "Não foi possível se conectar ao cliente!\n";
            exit(-1);
        } 
        cout << "Cliente conectado no IP: " << inet_ntoa(endereco_cliente.sin_addr) << " e porta " << ntohs(endereco_cliente.sin_port) << endl;

        // Recepção da(s) mensagem(s) do cliente (pois podem ser quebradas se forem maiores que LIMITE_MENSAGEM):
        while(true){

            bzero(mensagem, LIMITE_MENSAGEM);

            // Recebimento de 1 bloco da mensagem do cliente:
            if (recv(fd_cliente, mensagem, sizeof(mensagem) - 1, 0) == -1) {
                cout << "Mensagem não recebida!\n";
                exit(-1);
            }

            if (strcmp(mensagem, "/quit") == 0) {
                shut_down = true;
                break;
            }
            
            if (strlen(mensagem) == 0)
                break;

            printf("%s\n", mensagem);
            
            bzero(mensagem, LIMITE_MENSAGEM);
        }
    }

    // Fechamento do socket:
    close(fd_servidor);

    return 0;
}