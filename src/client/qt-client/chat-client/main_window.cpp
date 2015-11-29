#include "ui_main_window.h"

#include "main_window.hpp"
#include "login_dialog.hpp"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->actionConnect, SIGNAL(triggered()),
            SLOT(action_connect_triggered()));

    connect(ui->actionDisconnect, SIGNAL(triggered()),
            SLOT(action_disconnect_triggered()));

    qRegisterMetaType<QAbstractSocket::SocketError>();
}

void MainWindow::action_connect_triggered() {
    auto login_dialog = new LoginDialog(this);
    connect(login_dialog, SIGNAL(recieved_credentials(const std::string&,
                                                      const std::string&, int)),
            SLOT(establish_connection(const std::string&,
                                      const std::string&, int)));

    login_dialog->setModal(true);
    login_dialog->show();
}

void MainWindow::establish_connection(const std::string& nickname,
                                      const std::string& ip, int port)
{

    remote_server = new Server(nickname, ip, port);

    connect(remote_server, SIGNAL(connected()), SLOT(connection_established()));
    connect(remote_server, SIGNAL(error()), SLOT(connection_failed()));

    auto thread = new QThread(this);
    remote_server->moveToThread(thread);

    connect(thread, SIGNAL(started()), remote_server, SLOT(establish_connection()));
    connect(remote_server, SIGNAL(stop_thread()), thread, SLOT(quit()));

    thread->start();
}

void MainWindow::connection_failed() {
    delete remote_server;
    remote_server = nullptr;
    qDebug() << "Connection failed";
}

void MainWindow::action_disconnect_triggered() {
    if (remote_server == nullptr) {
        return;
    }
    remote_server->disconnect();
    remote_server = nullptr;
    qDebug() << "Disconnected from server";
}

void MainWindow::connection_established() {
    qDebug() << "Connected successfully";
}


MainWindow::~MainWindow() {
    delete remote_server;
    delete ui;
}
