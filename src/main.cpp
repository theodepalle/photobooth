#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "PhotoBooth.h"

int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);

    std::unique_ptr<PhotoBooth> myBooth = std::make_unique<PhotoBooth>();

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:src/main.qml")));

    if (engine.rootObjects().isEmpty())
    {
        return -1;
    }

    QQmlContext * rootContext = engine.rootContext();
    rootContext->setContextProperty("PhotoBooth", myBooth.get());

    return app.exec();
}
