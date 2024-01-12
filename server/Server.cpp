#include "Server.h"

#include <iostream>
#include <vector>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <random>

Server::Server() : deconectare(false) {
    // Create the server socket, setsockopt, bind, and listen
    // ...

    // Initialize other parameters if needed
}

Server::~Server() {
    // Cleanup resources, e.g., close sockets
    // ...
}

char *Server::conv_addr(sockaddr_in address) {
    static char str[25];
    char port[7];

    /* adresa IP a clientului */
    strcpy(str, inet_ntoa(address.sin_addr));
    /* portul utilizat de client */
    bzero(port, 7);
    sprintf(port, ":%d", ntohs(address.sin_port));
    strcat(str, port);
    return (str);
}

void Server::CreateSocket() {

    /* creare socket */
    if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("[server] Eroare la socket().\n");
    }

    /*setam pentru socket optiunea SO_REUSEADDR */
    setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

    /* pregatim structurile de date */
    bzero(&server, sizeof(server));

    /* umplem structura folosita de server */
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(PORT);

    /* atasam socketul */
    if (bind(sd, (struct sockaddr *) &server, sizeof(struct sockaddr)) == -1) {
        perror("[server] Eroare la bind().\n");
    }

    /* punem serverul sa asculte daca vin clienti sa se conecteze */
    if (listen(sd, 5) == -1) {
        perror("[server] Eroare la listen().\n");
    }

    /* completam multimea de descriptori de citire */
    FD_ZERO(&actfds);        /* initial, multimea este vida */
    FD_SET(sd, &actfds);        /* includem in multime socketul creat */

    tv.tv_sec = 1;        /* se va astepta un timp de 1 sec. */
    tv.tv_usec = 0;

    /* valoarea maxima a descriptorilor folositi */
    nfds = sd;

    printf("[server] Asteptam la portul %d... \n", PORT);
    fflush(stdout);

}

void Server::startListening() {
    CreateSocket();

    while (true) {
        bcopy((char *) &actfds, (char *) &readfds, sizeof(readfds));


        if (select(nfds + 1, &readfds, nullptr, nullptr, &tv) < 0) {
            perror("[server] Eroare la select().\n");
            return;
        }

        if (FD_ISSET(sd, &readfds)) {
            len = sizeof(from);
            bzero(&from, sizeof(from));

            client = accept(sd, (struct sockaddr *) &from, &len);

            if (client < 0) {
                perror("[server] Eroare la accept().\n");
                continue;
            }

            if (nfds < client)
                nfds = client;

            FD_SET(client, &actfds);

            cout << "[server] S-a conectat clientul cu descriptorul " << client
                 << ", de la adresa " << conv_addr(from) << ".\n";
            fflush(stdout);
        }

        for (fd = 0; fd <= nfds; fd++) {
            /* este un socket de citire pregatit? */
            if (fd != sd && FD_ISSET(fd, &readfds)) {
                char buffer[100];
                char msg[100];
                bool check;

                if (bytes < 0) {
                    perror("Eroare la read() de la client.\n");
                } else {
                    ParseCommand(fd);
                }

                if (deconectare) {
                    cout << "[server] Clientul cu descriptorul " << fd << " s-a deconectat \n";
                    fflush(stdout);
                    close(fd);
                    FD_CLR(fd, &actfds);
                    deconectare = false;
                }
            }
        }
    }
}


void Server::ExitClient() {
    deconectare = true;
    if ((write(fd, "Serverul a primit cererea de disconnect", 39)) < 0) {
        perror("[server] Eroare la write() catre client.\n");
    }

}

bool Server::Qcontains(int fd) {
    if (find(queue.begin(), queue.end(), fd) != queue.end())
        return true;
    else return false;
}

bool Server::Pcontains(int fd) {
    for (pair<int, int> p: perechi)
        if (fd == p.first || fd == p.second)
            return true;
    return false;
}

