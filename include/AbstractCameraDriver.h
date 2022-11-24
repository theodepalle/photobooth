#pragma once

#include <string>

class AbstractCameraDriver
{
public:
    virtual void captureToFile(const std::string& fileName) = 0;
    virtual void capturePreviewToFile(const std::string& fileName) = 0;
};
