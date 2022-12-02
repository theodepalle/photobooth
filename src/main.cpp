#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "PhotoBooth.h"
#include "CameraDriver.h"

int main(int argc, char **argv)
{
    qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(u"qrc:ui/main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

    CameraDriver cameraDriver;
    PhotoBooth photoBooth(cameraDriver);

    engine.rootContext()->setContextProperty("photoBooth", &photoBooth);
    qRegisterMetaType<PhotoBooth::State>();

    engine.load(url);

    return app.exec();
}
