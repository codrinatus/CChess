#ifndef CLIENT_CLIENTHANDLER_H
#define CLIENT_CLIENTHANDLER_H

#include <QApplication>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <iostream>

class UiMainWindow; // Forward declaration


class ClientHandler {
private:
    int sd;  // Socket descriptor
    int port = 2728;
    bool running;
    struct sockaddr_in server{};    // structura folosita pentru conectare



public:
    ClientHandler();

    ~ClientHandler();

    void run();

    void sendMessage(const char *msg);

    void receiveMessage(char msg[]);




private:
    int initConnection();



    void closeConnection();

};
#endif //CLIENT_CLIENTHANDLER_H
