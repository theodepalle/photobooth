#include <iostream>

#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "photobooth.h"

static void errordumper(GPLogLevel level, const char* domain, const char* str, void* data)
{
    if(level == GP_LOG_ERROR)
    {
        std::cout << "error happend :" << std::endl;
        std::cout << domain << std::endl;
        std::cout << str << std::endl;
        std::cout << data << std::endl;
    }
}

static void capture_to_file(Camera* camera, GPContext* context, char* fn)
{
    int fd, retval;
    CameraFile* file;
    CameraFilePath camera_file_path;

    if(gp_camera_capture(camera, GP_CAPTURE_IMAGE, &camera_file_path, context) != GP_OK)
    {
        std::cout << "Impossible to capture image. Retval : " << std::endl;
        return;
    }

    fd = open(fn, O_CREAT | O_WRONLY, 0644);
    retval = gp_file_new_from_fd(&file, fd);

    retval = gp_camera_file_get(camera, camera_file_path.folder, camera_file_path.name,
                                GP_FILE_TYPE_NORMAL, file, context);

    gp_file_free(file);

    retval = gp_camera_file_delete(camera, camera_file_path.folder, camera_file_path.name,
                                   context);
}

int main()
{
    std::cout << "Photobooth starting..." << std::endl;

    if(!gp_log_add_func(GP_LOG_ERROR, errordumper, NULL) != GP_OK)
    {
        std::cout << "Impossible to add log funciton" << std::endl;
    }

    // Create camera and context class /!\ memory alloc
    GPContext* context = gp_context_new();
    if(!context)
    {
        std::cout << "Impossible to create context" << std::endl;
        return 1;
    }
    Camera* camera;
    if(gp_camera_new(&camera) != GP_OK)
    {
        std::cout << "Impossible to create camera" << std::endl;
        return 1;
    }

    if(gp_camera_init(camera, context) != GP_OK)
    {
        std::cout << "Impossible to init camera" << std::endl;
        return 1;
    }

    capture_to_file(camera, context, "test_capture.jpg");

    // free camera and context
    gp_camera_exit(camera, context);

    return 0;
}
