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
    connect(login_dialog, SIGNAL(connected_to_server(Server*)),
            SLOT(establish_connection(Server*)));

    login_dialog->setModal(true);
    login_dialog->show();
}

void MainWindow::establish_connection(Server* server) {
    Server::server_ptr new_server(server);
    remote_server = std::move(new_server);
    server = nullptr;

    auto msg = QString::fromStdString(remote_server->get_address());
    statusBar()->showMessage("Connected to " + msg, 2000);
}

void MainWindow::action_disconnect_triggered() {
    if (remote_server == nullptr) {
        return;
    }

    remote_server->disconnect();
    auto msg = QString::fromStdString(remote_server->get_address());
    statusBar()->showMessage("Disconnected from " + msg, 2000);
    remote_server = nullptr;
}

MainWindow::~MainWindow() {
    if (remote_server != nullptr) {
        remote_server->disconnect();
    }
    delete ui;
}
