#include <sstream>

#include <QDateTime>

#include "EmailSender.h"

typedef struct curl_slist curl_slist;

EmailSender::EmailSender(const std::string& address, int port,
                         ConnectionType connectionType)
{
    m_curl = curl_easy_init();

    if(!m_curl)
    {
        throw std::runtime_error("Impossible to init curl");
    }

    std::stringstream sstr;
    sstr << "smtp" << (connectionType == ConnectionType::SSL ? "s" : "") << "://"
         << address + ":" << port;
    curl_easy_setopt(m_curl, CURLOPT_URL, sstr.str().c_str());

    if(connectionType == ConnectionType::TLS)
    {
        curl_easy_setopt(m_curl, CURLOPT_USE_SSL, (long)CURLUSESSL_ALL);
    }
}

EmailSender::~EmailSender()
{
    curl_easy_cleanup(m_curl);
}

void EmailSender::setCredentials(const std::string& userName, const std::string& password)
{
    curl_easy_setopt(m_curl, CURLOPT_USERNAME, userName.c_str());
    curl_easy_setopt(m_curl, CURLOPT_PASSWORD, password.c_str());
}

void EmailSender::send(const Email& email)
{
    if(!email.isValid())
    {
        throw std::runtime_error("Invalid Email");
    }

    // From
    curl_easy_setopt(m_curl, CURLOPT_MAIL_FROM, email.m_senderEmail.c_str());

    // To
    curl_slist* recipients = NULL;
    recipients = curl_slist_append(recipients, email.m_receiverEmail.c_str());
    curl_easy_setopt(m_curl, CURLOPT_MAIL_RCPT, recipients);

    // Headers
    std::vector<std::string> headers = mailToHeaders(email);
    curl_slist* curlHeaders = NULL;
    for(const std::string& header : headers)
    {
        curlHeaders = curl_slist_append(curlHeaders, header.c_str());
    }
    curl_easy_setopt(m_curl, CURLOPT_HTTPHEADER, curlHeaders);

    // Content
    curl_mime* mime = curl_mime_init(m_curl);
    curl_mimepart* part;
    if(!email.m_content.empty())
    {
        part = curl_mime_addpart(mime);
        curl_mime_data(part, email.m_content.c_str(), CURL_ZERO_TERMINATED);
        curl_slist* slist = curl_slist_append(NULL, "Content-Disposition: inline");
        curl_mime_headers(part, slist, 1);
    }

    // Attachement
    if(!email.m_attachments.empty())
    {
        for(const std::string& attachement : email.m_attachments)
        {
            part = curl_mime_addpart(mime);
            curl_mime_filedata(part, attachement.c_str());
            curl_mime_encoder(part, "base64");
        }
    }
    curl_easy_setopt(m_curl, CURLOPT_MIMEPOST, mime);

    // Set verbosity to curl (to see what it is currently doing)
    curl_easy_setopt(m_curl, CURLOPT_VERBOSE, 1L);

    CURLcode res = curl_easy_perform(m_curl);

    if(res != CURLE_OK)
    {
        // Free
        curl_slist_free_all(recipients);
        curl_slist_free_all(curlHeaders);
        curl_mime_free(mime);

        throw std::runtime_error("Impossible to send email. Error " +
                                 std::to_string(res));
    }

    // Free
    curl_slist_free_all(recipients);
    curl_slist_free_all(curlHeaders);
    curl_mime_free(mime);
}

std::vector<std::string> EmailSender::mailToHeaders(const Email& email)
{
    std::vector<std::string> headers;

    // Date
    QDateTime date = QDateTime::currentDateTime();
    std::string formattedDateTime =
        date.toString("ddd, dd MMMM yyyy hh:mm:ss").toStdString() + " +0100";
    headers.push_back("Date: " + formattedDateTime);

    // From
    std::string from = "From: " + email.m_senderEmail;
    if(!email.m_senderName.empty())
    {
        from = from + " (" + email.m_senderName + ")";
    }
    headers.push_back(from);

    // To
    headers.push_back("To :" + email.m_receiverEmail);

    // Subject
    headers.push_back("Subject: " + email.m_subject);

    return headers;
}
