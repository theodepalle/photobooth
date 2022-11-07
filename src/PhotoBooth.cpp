#include <QDateTime>

#include <filesystem>
#include <iostream>

#include "PhotoBooth.h"

PhotoBooth::PhotoBooth(QObject* parent) : QObject(parent)
{
    camera = std::make_unique<CameraDriver>();
}

void PhotoBooth::takePicture()
{
    QDateTime date = QDateTime::currentDateTime();
    QString formattedTime = date.toString("dd.MM.yy hh:mm:ss");
    QString fileName = "capture" + formattedTime + ".jpg";
    std::string currentPath = std::filesystem::current_path();

    if(camera->capture_to_file(fileName.toStdString()))
    {
        QString fullPath = QString::fromStdString(currentPath) + "/" + fileName;
        m_lastCapturePath = fullPath;
    }
}

QString PhotoBooth::getLastCapturePath()
{
    return m_lastCapturePath;
}
