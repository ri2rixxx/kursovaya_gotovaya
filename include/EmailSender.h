#ifndef EMAIL_SENDER_H
#define EMAIL_SENDER_H

#include <string>

class EmailSender {
public:
    EmailSender();
    ~EmailSender();
    
    bool sendFeedback(const std::string& userName, 
                     const std::string& userEmail, 
                     const std::string& message);
};

#endif
