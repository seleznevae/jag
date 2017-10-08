#ifndef TIMEEVENT_H
#define TIMEEVENT_H

#include <QString>
#include <QObject>
#include <QBrush>
#include <QPen>

/*!
 \brief Object that incapsulates info about commands (events) - objects that are show in the plot with vertical line at some
 particular time.

*/
class TimeEvent:public QObject
{
    Q_OBJECT
public:

    enum BrushBasis {Left,
                     Right
                     };

    TimeEvent(QObject *parent = NULL);
    TimeEvent(const TimeEvent& command);

    QString name() const {return mName;}
    QString description() const {return mDescription;}
    double eventTime() const {return mEventTime;}
    QBrush brush() const {return mBrush; }
    BrushBasis brushBasis() const {return mBrushBasis;}
    int labelDeviation() const { return mLabelDeviation; }
    double brushDuration() const { return mBrushDuration; }
    QPen pen() const { return mPen; }
public slots:
    void setBrushBasis(BrushBasis);
    void setBrush(const QBrush&);
    void setBrushColor(QColor);
    void setName(const QString &newName);
    void setEventTime(double newEventTime);
    void setDescription(const QString &newDecription);
    void setLabelDeviation(int deviation);
    void setBrushDuration(double duration);
    void setPen(const QPen&);

signals:
    void timeEventChanged();

private:
    QString mName;
    QString mDescription;
    double mEventTime;
    QBrush mBrush;
    QPen mPen;
    BrushBasis  mBrushBasis;
    int mLabelDeviation;
    double mBrushDuration;
};

#endif // TIMEEVENT_H
