#ifndef HORIZONTALLEVEL_H
#define HORIZONTALLEVEL_H

#include <QObject>
#include <QString>

class HorizontalLevel : public QObject
{
    Q_OBJECT
public:
    explicit HorizontalLevel(QObject *parent = 0);


    double level() const { return mLevel; }
    QString label() const { return mLabel; }

    void  setLevel(double);
    void  setLabel(const QString &label);

signals:
    void horizontalLevelChanged();
public slots:


private:
    double mLevel;
    QString mLabel;
};

#endif // HORIZONTALLEVEL_H
