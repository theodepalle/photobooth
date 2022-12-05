#pragma once

#include <string>
#include <vector>

class Email
{
public:
    Email(){};

    void setSender(const std::string& mail, const std::string& name = "")
    {
        m_senderEmail = mail;
        m_senderName = name;
    };
    void setReceiver(const std::string& receiverEmail)
    {
        m_receiverEmail = receiverEmail;
    }
    void setSubject(const std::string& subject) { m_subject = subject; }
    void setContent(const std::string& content) { m_content = content; }
    void addAttachment(const std::string& filePath) { m_attachments.push_back(filePath); }

    bool isValid() const { return !m_senderEmail.empty() && !m_receiverEmail.empty(); }

private:
    friend class EmailSender;

    std::string m_senderEmail = "";
    std::string m_senderName = "";
    std::string m_receiverEmail;
    std::string m_subject = "";
    std::string m_content = "";
    std::vector<std::string> m_attachments;
};
