#pragma once

#include <QObject>
#include <QString>

#include <condition_variable>
#include <mutex>
#include <thread>

#include "AbstractCameraDriver.h"

class PhotoBooth : public QObject
{
    Q_OBJECT
public:
    struct Params
    {
        std::string serverAddress;
        int serverPort;
        int connectionType; // see SmtpClient::ConnectionType
        std::string senderEmail;
        std::string senderPassword;
        std::string m_emailSubject;
        std::string emailContent;
    };

    enum State {
        IDLE,
        CAPTURE,
        LIVE_VIEW,
        SENDING,
    };
    Q_ENUM(State);
    PhotoBooth(AbstractCameraDriver& cameraDriver, QObject* parent = nullptr);
    ~PhotoBooth();

public slots:
    void capture();
    void startLiveView();
    void stopLiveView();
    void deleteCapture(const QString& path);
    void sendCaptureByMail(const QString& path, const QString& email);
signals:
    void newCapture(const QString& capturePath);
    void newLiveView(const QString& viewPath);
    void newState(const PhotoBooth::State& newState);
    void mailSended();

private:
    void setState(const State& state);
    void readParameters();

    const QString m_liveViewFileName = "preview.jpg";

    Params m_params;

    std::mutex m_mutex;
    std::condition_variable m_cv;
    std::thread m_captureThread;
    std::thread m_liveViewThread;
    std::thread m_sendMailThread;

    AbstractCameraDriver& m_cameraDriver;

    QString m_executablePath = "";

    State m_state = IDLE;

    std::atomic<bool> m_isLiveViewOn = false;
};
