#pragma once

#include <gphoto2/gphoto2.h>

#include "AbstractCameraDriver.h"

/*
    This class is used to take and save picture from a DLSR camera
    it used the libgphoto2
*/
class CameraDriver : public AbstractCameraDriver
{
public:
    CameraDriver();
    ~CameraDriver();
    void captureToFile(const std::string & filename) override;
    void capturePreviewToFile(const std::string & filename) override;
private:
    static void error_dumper(GPLogLevel level, const char* domain, const char* str, void* data);

    GPContext* context;
    Camera* camera;
};
