#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <QString>
#include <zmq.hpp>
const QString CONNECT_COMMAND ("CONNECT");
const QString PUBLISH_COMMAND ("PUBLISH");
const QString CLOSE_COMMAND ("CLOSE");

const QString SUCCESS_REPLY ("SUCCES");
const QString ERROR_REPLY ("ERROR");

zmq::message_t messageFromQString(const QString & message);

#endif // PROTOCOL_H
