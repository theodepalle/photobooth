#include <QDateTime>

#include "PhotoBooth.h"

PhotoBooth::PhotoBooth(QObject *parent) : QObject(parent)
{
    camera = std::make_unique<CameraDriver>();
}

void PhotoBooth::takePicture()
{
    QDateTime date = QDateTime::currentDateTime();
    QString formattedTime = date.toString("dd.MM.yy hh:mm:ss");
    std::string fileName = "capture" + formattedTime.toStdString() + ".jpg";
    camera->capture_to_file(fileName);
}
