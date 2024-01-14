#include "ClientHandler.h"


using namespace std;

ClientHandler::ClientHandler() : sd(-1), running(false) {

}

ClientHandler::~ClientHandler() {
    closeConnection();
}

void ClientHandler::run() {
    if (initConnection() == 0) {
        running = true;
        while (running) {
            //bzero(msg, 100);
            fflush(stdout);
            //read(0, msg, sizeof(msg));

            //sendMessage(msg);
            receiveMessage(msg);
            qDebug () << "read din while: " << msg;

            //cout << "[client]Mesajul primit este: " << msg << endl;
            if (strcmp(msg, "Serverul a primit cererea de disconnect") == 0)
                running = false;
        }
    }

    closeConnection();
}

int ClientHandler::initConnection() {
    /* cream socketul */
    if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("[client] Eroare la socket().\n");
        return errno;
    }


    /* umplem structura folosita pentru realizarea conexiunii cu serverul */
    /* familia socket-ului */
    server.sin_family = AF_INET;
    /* adresa IP a serverului */
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    /* portul de conectare */
    server.sin_port = htons(port);
    running = true;

    /* ne conectam la server */
    if (::connect(sd, (struct sockaddr *) &server, sizeof(struct sockaddr)) == -1) {
        perror("[client]Eroare la connect().\n");
        return errno;
    }
    return 0;
}

void ClientHandler::sendMessage(const char *msg) {
    if (running) {
        if (write(sd, msg, 100) <= 0) {
            perror("[client]Eroare la write() spre server.\n");
        }
    } else perror("No running client");
}

void ClientHandler::receiveMessage(char msg[]) {
    if(running) {
        strcpy(msg,"");
        bzero(msg,100);
        if (read(sd, msg, 100 ) < 0) {
        perror("[client]Eroare la read() de la server.\n");
        }
        else {
            //emit messageReceived(msg);
            printf("[client]Am primit de la server: %s \n",msg);
        }
    } else perror("No running client");
}

void ClientHandler::receiveMessage(char msg[],int size) {
    if(running) {

        strcpy(msg,"");
        bzero(msg,100);
        if (read(sd, msg, size ) < 0) {
            perror("[client]Eroare la read() de la server.\n");
        }
        else {
            //emit messageReceived(msg);
            printf("[client]Am primit de la server: %s \n",msg);
        }
    } else perror("No running client");
}

void ClientHandler::closeConnection() {
    if (sd != -1) {
        close(sd);
    }
}
