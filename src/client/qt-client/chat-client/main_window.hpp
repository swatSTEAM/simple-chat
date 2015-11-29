#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP

#include <QMainWindow>
#include <QThread>
#include "remote_server.hpp"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void connection_established();
    void connection_failed();
    void action_connect_triggered();
    void action_disconnect_triggered();


    void establish_connection(const std::string&,
                              const std::string&, int);

private:
    Ui::MainWindow *ui;
    Server *remote_server = nullptr;
};

#endif // MAIN_WINDOW_HPP
