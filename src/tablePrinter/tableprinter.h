#ifndef TABLEPRINTER_H
#define TABLEPRINTER_H

#include "core/graphcontainer.h"


class TablePrinter: public GraphContainer
{
public:
    TablePrinter();
    virtual ~TablePrinter() { }

    virtual GraphModel* addNewGraph(const QVector<double> &x, const QVector<double> &y, const QString &name);
    virtual void addNewEvent(const TimeEvent *command);

    void saveData();

private:
    QVector<QVector<double>> mXDataVector;
    QVector<QVector<double>> mYDataVector;
    QVector<QString> mNameVector;


    void writeDataToTable(QString fileName);
    QString mEmptyDataString;
};

#endif // TABLEPRINTER_H
