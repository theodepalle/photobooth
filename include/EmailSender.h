#pragma once

#include <curl/curl.h>

#include "Email.h"

class EmailSender
{
public:
    enum ConnectionType
    {
        SSL = 0,
        TLS
    };

    EmailSender(const std::string& address, int port, ConnectionType connectionType);
    ~EmailSender();

    void setCredentials(const std::string& userName, const std::string& password);
    void send(const Email& email);

private:
    std::vector<std::string> mailToHeaders(const Email& email);

    CURL* m_curl;
};
