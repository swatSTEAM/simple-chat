#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP

#include <QObject>
#include <QThread>
#include "remote_server.hpp"

class ChatCore : public QObject {
    Q_OBJECT

public:
    explicit ChatCore();
    ~ChatCore();

signals:
    void connected(const QString& mess);
    void disconnected(const QString& mess);
    void failed(const QString& mess);
    void incoming_mess(const QString& mess);
    void users(const QString& mess);
//    void

public slots:
    void tryconnect(const QString&, const QString&, const QString&);


    void connection_established();
    void connection_failed();
    void disconnect();
private:
    Server *remote_server = nullptr;
};

#endif // MAIN_WINDOW_HPP
