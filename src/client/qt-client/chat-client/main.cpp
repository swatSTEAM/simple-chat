#include <QApplication>
#include <QQmlApplicationEngine>
#include <chat_core.hpp>
#include <QQmlContext>
#include <QRegExpValidator>

int main(int argc, char *argv[])
{
    qRegisterMetaType<QTcpSocket::SocketError>();
    qRegisterMetaType<std::vector<QString>>();
//    auto thread = new QThread;
    QApplication app(argc, argv);

    QQmlApplicationEngine engine;

    ChatCore core;
    engine.rootContext()->setContextProperty("core", &core);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    auto ui_core = engine.rootObjects().takeFirst();

    core.add_UI(ui_core);
//    core.moveToThread(thread);


//    QObject::connect(&core, SIGNAL(failed(QString, int)),
//                     engine.rootObjects().takeFirst(), SLOT(showErr(QString, int)));


//    QApplication::quit();
    return app.exec();
}
