#include <QDateTime>
#include <QFile>

#include <filesystem>
#include <iostream>

#include <yaml-cpp/yaml.h>

#include <curl/curl.h>

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

void PhotoBooth::sendCaptureByMail(const QString& path, const QString& email)
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

    m_sendMailThread = std::thread([&, path, email]() {
        {
            std::scoped_lock lock(m_mutex);
            setState(SENDING);
        }

        CURL* curl;
        CURLcode res = CURLE_OK;

        curl = curl_easy_init();
        if(curl)
        {
            std::string url = "smtps://" + m_params.serverAddress + ":" +
                              std::to_string(m_params.serverPort);
            // std::string url = "smtps://" + m_params.serverAddress;
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

            // autantification
            curl_easy_setopt(curl, CURLOPT_USERNAME, m_params.senderEmail.c_str());
            curl_easy_setopt(curl, CURLOPT_PASSWORD, m_params.senderPassword.c_str());

            // from
            curl_easy_setopt(curl, CURLOPT_MAIL_FROM, m_params.senderEmail.c_str());

            // to
            struct curl_slist* recipients = NULL;
            recipients = curl_slist_append(recipients, email.toStdString().c_str());
            curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);

            // headers
            QDateTime date = QDateTime::currentDateTime();
            std::string formattedDateTime =
                date.toString("ddd, dd MMMM yyyy hh:mm:ss").toStdString() + " +0100";
            std::vector<std::string> headers = {
                "Date: " + formattedDateTime, "To: " + email.toStdString(),
                "From: " + m_params.senderEmail + " (Photobooth)",
                "Subject: " + m_params.m_emailSubject};
            struct curl_slist* curlHeaders = NULL;
            for(const std::string& header : headers)
            {
                curlHeaders = curl_slist_append(curlHeaders, header.c_str());
            }
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, curlHeaders);

            // Content
            curl_mime* mime = curl_mime_init(curl);
            curl_mimepart* part = curl_mime_addpart(mime);
            curl_mime_data(part, m_params.emailContent.c_str(), CURL_ZERO_TERMINATED);
            struct curl_slist* slist =
                curl_slist_append(NULL, "Content-Disposition: inline");
            curl_mime_headers(part, slist, 1);

            // Attachement
            part = curl_mime_addpart(mime);
            curl_mime_filedata(part, path.toStdString().c_str());
            curl_mime_encoder(part, "base64");
            curl_easy_setopt(curl, CURLOPT_MIMEPOST, mime);

            curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

            res = curl_easy_perform(curl);

            if(res != CURLE_OK)
                fprintf(stderr, "curl_easy_perform() failed: %s\n",
                        curl_easy_strerror(res));

            // free
            curl_slist_free_all(recipients);
            curl_slist_free_all(curlHeaders);
            curl_easy_cleanup(curl);
            curl_mime_free(mime);
        }

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
    // m_params.connectionType = config["connectionType"].as<int>();
    m_params.senderEmail = config["senderEmail"].as<std::string>();
    m_params.senderPassword = config["senderPassword"].as<std::string>();
    m_params.m_emailSubject = config["emailSubject"].as<std::string>();
    m_params.emailContent = config["emailContent"].as<std::string>();
}
