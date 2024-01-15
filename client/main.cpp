#include <QApplication>
#include <QThread>
#include "ClientHandler.h"
#include "mainwindow.h"
#include "ClientHandler.h"

using namespace std;





int main(int argc, char *argv[]) {

    QApplication a(argc, argv);
    ClientHandler client;

    MainWindow w(nullptr, &client);
    client.initConnection();

    w.show();

    const int result = a.exec();




    return result;
}
