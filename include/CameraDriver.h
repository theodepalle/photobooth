#pragma once

#include <gphoto2/gphoto2.h>
#include <string>

/*
    This class is used to take and save picture from a DLSR camera
    it used the libgphoto2
*/
class CameraDriver
{
public:
    CameraDriver();
    ~CameraDriver();
    bool capture_to_file(const std::string & filename);
private:
    static void error_dumper(GPLogLevel level, const char* domain, const char* str, void* data);

    GPContext* context;
    Camera* camera;
};