pair<int, int> Server::Preturn(int fd) {
    for (pair<int, int> p: perechi)
        if (fd == p.first || fd == p.second)
            return p;
    cout << "[server]Preturn nu a gasit pereche";
    return make_pair(0, 0);

}

int Server::FindPartida(int fd) {
    pair<int, int> p = Preturn(fd);
    for (int i = 0; i < nrpartide; i++)
        if (partide[i].p == p)
            return i;
    return -1;
}

void Server::Qerase(vector<int> queue, int fd) {
    queue.erase(remove(queue.begin(), queue.end(), fd), queue.end());
}

void Server::Perase(vector<pair<int, int>> perechi, pair<int, int> p) {
    perechi.erase(remove(perechi.begin(), perechi.end(), p), perechi.end());
}


int Server::Suitable(int fd) {
    if (queue.empty()) {
        queue.push_back(fd);
        return -1;
    } else if (Qcontains(fd))
        return -1;
    else return queue.front();

}

const vector<pair<int, int>> &Server::getPerechi() const {
    return perechi;
}

void Server::setPerechi(const vector<pair<int, int>> &perechi) {
    Server::perechi = perechi;
}

bool checkcheck(int x, int y, Piece *board[8][8], bool color) {

    int diri[4] = {1, 1, -1, -1};
    int dirj[4] = {-1, 1, 1, -1};


    for (int t = 0; t < 4; t++) {
        int k = 1;

        while (board[x + k * diri[t]][y + k * dirj[t]] == NULL && (x + k * diri[t] < 7 && x + k * diri[t] > 0) &&
               (y + k * dirj[t] < 7 && y + k * dirj[t] > 0))
            k++;
        if ((x + k * diri[t] < 7 && x + k * diri[t] > 0) && (y + k * dirj[t] < 7 && y + k * dirj[t] > 0))
            if (board[x + k * diri[t]][y + k * dirj[t]]->getColor() != color)
                if (strcmp(typeid(board[x + k * diri[t]][y + k * dirj[t]]).name(), "Bishop") ||
                    strcmp(typeid(board[x + k * diri[t]][y + k * dirj[t]]).name(), "Queen"))
                    return false;
    }


    return true;
}

void BoardFill(Piece *board[][8]) {
    for (int row = 0; row < 8; row++) {
        board[1][row] = new Pawn(false);
        board[6][row] = new Pawn(true);

    }

    board[0][0] = new Rook(false);
    board[0][7] = new Rook(false);
    board[7][7] = new Rook(true);
    board[7][0] = new Rook(true);

    board[0][1] = new Knight(false);
    board[0][6] = new Knight(false);
    board[7][1] = new Knight(true);
    board[7][6] = new Knight(true);

    board[0][2] = new Bishop(false);
    board[0][5] = new Bishop(false);
    board[7][5] = new Bishop(true);
    board[7][2] = new Bishop(true);

    board[0][4] = new King(false);
    board[7][4] = new King(true);

    board[0][3] = new Queen(false);
    board[7][3] = new Queen(true);


}

void Server::SendTurn(int indexpartida){
    if(partide[indexpartida].first){
        write(partide[indexpartida].p.first,"turn",4);
        write(partide[indexpartida].p.second,"asteapta",8);
        partide[indexpartida].first = false;
    }
    else {
        write(partide[indexpartida].p.second,"turn",4);
        write(partide[indexpartida].p.first,"asteapta",8);
        partide[indexpartida].first = true;

    }



}

