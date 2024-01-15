#include "Server.h"

#include <iostream>
#include <vector>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <random>

Server::Server() : deconectare(false) {
}

Server::~Server() {

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

void Server::start() {
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

void Server::Qerase(int f) {
    queue.erase(remove(queue.begin(), queue.end(), f), queue.end());
    cout << "am sters queue: " << queue[0] << endl;
}

void Server::Perase(pair<int, int> p) {
    perechi.erase(remove(perechi.begin(), perechi.end(), p), perechi.end());
}


int Server::Suitable(int fd) {
    if (queue.empty()) {
        cout << "not else" << endl;
        queue.push_back(fd);
        return -1;
    } else if (Qcontains(fd)) {
        cout << "else 1" << endl;
        return -1;

    } else {
        cout << "else 2" << endl;
        return queue.front();
    }

}

const vector<pair<int, int>> &Server::getPerechi() const {
    return perechi;
}

void Server::setPerechi(const vector<pair<int, int>> &perechi) {
    Server::perechi = perechi;
}

bool checkcheck(int x, int y, Piece *board[8][8], bool color) {

    //4Rook
    //6Knight
    //6Bishop
    //5Queen
    //4King
    //4Pawn

    for (int l = y - 1; l >= 0; l--)
        if (board[x][l] != NULL) {
            if (board[x][l]->getColor() != board[x][y]->getColor()) {
                cout << "piesa din stanga: " << typeid(*board[x][l]).name() << endl;
                if ((strcmp(typeid(*board[x][l]).name(), "4Rook") == 0) ||
                    (strcmp(typeid(*board[x][l]).name(), "5Queen") == 0))
                    return true;
            }
            break;
        }

    for (int l = y + 1; l <= 7; l++)
        if (board[x][l] != NULL) {
            if (board[x][l]->getColor() != board[x][y]->getColor()) {
                cout << "piesa din stanga: " << typeid(*board[x][l]).name() << endl;
                if ((strcmp(typeid(*board[x][l]).name(), "4Rook") == 0) ||
                    (strcmp(typeid(*board[x][l]).name(), "5Queen") == 0))
                    return true;
            }
            break;
        }

    for (int l = x - 1; l >= 0; l--)
        if (board[l][y] != NULL) {
            if (board[l][y]->getColor() != board[x][y]->getColor()) {
                cout << "piesa din stanga: " << typeid(*board[l][y]).name() << endl;
                if ((strcmp(typeid(*board[l][y]).name(), "4Rook") == 0) ||
                    (strcmp(typeid(*board[l][y]).name(), "5Queen") == 0))
                    return true;
            }
            break;
        }

    for (int l = x + 1; l <= 7; l++)
        if (board[l][y] != NULL) {
            if (board[l][y]->getColor() != board[x][y]->getColor()) {
                cout << "piesa din stanga: " << typeid(*board[l][y]).name() << endl;
                if ((strcmp(typeid(*board[l][y]).name(), "4Rook") == 0) ||
                    (strcmp(typeid(*board[l][y]).name(), "5Queen") == 0))
                    return true;
            }
            break;
        }

    for (int l = x - 1, m = y - 1; l >= 0 && m >= 0; l--, m--) {
        if (board[l][m] != NULL) {
            if (board[l][m]->getColor() != board[x][y]->getColor()) {
                cout << "piesa diagonala stanga sus: " << typeid(*board[l][m]).name() << endl;
                if ((strcmp(typeid(*board[l][m]).name(), "6Bishop") == 0) ||
                    (strcmp(typeid(*board[l][m]).name(), "5Queen") == 0))
                    return true;
            }
            break;
        }
    }

    for (int l = x - 1, m = y + 1; l >= 0 && m <= 7; l--, m++) {
        if (board[l][m] != NULL) {
            if (board[l][m]->getColor() != board[x][y]->getColor()) {
                cout << "piesa diagonala dreapta sus: " << typeid(*board[l][m]).name() << endl;
                if ((strcmp(typeid(*board[l][m]).name(), "6Bishop") == 0) ||
                    (strcmp(typeid(*board[l][m]).name(), "5Queen") == 0))
                    return true;
            }
            break;
        }
    }

    for (int l = x + 1, m = y - 1; l <= 7 && m >= 0; l++, m--) {
        if (board[l][m] != NULL) {
            if (board[l][m]->getColor() != board[x][y]->getColor()) {
                cout << "piesa diagonala stanga jos: " << typeid(*board[l][m]).name() << endl;
                if ((strcmp(typeid(*board[l][m]).name(), "6Bishop") == 0) ||
                    (strcmp(typeid(*board[l][m]).name(), "5Queen") == 0))
                    return true;
            }
            break;
        }
    }

    for (int l = x + 1, m = y + 1; l <= 7 && m <= 7; l++, m++) {
        if (board[l][m] != NULL) {
            if (board[l][m]->getColor() != board[x][y]->getColor()) {
                cout << "piesa diagonala dreapta jos: " << typeid(*board[l][m]).name() << endl;
                if ((strcmp(typeid(*board[l][m]).name(), "6Bishop") == 0) ||
                    (strcmp(typeid(*board[l][m]).name(), "5Queen") == 0))
                    return true;
            }
            break;
        }
    }

    return false;

}

bool isCheckmate(int x, int y, Piece *board[8][8], bool color) {
    if (!checkcheck(x, y, board, color)) {
        return false;
    }

    for (int i = x - 1; i <= x + 1; i++) {
        for (int j = y - 1; j <= y + 1; j++) {
            if (i >= 0 && i <= 7 && j >= 0 && j <= 7) {
                if (board[i][j] == NULL || (board[i][j]->getColor() != color)) {
                    Piece *temp = board[i][j];
                    board[i][j] = board[x][y];
                    board[x][y] = NULL;

                    if (!checkcheck(i, j, board, color)) {
                        board[x][y] = board[i][j];
                        board[i][j] = temp;
                        return false;
                    }

                    board[x][y] = board[i][j];
                    board[i][j] = temp;
                }
            }
        }
    }

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if ((board[i][j] != NULL && board[i][j]->getColor() == color) && (i != x || j != y)) {
                for (int ii = 0; ii < 8; ii++)
                    for (int jj = 0; jj < 8; jj++)
                        if (i != ii || j != jj)
                            if (board[i][j]->checkMove(i, j, ii, jj, board)) {
                                Piece *temp = board[ii][jj];
                                board[ii][jj] = board[i][j];
                                board[i][j] = NULL;

                                if (!checkcheck(x, y, board, color)) {
                                    board[i][j] = board[ii][jj];
                                    board[ii][jj] = temp;
                                    cout << "checkmate block: " << typeid(*board[i][j]).name() << " ij:" << i << j
                                         << " iijj: " << ii << jj << endl;
                                    return false;
                                }

                                board[i][j] = board[ii][jj];
                                board[ii][jj] = temp;
                            }

            }
        }
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

void Server::SendTurn(int indexpartida) {
    cout << "before sendturn if: " << partide[indexpartida].p.first << endl;
    cout << "before sendturn if: " << partide[indexpartida].p.second << endl;
    if (partide[indexpartida].first) {
        write(partide[indexpartida].p.first, "turn", 4);
        write(partide[indexpartida].p.second, "asteapta", 8);
        partide[indexpartida].first = false;
    } else {
        write(partide[indexpartida].p.second, "turn", 4);
        write(partide[indexpartida].p.first, "asteapta", 8);
        partide[indexpartida].first = true;

    }
    cout << "after sendturn if" << endl;


}

void Server::Play(int fd) {
    if ((check = Suitable(fd)) == -1) {
        //printf("Asteapta un alt jucator \n");
        if ((write(fd, "Asteapta un alt jucator", 23)) < 0) {
            perror("[server] Eroare la write() catre client.\n");
        }


    } else {
        pair<int, int> pereche;
        Qerase(check);
        pereche.first = check;
        pereche.second = fd;
        BoardFill(partide[nrpartide].board);
        partide[nrpartide].p = pereche;
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
            partide[nrpartide].whiteturn = true;
            partide[nrpartide].regefirst.first = 7;
            partide[nrpartide].regefirst.second = 4;
            partide[nrpartide].regesecond.first = 0;
            partide[nrpartide].regesecond.second = 4;
        } else {
            write(check, "black", 5);
            write(fd, "white", 5);
            partide[nrpartide].first = false;
            partide[nrpartide].whiteturn = false;
            partide[nrpartide].regefirst.first = 0;
            partide[nrpartide].regefirst.second = 4;
            partide[nrpartide].regesecond.first = 7;
            partide[nrpartide].regesecond.second = 4;

        }
        cout << "before sendturn" << endl;
        SendTurn(nrpartide);
        nrpartide++;


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
        cout<<"pcontains if" << endl;
        if (strcmp(msg, "miscare") == 0) {
            if (read(fd, msg, sizeof(buffer)) < 0) { //al doilea read pentru "miscare"
                printf("[server] Eroare la read");
            };
            move = MoveConverter(msg);


            bool isvalid = false;
            int match = FindPartida(fd);

            if (partide[match].board[move.fromX][move.fromY] != NULL) {


                isvalid = partide[match].board[move.fromX][move.fromY]->checkMove(move.fromX, move.fromY,
                                                                                  move.toX, move.toY,
                                                                                  partide[match].board);


                cout << "isvalid: " << isvalid << "\n";

//            cout << "partide first: " << partide[match].first << endl;
//            cout << "partide p first ==fd : " << (partide[match].p.first == fd) << endl;
//            cout << "getcolor: " << partide[match].board[move.fromX][move.fromY]->getColor() << endl;

                if (!partide[match].first) { //first == 1 -> primul e negru
                    cout << "primul e alb" << endl;
                    if ((partide[match].p.first == fd) == partide[match].board[move.fromX][move.fromY]->getColor()) {
                        isvalidcolor = true;
                    } else isvalidcolor = false;
                } else {
                    if ((partide[match].p.first == fd) != partide[match].board[move.fromX][move.fromY]->getColor()) {
                        isvalidcolor = true;
                    } else isvalidcolor = false;
                }

                cout << "isvalidcolor: " << isvalidcolor << endl;

                bool ischeck;

                if (partide[match].board[move.fromX][move.fromY] != NULL) {
                    Piece *aux = partide[match].board[move.toX][move.toY];
                    partide[match].board[move.toX][move.toY] = partide[match].board[move.fromX][move.fromY];
                    partide[match].board[move.fromX][move.fromY] = NULL;
                    if (partide[match].p.first == fd) {
                        if (strcmp(typeid(*partide[match].board[move.toX][move.toY]).name(), "4King") == 0) {
                            if (checkcheck(move.toX, move.toY,
                                           partide[match].board,
                                           partide[match].board[move.toX][move.toY]->getColor())) {
                                ischeck = true;
                                cout << "ischeck true" << endl;
                            } else {
                                ischeck = false;
                                cout << "ischeck false" << endl;
                            }

                        } else if (checkcheck(partide[match].regefirst.first, partide[match].regefirst.second,
                                              partide[match].board,
                                              partide[match].board[partide[match].regefirst.first][partide[match].regefirst.second]->getColor())) {
                            cout << "ischeck true" << endl;
                            ischeck = true;
                        } else {
                            ischeck = false;
                            cout << "ischeck false" << endl;
                        }
                    } else {
                        if (strcmp(typeid(*partide[match].board[move.toX][move.toY]).name(), "4King") == 0) {
                            if (checkcheck(move.toX, move.toY,
                                           partide[match].board,
                                           partide[match].board[move.toX][move.toY]->getColor())) {
                                cout << "ischeck true" << endl;
                                ischeck = true;
                            } else {
                                ischeck = false;
                                cout << "ischeck false" << endl;
                            }
                        } else if (checkcheck(partide[match].regesecond.first, partide[match].regesecond.second,
                                              partide[match].board,
                                              partide[match].board[partide[match].regesecond.first][partide[match].regesecond.second]->getColor())) {
                            cout << "ischeck true" << endl;
                            ischeck = true;
                        } else {
                            ischeck = false;
                            cout << "ischeck false" << endl;
                        }
                    }
                    partide[match].board[move.fromX][move.fromY] = partide[match].board[move.toX][move.toY];
                    partide[match].board[move.toX][move.toY] = aux;

                }


                if (isvalid && isvalidcolor && (!ischeck)) {
                    partide[match].board[move.toX][move.toY] = partide[match].board[move.fromX][move.fromY]; // actualizare mutare
                    partide[match].board[move.fromX][move.fromY] = NULL; //golire pozitie precedenta
                    partide[match].whiteturn = !partide[match].whiteturn;
                    cout << "strcmp king: "
                         << (strcmp(typeid(*partide[match].board[move.toX][move.toY]).name(), "4King") == 0) << endl;
                    if (strcmp(typeid(*partide[match].board[move.toX][move.toY]).name(), "4King") ==
                        0) { //actualizare pozitie rege
                        cout << "se actualizeaza rege" << endl;
                        cout << "old regefirst first: " << partide[match].regefirst.first << " regefirst second: "
                             << partide[match].regefirst.second << endl;
                        cout << "old regesecond first: " << partide[match].regesecond.first << " regefirst second: "
                             << partide[match].regesecond.second << endl;
                        if (partide[match].p.first == fd) {
                            partide[match].regefirst.first = move.toX;
                            partide[match].regefirst.second = move.toY;

                        } else {
                            partide[match].regesecond.first = move.toX;
                            partide[match].regesecond.second = move.toY;
                        }
                        cout << "new regefirst first: " << partide[match].regefirst.first << " regefirst second: "
                             << partide[match].regefirst.second << endl;
                        cout << "new regesecond first: " << partide[match].regesecond.first << " regefirst second: "
                             << partide[match].regesecond.second << endl;
                    }

                    bool checkmate = false;

                    if (partide[match].p.first == fd) {
                        checkmate = isCheckmate(partide[match].regesecond.first, partide[match].regesecond.second,
                                                partide[match].board,
                                                !partide[match].board[move.toX][move.toY]->getColor());
                        cout << "checkmate!!" << endl;
                    } else {
                        checkmate = isCheckmate(partide[match].regefirst.first, partide[match].regefirst.second,
                                                partide[match].board,
                                                !partide[match].board[move.toX][move.toY]->getColor());
                        cout << "checkmate!!!" << endl;
                    }

                    if (checkmate) {
                        Perase(partide[match].p);

                        write(fd, "w", 1);
                        if (partide[match].p.first == fd) {
                            write(partide[match].p.second, "loss", 4);
                            //write(partide[match].p.second, "calm", 4);
                        } else {
                            write(partide[match].p.first, "loss", 4);
                            //write(partide[match].p.second, "calm", 4);

                        }
                        partide[match].p.first = -1;
                        partide[match].p.second = -1;


                    } else {
                        write(fd, "valid", 5);
                        if (partide[match].p.first == fd)
                            write(partide[match].p.second, msg, 4);
                        else write(partide[match].p.first, msg, 4);
                    }

                } else write(fd, "invalid", 7);


                for (int i = 0; i < 8; i++) {
                    cout << "\n";
                    for (int j = 0; j < 8; j++) {
                        if (partide[match].board[i][j] == NULL)
                            cout << "NULL    ";
                        else cout << typeid(*partide[match].board[i][j]).name() << "  ";
                    }
                }
                cout << endl;
            } else write(fd, "invalid", 7);


        }

    } else if (
            strcmp(msg,
                   "play") == 0) {
        printf("[server]Clientul cu descriptorul %d a apasat comanda Play \n", fd);
        Play(fd);
    } else {
        if ((
                    write(fd,
                          "Comanda invalida!", 17)) < 0) {
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

