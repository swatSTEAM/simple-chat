#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP

#include <QMainWindow>
#include <memory>
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
    void action_connect_triggered();
    void action_disconnect_triggered();
    void establish_connection(Server*);
private:
    Ui::MainWindow *ui;
    std::unique_ptr<Server> remote_server = nullptr;

};

#endif // MAIN_WINDOW_HPP
