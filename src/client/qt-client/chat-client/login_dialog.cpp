#include "login_dialog.hpp"
#include "ui_login_dialog.h"

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);

    ui->nickname_edit->setAttribute(Qt::WA_MacShowFocusRect, false);
    ui->ip_edit->setAttribute(Qt::WA_MacShowFocusRect, false);
    ui->port_edit->setAttribute(Qt::WA_MacShowFocusRect, false);

    QString oIpRange = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
    QRegExp oIpRegex ("^" + oIpRange
                  + "\\." + oIpRange
                  + "\\." + oIpRange
                  + "\\." + oIpRange + "$");
    ui->ip_edit->setValidator(new QRegExpValidator(oIpRegex));

    ui->port_edit->setValidator(new QIntValidator(1,65535, this));
    connect(ui->login_btn, SIGNAL(clicked()), SLOT(login_btn_clicked()));
}

LoginDialog::~LoginDialog() {
    delete ui;
}

void LoginDialog::login_btn_clicked() {
    auto nickname = ui->nickname_edit->text().toStdString();
    auto ip = ui->ip_edit->text().toStdString();
    auto port = ui->port_edit->text().toInt();

    Server *remote = new Server(nickname, ip, port);
    auto connect_result = remote->connect();
    if (connect_result == Server::SUCCESS) {
        emit connected_to_server(remote);
        close();
    } else {
        switch (connect_result) {
        case Server::SOCKET_ERROR:
            QMessageBox::critical(this, "Error", "Socket error.");
            break;
        case Server::ADDRESS_ERROR:
             QMessageBox::critical(this, "Error", "Incorrect address.");
            break;
        case Server::CONNECTION_ERROR: {
             auto msg = QString("%1:%2 is inaccessible.").arg(ip.c_str()).arg(port);
             QMessageBox::critical(this, "Error", msg.toStdString().c_str());
             break;
        }
        default:
            QMessageBox::critical(this, "Error", "Unknow error.");
            break;
        }
    }
}
