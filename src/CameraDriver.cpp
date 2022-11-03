#include <iostream>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "CameraDriver.h"

CameraDriver::CameraDriver()
{
    std::cout << "CameraDriver init" << std::endl;
    gp_log_add_func(GP_LOG_ERROR, error_dumper, NULL);

    context = gp_context_new();
    if (!context)
    {
        std::cout << "Impossible to create context" << std::endl;
        // TODO create an error handle logic
    }

    if (gp_camera_new(&camera) != GP_OK)
    {
        std::cout << "Impossible to create camera" << std::endl;
        // TODO create an error handle logic
    }

    if (gp_camera_init(camera, context) != GP_OK)
    {
        std::cout << "Impossible to init camera" << std::endl;
        // TODO create an error handle logic
    }
}

CameraDriver::~CameraDriver()
{
    gp_camera_exit(camera, context);
}

void CameraDriver::error_dumper(GPLogLevel level, const char *domain, const char *str, void *data)
{
    if (level == GP_LOG_ERROR)
    {
        std::cout << "error happend :" << std::endl;
        std::cout << domain << std::endl;
        std::cout << str << std::endl;
        std::cout << data << std::endl;
    }
}

bool CameraDriver::capture_to_file(const std::string &filename)
{
    int fd;
    CameraFile *file;
    CameraFilePath camera_file_path;

    if (gp_camera_capture(camera, GP_CAPTURE_IMAGE, &camera_file_path, context) != GP_OK)
    {
        std::cout << "Impossible to capture image" << std::endl;
        return false;
    }

    fd = open(filename.c_str(), O_CREAT | O_WRONLY, 0644);

    if (gp_file_new_from_fd(&file, fd) != GP_OK)
    {
        std::cout << "gp_file_new_from_fd fail" << std::endl;
        return false;
    }

    if (gp_camera_file_get(camera, camera_file_path.folder, camera_file_path.name,
                           GP_FILE_TYPE_NORMAL, file, context) != GP_OK)
    {
        std::cout << "gp_camera_file_get fail" << std::endl;
        return false;
    }

    gp_file_free(file);

    if (gp_camera_file_delete(camera, camera_file_path.folder, camera_file_path.name,
                              context) != GP_OK)
    {
        std::cout << "gp_camera_file_delete fail" << std::endl;
        return false;
    }

    return true;
}