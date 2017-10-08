#ifndef DUMMYLINEEDIT_H
#define DUMMYLINEEDIT_H

#include <QLineEdit>

/*!
 \brief DummyLineEdit - ordinal QLineEdit that ignores all QKeyEvents

*/
class DummyLineEdit : public QLineEdit
{
    Q_OBJECT
public:

    explicit DummyLineEdit(QWidget *parent = 0);

signals:

public slots:
protected:
    /*!
     \brief keyPressEvent - overloaded virtual function that ignores all events.

     \param event
    */
    virtual void keyPressEvent(QKeyEvent *event);

};

#endif // DUMMYLINEEDIT_H
