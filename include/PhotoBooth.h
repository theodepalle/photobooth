#pragma once

#include <QObject>

#include "CameraDriver.h"

class PhotoBooth : public QObject
{
    Q_OBJECT
public:
    explicit PhotoBooth(QObject* parent = nullptr);
public slots:
    void takePicture();
    QString getLastCapturePath();
private:
    std::unique_ptr<CameraDriver> camera;
    QString m_lastCapturePath = "";
};
