#ifndef CLIENT_CLIENTHANDLER_H
#define CLIENT_CLIENTHANDLER_H

#include <QApplication>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

class UiMainWindow;


class ClientHandler : public QObject {
    Q_OBJECT
private:
    int sd;  // socket descriptor
    int port = 2728;
    bool running;
    struct sockaddr_in server{};    // structura folosita pentru conectare




public:
    ClientHandler();

    ~ClientHandler();

    void run();

    void sendMessage(const char *msg);

    void receiveMessage(char msg[]);
    void receiveMessage(char msg[],int size);
    char msg[100];
    int initConnection();




signals:
    void messageReceived(const char* msg);




private:



    void closeConnection();

};
#endif //CLIENT_CLIENTHANDLER_H
