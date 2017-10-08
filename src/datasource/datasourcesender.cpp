#include "datasourcesender.h"
#include "global/global_definitions.h"
#include <QCoreApplication>

DataSourceSender::DataSourceSender(QVector<QString> fileNameVector, QObject *parent) :
    QObject(parent), mFileNameVector(fileNameVector)
{
    mSenderSocket = new QTcpSocket(this);
    connect(mSenderSocket, SIGNAL(connected()), this, SLOT(slotConnected()));

    mSenderSocket->connectToHost(QHostAddress("127.0.0.1"), global::gExternalDataSourcePortNumber);

    connect(mSenderSocket, SIGNAL(readyRead()),
            this, SLOT(slotReadServer()));
}

void DataSourceSender::slotConnected()
{
    QByteArray arrBlock;
    QDataStream outStream(&arrBlock, QIODevice::WriteOnly);
    outStream.setVersion(QDataStream::Qt_4_5);
    outStream << quint16(0);
    outStream << quint16(mFileNameVector.size());
    for (int i = 0; i < mFileNameVector.size(); ++i) {
        outStream << mFileNameVector[i];
    }
    outStream.device()->seek(0);
    outStream << quint16(arrBlock.size() - sizeof(quint16));
    mSenderSocket->write(arrBlock);

//    qApp->exit(0);
}

void DataSourceSender::slotReadServer()
{
    quint16 magicNumber = 0;
    QDataStream in(mSenderSocket);
    in.setVersion(QDataStream::Qt_4_5);
    for (;;) {
        if (mSenderSocket->bytesAvailable()  < static_cast<qint64>(sizeof(quint16)))
            continue;
        in >> magicNumber;
        break;
    }

    if (magicNumber == 666) {
        qDebug() << "Valid exit code was recieved from server. Exiting successfully.";
        qApp->exit(0);
    } else {
        qCritical() << "INVALID exit code was recieved from server!";
        qApp->exit(1);
    }

//    quint16 numberOfStrings = 0;
//    for (;;) {
//          if (mClientSocket->bytesAvailable()  < blockSize)
//             continue;
//          in >> numberOfStrings;
//          for (int i = 0; i < numberOfStrings; ++i) {
//              QString dataSourceName;
//              in >> dataSourceName;
//              addDataSource(dataSourceName);
//          }
//          break;
//    }
}
