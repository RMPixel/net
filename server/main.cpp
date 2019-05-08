#include <QCoreApplication>
#include <protocol.h>
#include <QDebug>
#include <zmq.hpp>

const QString SERVER_PUBLISHER_HOST("127.0.0.1");
const QString PUBLISHER_PORT("5556");
const QString PUBLISHER_URL=QString("tcp://%1:%2").arg(SERVER_PUBLISHER_HOST).arg(PUBLISHER_PORT);

const QString SERVER_CONTROL_HOST("*");
const QString CONTROL_PORT("5555");
const QString CONTROL_URL=QString("tcp://%1:%2").arg(SERVER_CONTROL_HOST).arg(CONTROL_PORT);


int main(int argc, char *argv[])
{

    //QCoreApplication a(argc, argv);

    //return a.exec();


  zmq::context_t context(1);
   zmq::socket_t controlsocket (context, ZMQ_REP);
    zmq::socket_t publishersocket (context, ZMQ_PUB);

    //controlsocket.bind(CONTROL_URL.toStdString().c_str());
    controlsocket.bind(CONTROL_URL.toLocal8Bit());


    //publishersocket.bind(publisherurl.toLocal8Bit());
    //publishersocket.bind(PUBLISHER_URL.toStdString().c_str());
    publishersocket.bind(PUBLISHER_URL.toLocal8Bit());

    while (true)
    {

    zmq::message_t request;    
    controlsocket.recv(&request);
    QString command =QString::fromUtf8(reinterpret_cast<char *>(request.data()),request.size());

    if (command == CONNECT_COMMAND){
        qDebug() << "CONNECT" ;
        zmq::message_t borrador= messageFromQString(SUCCESS_REPLY);
         controlsocket.send(borrador,ZMQ_SNDMORE);
         borrador= messageFromQString(PUBLISHER_PORT);
          controlsocket.send(borrador,ZMQ_SNDMORE);
          borrador= messageFromQString(SERVER_PUBLISHER_HOST);
          controlsocket.send(borrador,0);

    }
    else if (command == PUBLISH_COMMAND){
        qDebug() << "PUBLISH" ;
        if (request.more()) {
                    controlsocket.recv(&request);
                    QString url= QString::fromUtf8(reinterpret_cast<char *>(request.data()),request.size());
                    qDebug() << "URL reenviada" << url;
                    publishersocket.send(request);
        }

        zmq::message_t borrador= messageFromQString(SUCCESS_REPLY);
        controlsocket.send(borrador);

    }
    else if (command == CLOSE_COMMAND){
        qDebug() << "CLOSE" ;
        zmq::message_t borrador= messageFromQString(SUCCESS_REPLY);
        controlsocket.send(borrador);

    }
    else
    {
         qDebug() << QString("Comando Desconocido : %1").arg(command);
         zmq::message_t response = messageFromQString(ERROR_REPLY);
         controlsocket.send(response,0);
      }
   }


  return 0;
}

