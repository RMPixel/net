#include <QCoreApplication>
#include <zmq.hpp>
#include <protocol.h>
#include <QString>
#include <QDebug>
#include <unistd.h>
#include <QTime>
const  QString SERVER_CONTROL_HOST("localhost");
const QString CONTROL_PORT("5555");
const QString CONTROL_URL=QString("tcp://%1:%2").arg(SERVER_CONTROL_HOST).arg(CONTROL_PORT);

QString PUBLISHER_URL=QString("tcp://%1:%2");
QString SUSCRIBER_PORT,SERVER_SUSCRIBER_HOST;

int main(int argc, char *argv[])
{
    //QCoreApplication a(argc, argv);

    //return a.exec();

    zmq::context_t context(1);
    zmq::socket_t controlsocket(context, ZMQ_REQ);
    //controlsocket.connect(CONTROL_URL.toStdString().c_str());
    controlsocket.connect(CONTROL_URL.toLocal8Bit());

    zmq::socket_t suscribersocket (context, ZMQ_SUB);

    //conexion con el servidor

    zmq::message_t requestmessage= messageFromQString(CONNECT_COMMAND);
    controlsocket.send(requestmessage);

    zmq::message_t replymessage;
    controlsocket.recv(&replymessage);

    QString reply=QString::fromUtf8(reinterpret_cast<char *>(replymessage.data()),replymessage.size());

    if (reply== SUCCESS_REPLY){

       // int more=0;
       // size_t more_size=sizeof(more);
        //controlsocket.getsockopt(ZMQ_RCVMORE, &more, &more_size);
        if (replymessage.more()) {
                controlsocket.recv(&replymessage);
                SUSCRIBER_PORT= QString::fromUtf8(reinterpret_cast<char *>(replymessage.data()),replymessage.size());
               if (replymessage.more()) {
                    controlsocket.recv(&replymessage);
                     SERVER_SUSCRIBER_HOST= QString::fromUtf8(reinterpret_cast<char *>(replymessage.data()),replymessage.size());
                //suscribersocket.connect(QString());
                //suscribersocket.setsockopt(ZMQ_SUBSCRIBE,"bc",0);
                //suscribersocket.setsockopt(ZMQ_SUBSCRIBE,"ab",0);
                       PUBLISHER_URL=PUBLISHER_URL.arg(SERVER_SUSCRIBER_HOST).arg(SUSCRIBER_PORT);
                suscribersocket.connect(PUBLISHER_URL.toLocal8Bit());
                suscribersocket.setsockopt(ZMQ_SUBSCRIBE,"",0);

               } else  {
                   qDebug() << "Error del protocolo en la respuesta a conectar : Se esperaba direccion servidor publicador";
           }

        } else {
                qDebug() << "Error del protocolo en la respuesta a conectar: Se esperaba numero de puerto";
        }

    } else {
        qDebug() << "Error Conectando CONNECT_COMMAND a : " << CONTROL_URL;

    }

    QTime timer;
    timer.start();
      //publicar URL
    while (true){

        if (suscribersocket.recv(&replymessage,ZMQ_DONTWAIT)) //no bloqueante
                 qDebug() << QString::fromUtf8(reinterpret_cast<char *>(replymessage.data()),replymessage.size());;

        if(timer.elapsed()>5000) {

            zmq::message_t requestmessage= messageFromQString(PUBLISH_COMMAND);
            controlsocket.send(requestmessage,ZMQ_SNDMORE);
            zmq::message_t urlmessage= messageFromQString(QString("http://miservidor.com"));
            controlsocket.send(urlmessage,0);

            zmq::message_t replymessage;
            controlsocket.recv(&replymessage); //bloqueante

            //suscribersocket.recv(&replymessage); //bloqueante

    timer.restart();
        }


    }
}
