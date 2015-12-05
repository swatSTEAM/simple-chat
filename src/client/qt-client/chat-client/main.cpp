#include <QApplication>
#include <QQmlApplicationEngine>
#include <chat_core.hpp>
#include <QQmlContext>
#include <QRegExpValidator>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QQmlApplicationEngine engine;
    ChatCore core;
    QQmlContext* uiCtx = engine.rootContext();
    uiCtx->setContextProperty("core", &core);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    auto uiCore = engine.rootObjects().takeFirst();

    QObject::connect(uiCore, SIGNAL(loginSignal(QString, QString, QString)),
                     &core, SLOT(tryconnect(QString, QString, QString)));
    QObject::connect(uiCore, SIGNAL(disconnectSignal()),
                     &core, SLOT(disconnect()));
//    QObject::connect(&core, SIGNAL(failed(QString, int)),
//                     engine.rootObjects().takeFirst(), SLOT(showErr(QString, int)));


//    QApplication::quit();
    return app.exec();
}
