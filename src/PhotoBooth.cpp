#include <QDateTime>

#include <filesystem>
#include <iostream>

#include "PhotoBooth.h"

PhotoBooth::PhotoBooth(AbstractCameraDriver& cameraDriver, QObject* parent)
    : QObject(parent), m_cameraDriver(cameraDriver)
{
    std::string currentPath = std::filesystem::current_path();
    m_executablePath = QString::fromStdString(currentPath);
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
        m_liveViewThread.join();
    }

    if(m_captureThread.joinable())
    {
        m_captureThread.join();
    }
    if(m_liveViewThread.joinable())
    {
        m_liveViewThread.join();
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

    // if we already capture we need to join thread
    if(m_liveViewThread.joinable())
    {
        m_liveViewThread.join();
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
