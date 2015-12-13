#include "worker.h"

Worker::Worker(QObject *parent, std::queue<QString> *q, QMutex *m) :
    QThread(parent)
{
    stop = false;
    msg_queue = q;
    mutex = m;
}


void Worker::run()
{
    while (!stop) {
        if (!msg_queue->empty())
        {
            mutex->lock();
                auto msg = msg_queue->back();
            mutex->unlock();
            auto d = QJsonDocument::fromJson(msg.toUtf8());
            auto obj = d.object();

            if (obj.contains("authorization_status"))
            {
                auto status = obj["authorization_status"].toString();
                if (status == "succsess")
                {
                    emit authorization_succsess();
                }
                else
                {
                    emit authorization_failed();
                }
            }

            if (obj.contains("online_users"))
            {
                emit online_users(msg);
            }

            if (obj.contains("user_connected"))
            {
                auto user = obj["user_connected"].toString();
                emit user_connected(user);
            }

            if (obj.contains("user_disconnected"))
            {
                auto user = obj["user_disconnected"].toString();
                emit user_disconnected(user);
            }

            if (obj.contains("msg_from_user") && obj.contains("msg"))
            {
                auto user = obj["msg_from_user"].toString();
                auto msg = obj["msg"].toString();
                emit msg_from_user(user, msg);
            }

//            if (obj.contains("msg_delivery_error"))
//            {
//                auto user = obj["msg_from_user"].toString();
//                auto msg = obj["msg"].toString();
//                emit msg_failed(user, msg);
//            }

            mutex->lock();
                msg_queue->pop();
            mutex->unlock();
        }
    }
}

void Worker::abort()
{
    stop = true;
}
