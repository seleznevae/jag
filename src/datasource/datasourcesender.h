#ifndef DATASOURCESENDER_H
#define DATASOURCESENDER_H

#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>

class DataSourceSender : public QObject
{
    Q_OBJECT
public:
    explicit DataSourceSender(QVector<QString> fileNameVector, QObject *parent = 0);

signals:

public slots:
    void slotConnected();
    void slotReadServer();
private:
    QTcpSocket *mSenderSocket;
    QVector<QString> mFileNameVector;
};

#endif // DATASOURCESENDER_H
