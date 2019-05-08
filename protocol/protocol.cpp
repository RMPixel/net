#include "protocol.h"
#include <zmq.hpp>

zmq::message_t messageFromQString(const QString & message)
{
    QByteArray bytes = message.toUtf8();
    return zmq::message_t(bytes.data(),bytes.size());
}
