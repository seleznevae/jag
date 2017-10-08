#include "event/eventsource.h"
#include "basic/basic.h"
#include "basic/bassic.h"

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <QStringList>
#include <QDebug>
#include <QTextCodec>
#include <iostream>


QStringList EventSource::sAliasesList;

using namespace std;
using boost::algorithm::trim;

EventSource::EventSource()
{

}

EventSource::EventSource(const QString &fileName)
{

    setName(fileName);


//    fIn.enable_terminate_on_blank_line(false);

    //encoding determing
    QTextCodec *textCodec;
    std::ifstream fTextCodec;
    fTextCodec.open(fileName.toLocal8Bit().constData(), ios_base::in); //textmode
    if (!fTextCodec.is_open()) {
        errorExit("Can't open file %s\n", fileName.toStdString().c_str());
    }
    std::string comment;
    std::getline(fTextCodec, comment);
    QString qComment = QString::fromStdString(comment);
    qComment = qComment.trimmed();
    textCodec = QTextCodec::codecForName ("UTF-8");
    bool passFirstLine = false;
    if (qComment.startsWith("#")) {
        passFirstLine = true;
        int index = qComment.indexOf("encoding=", 0, Qt::CaseInsensitive);
        if (index != -1) {
            qComment.remove(0, index + QString("encoding=").size());
            textCodec = QTextCodec::codecForName(qComment.toStdString().c_str());
        }
    }
    fTextCodec.close();





    std::string mFileName = fileName.toLocal8Bit().constData();
    char delimeter = ',';
    fIn.open(mFileName.c_str());
    if (!fIn.is_open()) {
        errorExit("Can't open file %s\n", mFileName.c_str());
    }


    fIn.set_delimiter(delimeter, "##");
    double commandTime=0.0;
    QString commandName;
    QString commandDescription;
    fIn.enable_terminate_on_blank_line(false);
    while ( fIn.read_line()) {
        if (passFirstLine) {
            passFirstLine = false;
            continue;
        }
        for (size_t i = 0; i < 3; ++i) {
            std::string dataString = fIn.get_delimited_str();
            //trimming spaces to provide correct treatment

            trim(dataString);
            if (i == 0) {
                if ( dataString == "") {
                    errorExit("Unexpected situation in %s in simple_director", __FUNCTION__);
                }
//                commandName = QString::fromStdString(dataString);
                QByteArray byteArray(dataString.c_str());
                commandName = textCodec->toUnicode(byteArray);
            } else if (i == 1) {
                if ( dataString == "") {
                    errorExit("Unexpected situation in %s in simple_director", __FUNCTION__);
                }
                commandTime = boost::lexical_cast<double>(dataString);
            } else if (i == 2) {
                QByteArray byteArray(dataString.c_str());
                commandDescription = textCodec->toUnicode(byteArray);

            }
        }
        TimeEvent *com = new TimeEvent();
        com->setName(commandName);
        com->setEventTime(commandTime);
        com->setDescription(commandDescription);
        mCommandVector.push_back(com);
    }

    fIn.close();
    emit eventSourceChanged();

}

EventSource::~EventSource()
{
    for (auto command : mCommandVector) {
        delete command;
    }

}

void EventSource::setName(const QString &newName)
{
    mName = newName;
    mAlias = stripDirectoryFromFileName(mName);
    if (sAliasesList.indexOf(mAlias) != -1) {
        int index = 1;
        while (sAliasesList.indexOf(createIndexedFileName(mAlias, index)) != -1) {
            ++index;
        }
        mAlias = createIndexedFileName(mAlias, index);
    }
    sAliasesList << mAlias;
}

void EventSource::setAlias(const QString &alias)
{
    mAlias = alias;
}

TimeEvent EventSource::getEventByIndex(int index) const
{
    return *mCommandVector[index];
}

QString EventSource::getDescriptionByIndex(int index) const
{
    return mCommandVector[index]->description();
}

QVector<QString> EventSource::getEventList() const
{
    QVector<QString> ret;
    for (const auto & item : mCommandVector) {
        ret.push_back(item->name());
    }
    return ret;
}

void EventSource::addEvent(const TimeEvent *newEvent)
{
    TimeEvent *event = new TimeEvent();
    event->setName(newEvent->name());
    event->setEventTime(newEvent->eventTime());
    event->setDescription(newEvent->description());
    mCommandVector.push_back(event);

    emit eventSourceChanged();
}

bool EventSource::isDataReady() const
{
    return true;
}

int EventSource::getMaxNameLength() const
{
    int maxLength = 0;
    for (const auto & command : mCommandVector) {
        if (maxLength < command->name().size())
            maxLength = command->name().size();
    }
    return maxLength;
}

int EventSource::progress() const
{
    return 75;
}

void EventSource::saveAs(const QString &outputFileName)
{
    // this function was not checked
    ofstream fout;
    fout.open(outputFileName.toLocal8Bit().constData(), std::fstream::out);
    if (!fout) {
        showCriticalMessage("Can't save " + outputFileName);
        return;
    }

    fout << "#encoding=UTF-8\n";
    QVector<QString> eventList = getEventList();
    for (int i = 0; i < eventList.size(); ++i) {
        TimeEvent event = getEventByIndex(i);
        fout << event.name().toUtf8().data() << " , " << event.eventTime() << " , " << event.description().toUtf8().data() << '\n';
    }

    fout.close();



}
