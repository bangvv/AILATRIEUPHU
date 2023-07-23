#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <ctype.h>
#include <QVariant>
#include "BackEnd.h"
#include "CountTime.h"

int main(int argc, char *argv[])
{
   if(argc != 3)
   {
       printf("Usage: %s <server_ip> <server_port>\n", argv[0]);
       exit(0);
   }

   BackEnd::server_ip = argv[1];
   BackEnd::server_port = atoi(argv[2]);

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("applicationDirPath", QUrl::fromLocalFile(QCoreApplication::applicationDirPath()));

    const QUrl url(QStringLiteral("qrc:/qml/main.qml"));

    qmlRegisterType<BackEnd>("AiLaTrieuPhu.BackEnd",1,0,"BackEnd");
    qmlRegisterType<Scores>("Scores", 1, 0, "Scores");
    Scores *sc = Scores::instance();
    engine.rootContext()->setContextProperty("scid", sc);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
