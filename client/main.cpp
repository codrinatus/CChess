#include <iostream>
#include <QApplication>
#include <QThread>
#include "ClientHandler.h"
#include "mainwindow.h"
#include "ClientHandler.h"

using namespace std;

class ClientThread : public QThread {
public:
    explicit ClientThread(ClientHandler* client) : client(client) {}

protected:
    void run() override {
        client->run();
    }

private:
    ClientHandler* client;

};



int main(int argc, char *argv[]) {

    QApplication a(argc, argv);
    ClientHandler client;
    ClientThread clientThread(&client);


    MainWindow w(nullptr, &client);
    client.initConnection();
    //clientThread.start();

    // Show the MainWindow
    w.show();


    //client.run();

    // Run the application event loop
    const int result = a.exec();


    //clientThread.wait();


    return result;
}
