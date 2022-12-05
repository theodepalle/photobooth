#include <QDateTime>
#include <QFile>

#include <filesystem>
#include <iostream>

#include <yaml-cpp/yaml.h>

#include "EmailSender.h"
#include "PhotoBooth.h"

PhotoBooth::PhotoBooth(AbstractCameraDriver& cameraDriver, QObject* parent)
    : QObject(parent), m_cameraDriver(cameraDriver)
{
    // Capture file name are relative to where the executable iS. To display it in gui
    // we need the absolute path
    std::string currentPath = std::filesystem::current_path();
    m_executablePath = QString::fromStdString(currentPath);

    // Read all needed parameters from yaml config file
    readParameters();
}

PhotoBooth::~PhotoBooth()
{
    if(m_state == CAPTURE)
    {
        m_captureThread.join();
    }
    else if(m_state == LIVE_VIEW)
    {
        stopLiveView();
    }
    else if(m_state == SENDING)
    {
        m_sendMailThread.join();
    }

    if(m_captureThread.joinable())
    {
        m_captureThread.join();
    }
    if(m_liveViewThread.joinable())
    {
        m_liveViewThread.join();
    }
    if(m_sendMailThread.joinable())
    {
        m_sendMailThread.join();
    }
}

void PhotoBooth::capture()
{
    if(m_state != IDLE)
    {
        return;
    }

    // if we already capture we need to join thread
    if(m_captureThread.joinable())
    {
        m_captureThread.join();
    }

    QDateTime date = QDateTime::currentDateTime();
    QString formattedTime = date.toString("dd-MM-yy_hh-mm-ss");
    QString fileName = "capture" + formattedTime + ".jpg";
    QString fullPath = m_executablePath + "/" + fileName;

    m_captureThread = std::thread([&, fullPath, fileName]() {
        {
            std::scoped_lock lock(m_mutex);
            setState(CAPTURE);
            m_cameraDriver.captureToFile(fileName.toStdString());
            setState(IDLE);
        }
        emit newCapture(fullPath);
    });
}

void PhotoBooth::startLiveView()
{
    if(m_state != IDLE)
    {
        return;
    }

    m_isLiveViewOn.store(true);
    m_liveViewThread = std::thread([&]() {
        {
            std::scoped_lock lock(m_mutex);
            setState(LIVE_VIEW);
        }
        while(m_isLiveViewOn.load())
        {
            {
                std::scoped_lock lock(m_mutex);
                m_cameraDriver.capturePreviewToFile(m_liveViewFileName.toStdString());
            }
            emit newLiveView(m_executablePath + "/" + m_liveViewFileName);
        }

        {
            std::scoped_lock lock(m_mutex);
            setState(IDLE);
        }
        m_cv.notify_all();
    });
}

void PhotoBooth::stopLiveView()
{
    if(m_state != LIVE_VIEW)
    {
        return;
    }

    m_isLiveViewOn.store(false);
    std::unique_lock lock(m_mutex);
    m_cv.wait(lock, [&]() { return m_state == IDLE; });

    // we stop liveView so we can now join thread to reuse it on next liveView
    m_liveViewThread.join();
}

void PhotoBooth::deleteCapture(const QString& path)
{
    if(std::filesystem::remove(path.toStdString()))
    {
        std::cout << "Capture " << path.toStdString() << " succefuly deleted"
                  << std::endl;
    }
    else
    {
        std::cout << "Impossible to remove capture " << path.toStdString() << std::endl;
    }
}

void PhotoBooth::sendCaptureByMail(const QString& path, const QString& receiverMail)
{
    if(m_state != IDLE)
    {
        return;
    }

    // if we already send we need to join thread to reuse it
    if(m_sendMailThread.joinable())
    {
        m_sendMailThread.join();
    }

    m_sendMailThread = std::thread([&, path, receiverMail]() {
        {
            std::scoped_lock lock(m_mutex);
            setState(SENDING);
        }

        // Create email
        Email email;
        email.setSender(m_params.senderEmail, m_params.senderName);
        email.setReceiver(receiverMail.toStdString());
        email.setSubject(m_params.emailSubject);
        email.setContent(m_params.emailContent);
        email.addAttachment(path.toStdString());

        // And send it
        EmailSender::ConnectionType connectionType =
            m_params.connectionType == "tls" ? EmailSender::ConnectionType::TLS
                                             : EmailSender::ConnectionType::SSL;
        EmailSender emailSender(m_params.serverAddress, m_params.serverPort,
                                connectionType);
        emailSender.setCredentials(m_params.senderEmail, m_params.senderPassword);
        emailSender.send(email);

        {
            std::scoped_lock lock(m_mutex);
            setState(IDLE);
        }

        emit mailSended();
    });
}

void PhotoBooth::setState(const State& state)
{
    if(m_state == state)
    {
        return;
    }

    m_state = state;
    emit newState(m_state);
}

void PhotoBooth::readParameters()
{
    YAML::Node config = YAML::LoadFile("../config/parameters.yaml");

    // all of this command throw if it can't read a parameter. That is wanted because all
    // parameters are necessary (no default value)
    m_params.serverAddress = config["serverAddress"].as<std::string>();
    m_params.serverPort = config["serverPort"].as<int>();
    m_params.connectionType = config["connectionType"].as<std::string>();
    m_params.senderEmail = config["senderEmail"].as<std::string>();
    m_params.senderPassword = config["senderPassword"].as<std::string>();
    m_params.senderName = config["senderName"].as<std::string>();
    m_params.emailSubject = config["emailSubject"].as<std::string>();
    m_params.emailContent = config["emailContent"].as<std::string>();
}
