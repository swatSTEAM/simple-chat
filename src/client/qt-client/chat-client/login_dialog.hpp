#ifndef LOGIN_DIALOG_HPP
#define LOGIN_DIALOG_HPP

#include <QDialog>
#include "remote_server.hpp"

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = 0);
    ~LoginDialog();

signals:
    void connected_to_server(Server*);

private slots:
    void login_btn_clicked();
private:
    Ui::LoginDialog *ui;
};

#endif // LOGIN_DIALOG_HPP
