#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>

namespace message
{
class Message
{
public:
    Message();
    Message(int head, const std::string& body);
    Message(const Message& other);

    Message& operator=(const Message& other);

    std::string ToString() const;
    std::string GetHeadText() const;
    int GetHead() const;
    std::string GetBody() const;

    void SetHead(int head);
    void SetBody(const std::string body);

private:
    int _head;
    std::string _body;
};
}
#endif
