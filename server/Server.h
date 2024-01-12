#ifndef SERVER_H
#define SERVER_H

#include <vector>
#include <string>
#include <csignal>
#include <netinet/in.h>
#include <algorithm>
#include "Piece.h"
#include "Rook.h"
#include "Knight.h"
#include "Bishop.h"
#include "King.h"
#include "Queen.h"
#include "Pawn.h"

#define PORT 2728
using namespace std;

struct Pos{
    int fromX,fromY,toX,toY;
};

struct Partide{
    pair<int,int> p;
    Piece* board[8][8];
    bool first;
};

class Server {
private:
    struct sockaddr_in server;    /* structurile pentru server si clienti */
    struct sockaddr_in from;
    fd_set readfds;        /* multimea descriptorilor de citire */
    fd_set actfds;        /* multimea descriptorilor activi */
    struct timeval tv;        /* structura de timp pentru select() */
    int sd,client;        /* descriptori de socket */
    int optval = 1;            /* optiune folosita pentru setsockopt()*/
    int fd;            /* descriptor folosit pentru
				   parcurgerea listelor de descriptori */
    int nfds;            /* numarul maxim de descriptori */
    socklen_t len;            /* lungimea structurii sockaddr_in */
    bool deconectare;
    int check;
    vector<int> queue;
    vector<pair<int,int>> perechi;
    char msgrasp[100] =" ";
    int bytes=0;
    Pos move;
    int nrpartide=0;
    Partide partide[1000];


public:
    Server();
    ~Server();
    void startListening();
    void ExitClient();
    void ParseCommand(int fd);
    void sayHello(int fd);
    void handleConnections(fd_set &actfds);
    static char * conv_addr(sockaddr_in address);
    void CreateSocket();
    void Play(int fd);
    int Suitable(int fd);

    bool Qcontains(int fd);

    void Qerase(vector<int> queue,int fd);

    void Perase(vector<pair<int, int>> perechi, pair<int, int> p);

    const vector<pair<int, int>> &getPerechi() const;

    void setPerechi(const vector<pair<int, int>> &perechi);

    bool Pcontains(int fd);

    Pos MoveConverter(char *movebuf);

    pair<int, int> Preturn(int fd);


    int FindPartida(int fd);

    void SendTurn(int indexpartida);
};

#endif  // SERVER_H
