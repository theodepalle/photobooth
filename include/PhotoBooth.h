#pragma once

#include <QObject>
#include <QString>

#include <mutex>
#include <thread>
#include <condition_variable>

#include "AbstractCameraDriver.h"

class PhotoBooth : public QObject
{
    Q_OBJECT
public:
    enum State{
        IDLE,
        CAPTURE,
        LIVE_VIEW
    };
    Q_ENUM(State);
    PhotoBooth(AbstractCameraDriver& cameraDriver, QObject* parent = nullptr);
    ~PhotoBooth();

public slots:
    void capture();
    void startLiveView();
    void stopLiveView();
signals:
    void newCapture(const QString& capturePath);
    void newLiveView(const QString& viewPath);
    void newState(const PhotoBooth::State& newState);

private:
    void setState(const State& state);

    const QString m_liveViewFileName = "preview.jpg";

    std::mutex m_mutex;
    std::condition_variable m_cv;
    std::thread m_captureThread;
    std::thread m_liveViewThread;

    AbstractCameraDriver& m_cameraDriver;

    QString m_executablePath = "";

    State m_state = IDLE;

    std::atomic<bool> m_isLiveViewOn = false;
};
