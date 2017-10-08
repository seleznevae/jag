#ifndef ANYCOMBOBOX_H
#define ANYCOMBOBOX_H

#include <QComboBox>
#include <boost/any.hpp>
#include <tuple>

class AnyComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit AnyComboBox(QWidget *parent = 0);

    void addItem(const QString&, boost::any);
    void addItem(const QIcon&, boost::any);

    template <typename T>
    void setCurrentValue(const T &value)
    {
        for (auto i = 0; i != mObjectToVariantMap.size(); i++) {
            if (typeid(T) == std::get<0>(mObjectToVariantMap[i]).type()
                    && boost::any_cast<T>(std::get<0>(mObjectToVariantMap[i])) == value) {
                setCurrentIndex(i);
                return;
            }
        }
        setCurrentIndex(-1);
    }

    template <typename T>
    T currentValue() const {
        int index = currentIndex();
        if (currentIndex() < 0 || index >= mObjectToVariantMap.size())
            return T();
        return boost::any_cast<T>(std::get<0>(mObjectToVariantMap[index]));
    }

signals:
    void valueActivated(boost::any);
private slots:
    void treatItemActivation(int);

private:
    QVector<std::tuple<boost::any, QVariant>> mObjectToVariantMap;
};

#endif // ANYCOMBOBOX_H