void Server::Play(int fd) {
    if ((check = Suitable(fd)) == -1) {
        //printf("Asteapta un alt jucator \n");
        if ((write(fd, "Asteapta un alt jucator", 23)) < 0) {
            perror("[server] Eroare la write() catre client.\n");
        }


    } else {
        pair<int, int> pereche;
        Qerase(queue, check);
        pereche.first = check;
        pereche.second = fd;
        BoardFill(partide[nrpartide].board);
        partide[nrpartide].p = pereche;
        nrpartide++;
        perechi.push_back(pereche);
        printf("[server]Am imperecheat %d cu %d \n", check, fd);
        sprintf(msgrasp, "Vei juca cu %d", check);
        if ((write(fd, msgrasp, sizeof(msgrasp))) < 0) {
            perror("[server] Eroare la write() catre client.\n");
        }
        char temp[100] = " ";
        sprintf(temp, "%d\n", fd);
        write(check, temp, 1);

        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<int> distribution(1, 2);
        int color = distribution(gen);
        if (color == 1) {
            write(check, "white", 5);
            write(fd, "black", 5);
            partide[nrpartide].first = true;
        } else {
            write(check, "black", 5);
            write(fd, "white", 5);
            partide[nrpartide].first = false;
        }
        //SendTurn(nrpartide);



    }

}

Pos Server::MoveConverter(char movebuf[]) {
    Pos move;
    move.fromX = movebuf[0] - '0';
    move.fromY = movebuf[1] - '0';
    move.toX = movebuf[2] - '0';
    move.toY = movebuf[3] - '0';
    return move;
}

void Server::ParseCommand(int fd) {
    char buffer[100];
    char msg[100];

    if (read(fd, msg, sizeof(buffer)) < 0) {
        printf("[server] Eroare la read");
    };
    printf("[server]Am primit de clientul %d mesajul: %s \n", fd, msg);
    //printf("%d",strcmp(msg,"exit"));
    if (strcmp(msg, "exit") == 0) {
        //printf("meow");
        ExitClient();
        printf("[server]Clientul cu descriptorul %d vrea sa se deconecteze \n", fd);
    } else if (Pcontains(fd)) {
        if (strcmp(msg, "miscare") == 0) {
            if (read(fd, msg, sizeof(buffer)) < 0) { //al doilea read pentru "miscare"
                printf("[server] Eroare la read");
            };
            printf("Primul print: %s\n", msg);
            move = MoveConverter(msg);
            printf("A doilea: %d\n", move.fromX);


            bool isvalid = false;
            int match = FindPartida(fd);
            cout << "nr partida: " << match << "\n";

            if (partide[match].board[move.fromX][move.fromY] != NULL)
                isvalid = partide[match].board[move.fromX][move.fromY]->checkMove(move.fromX, move.fromY,
                                                                                  move.toX, move.toY,
                                                                                  partide[match].board);


            cout << "isvalid: " << isvalid << "\n";


            if (isvalid) {
                partide[match].board[move.toX][move.toY] = partide[match].board[move.fromX][move.fromY]; // actualizare mutare
                partide[match].board[move.fromX][move.fromY] = NULL; //golire pozitie precedenta

                write(fd, "valid", 5);

            } else write(fd, "invalid", 7);

            for (int i = 0; i < 8; i++) {
                cout << "\n";
                for (int j = 0; j < 8; j++)
                    if (partide[match].board[i][j] == NULL)
                        cout << "NULL    ";
                    else cout << typeid(*partide[match].board[i][j]).name() << "  ";

            }
            cout << endl;
            //SendTurn(match);


        }

    } else if (strcmp(msg, "play") == 0) {
        printf("[server]Clientul cu descriptorul %d a apasat comanda Play \n", fd);
        Play(fd);
    } else {
        if ((write(fd, "Comanda invalida!", 17)) < 0) {
            perror("[server] Eroare la write() catre client.\n");
        }

    }


}

void Server::sayHello(int fd) {
    char buffer[100];
    char msg[100];
    char msgrasp[100] = " ";

    bytes = read(fd, msg, sizeof(buffer));
    if (bytes < 0) {
        perror("Eroare la read() de la client.\n");
        return;
    }

    cout << "[server] Mesajul a fost receptionat..." << msg << "\n";

    bzero(msgrasp, 100);
    strcat(msgrasp, "Hello ");
    strcat(msgrasp, msg);

    cout << "[server] Trimitem mesajul inapoi..." << msgrasp << "\n";

    if (bytes && write(fd, msgrasp, bytes) < 0) {
        perror("[server] Eroare 3 la write() catre client.\n");
    }
}

