#ifndef COMBOBOXWITHTIPS_H
#define COMBOBOXWITHTIPS_H

#include <QComboBox>



/*!
 \brief ComboBox that can additionally show some extra info about each item in combobox.

*/
class ComboBoxWithTips : public QComboBox
{
    Q_OBJECT
public:
//    template <typename... Args>
//    ComboBoxWithTips(Args&&... args)
//        :QComboBox(std::forward<Args>(args)...) {
//            setMouseTracking(true);
//            connect(this, SIGNAL(highlighted(int)),
//                    this, SLOT(showTipForItem(int)));
//    }

    ComboBoxWithTips(QWidget *PArent = NULL);
//    explicit ComboBoxWithTips(QWidget *parent = 0);

    void addItems(const QStringList & texts,  QVector<QStringList> tipVectors);
    void addItems(QMap<QString, QStringList> valuesToTipsMap);



signals:

public slots:
    void clear();
    void showTipForItem(int index);


private:
    QVector<QStringList> mTipVector;
    QPoint mMousePoint;
};

#endif // COMBOBOXWITHTIPS_H
