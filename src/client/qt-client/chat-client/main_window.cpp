#include "main_window.hpp"
#include "ui_main_window.h"
#include "login_dialog.hpp"
#include <QDebug>
#include <QString>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    connect(ui->actionConnect, SIGNAL(triggered()), SLOT(action_connect_triggered()));
    connect(ui->actionDisconnect, SIGNAL(triggered()), SLOT(action_disconnect_triggered()));
}

void MainWindow::action_connect_triggered() {
    auto login_dialog = new LoginDialog(this);
    connect(login_dialog, SIGNAL(recieved_credentials(QString&, QString&, int)),
            SLOT(establish_connection(QString&, QString&, int)));

    login_dialog->setModal(true);
    login_dialog->show();
}

void MainWindow::establish_connection(QString& nickname, QString& ip, int port) {
    remote_server = std::move(
                std::unique_ptr<Server>(new Server(nickname, ip, port)));

    connect(remote_server.get(), SIGNAL(socket_fails(QAbstractSocket::SocketError)),
            this, SLOT(connection_fails(QAbstractSocket::SocketError)));

    connect(remote_server.get(), SIGNAL(socket_connected()),
                this, SLOT(connection_established()));

    remote_server->establish_connection();
    statusBar()->showMessage("Trying connect to " +
                             remote_server->get_address(), 30000);
}

void MainWindow::connection_fails(QAbstractSocket::SocketError e) {
    QString error_msg = "Error: " +
        (e == QAbstractSocket::HostNotFoundError ?
        "The host was not found." :
         e == QAbstractSocket::RemoteHostClosedError ?
        "The remote host is closed." :
         e == QAbstractSocket::ConnectionRefusedError ?
        "The connection was refused." :
         QString(remote_server->get_client_socket()->errorString())
    );
    QMessageBox::critical(this, "Error", error_msg);
}

void MainWindow::action_disconnect_triggered() {
//    if (remote_server == nullptr) {
//        return;
//}

//    remote_server->disconnect();
//    auto msg = remote_server->get_address();
//    statusBar()->showMessage("Disconnected from " + msg, 2000);
//    remote_server = nullptr;
}

MainWindow::~MainWindow() {
    if (remote_server != nullptr) {
        remote_server->disconnect();
    }
    delete ui;
}

void MainWindow::connection_established() {
    statusBar()->showMessage("Connected to " +
                             remote_server->get_address(), 3000);
}
