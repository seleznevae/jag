#include "commonWidgets/fastsecondaryprocessingdialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include "commonWidgets/anycombobox.h"

FastSecondaryProcessingDialog::FastSecondaryProcessingDialog(QWidget *parent)
    :QDialog(parent), mIndexOfChosenRequest(-1)
{
    QVBoxLayout *verLayout = new QVBoxLayout;
    setLayout(verLayout);

    for (const global::FastSecondaryProcessingRequest &secProcRequest : global::gFastSecondaryProcessingVector) {
        QHBoxLayout *horLayout = new QHBoxLayout;
        QPushButton *newButton = new QPushButton(">>>");
        QLineEdit *newLineEdit = new QLineEdit(secProcRequest.mFunctionString);
        AnyComboBox *newAnyComboBox = new AnyComboBox;
        newAnyComboBox->addItem("Add to the current plot frame",  global::FastSecondaryProcessingRequest::NewGraphLocation::CurrentPlotFrame);
        newAnyComboBox->addItem("Add to the new right plot frame",  global::FastSecondaryProcessingRequest::NewGraphLocation::NewRightPlotFrame);
        newAnyComboBox->addItem("dd to the new bottom plot frame",  global::FastSecondaryProcessingRequest::NewGraphLocation::NewBottomPlotFrame);
        newAnyComboBox->setCurrentValue(secProcRequest.mNewGraphLocation);
//        QLabel *newLabel = new QLabel;
//        switch (secProcRequest.mNewGraphLocation) {
//            case global::FastSecondaryProcessingRequest::NewGraphLocation::CurrentPlotFrame:
//                newLabel->setText("Add to current plot frame");
//                break;
//            case global::FastSecondaryProcessingRequest::NewGraphLocation::NewRightPlotFrame:
//                newLabel->setText("Add to new right plot frame");
//                break;
//            case global::FastSecondaryProcessingRequest::NewGraphLocation::NewBottomPlotFrame:
//                newLabel->setText("Add to new bottom plot frame");
//                break;
//        default:
//            break;
//        }
        connect(newButton, SIGNAL(clicked(bool)),
                this, SLOT(treatPushButtonPressed()));

        mButtonVector.push_back(newButton);
        mLineEditVector.push_back(newLineEdit);
        mAnyComboBoxVector.push_back(newAnyComboBox);

        horLayout->addWidget(newButton);
        horLayout->addWidget(newLineEdit);
        horLayout->addWidget(newAnyComboBox);
        verLayout->addLayout(horLayout);
    }

    if (mButtonVector.size() > 0) {
        mButtonVector[0]->setFocus();
    }

}

FastSecondaryProcessingDialog::~FastSecondaryProcessingDialog()
{

}

global::FastSecondaryProcessingRequest FastSecondaryProcessingDialog::request() const
{
    global::FastSecondaryProcessingRequest ret;
    if (mIndexOfChosenRequest >= 0 && mIndexOfChosenRequest < mLineEditVector.size()) {
        ret.mFunctionString = mLineEditVector[mIndexOfChosenRequest]->text();
        ret.mNewGraphLocation = mAnyComboBoxVector[mIndexOfChosenRequest]->currentValue<global::FastSecondaryProcessingRequest::NewGraphLocation>();
    }
    return ret;
}

void FastSecondaryProcessingDialog::treatPushButtonPressed()
{
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if (button) {
        mIndexOfChosenRequest = mButtonVector.indexOf(button);
    }
    accept();
}

