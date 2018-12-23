#include "Message.h"

namespace message
{
Message::Message():
        _head(0),
        _body("")
{
}

Message::Message(int head, const std::string& body):
        _head(head),
        _body(body)
{
}

Message::Message(const Message& other):
        _head(other._head),
        _body(other._body)
{
}

Message& Message::operator=(const Message& other)
{
    _head = other._head;
    _body = other._body;
    return *this;
}

std::string Message::ToString() const
{
    return GetHeadText() + _body;
}

std::string Message::GetHeadText() const
{
    return std::to_string(_head) + "$";
}

int Message::GetHead() const
{
    return _head;
}

std::string Message::GetBody() const
{
    return _body;
}

void Message::SetHead(int head)
{
    _head = head;
}

void Message::SetBody(const std::string body)
{
    _body = body;
}
}
