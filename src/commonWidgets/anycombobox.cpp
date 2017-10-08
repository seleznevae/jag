#include "anycombobox.h"
#include "typeinfo"

using namespace boost;

AnyComboBox::AnyComboBox(QWidget *parent) :
    QComboBox(parent)
{
    connect(this, SIGNAL(activated(int)),
            this, SLOT(treatItemActivation(int)));

}

void AnyComboBox::addItem(const QString &label, any value)
{
    mObjectToVariantMap.push_back(std::tuple<any, QVariant>(value, label));
    QComboBox::addItem(label, QVariant());
}

void AnyComboBox::addItem(const QIcon &icon, any value)
{
    mObjectToVariantMap.push_back(std::tuple<any, QVariant>(value, icon));
    QComboBox::addItem(icon, "");
}

void AnyComboBox::treatItemActivation(int index)
{
//    if (currentIndex() >= 0) {
//        clearFocus();
//        if (mObjectToVariantMap.size() != 0) {
//            emit valueActivated(std::get<0>(mObjectToVariantMap[currentIndex()]));
//        }
//    }

    clearFocus();
    if (index >= 0 && index < mObjectToVariantMap.size()) {
        emit valueActivated(std::get<0>(mObjectToVariantMap[index]));
    }
}




