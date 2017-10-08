#include "fastsecprocrequestvectoreditor.h"
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QScrollArea>

FastSecProcRequestVectorEditor::FastSecProcRequestVectorEditor(QWidget *parent)
    : QWidget(parent), mGridLayout(nullptr)
{
    QVBoxLayout *mainVLayout = new QVBoxLayout;
    setLayout(mainVLayout);

    QScrollArea *scrollArea = new QScrollArea;
    scrollArea->setWidgetResizable(true);
    mainVLayout->addWidget(scrollArea);
    QWidget *scrollableWidget = new QWidget;
    scrollArea->setWidget(scrollableWidget);

    mGridLayout = new QGridLayout;
    scrollableWidget->setLayout(mGridLayout);
    mGridLayout->setSpacing(0);
    mGridLayout->setMargin(0);

    QLabel *leftLabel = new QLabel("<p align=\"center\"><b>Function string</b></p>");
    leftLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    mGridLayout->addWidget(leftLabel, 0, 0);
    QLabel *rightLabel = new QLabel("<p align=\"center\"><b>Location of new graphs</b></p>");
    rightLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    mGridLayout->addWidget(rightLabel, 0 , 1);

    bottom0 = new QWidget;
    bottom0->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    mGridLayout->addWidget(bottom0, 1 , 0);
    bottom1 = new QWidget;
    bottom1->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    mGridLayout->addWidget(bottom0, 1 , 1);
    bottom2 = new QWidget;
    bottom2->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    mGridLayout->addWidget(bottom0, 1 , 2);
//    mainVLayout->addStretch();

    QPushButton *addButton = new QPushButton(QIcon(":/add"), "Add");
    mainVLayout->addWidget(addButton);

    connect(addButton, SIGNAL(clicked(bool)),
            this, SLOT(treatAddRequestButtonPushed()));

}

FastSecProcRequestVectorEditor::~FastSecProcRequestVectorEditor()
{

}

QVector<global::FastSecondaryProcessingRequest> FastSecProcRequestVectorEditor::fastSecProcRequests() const
{
    QVector<global::FastSecondaryProcessingRequest> ret;
    if (mLineEditVector.size() == mAnyComboBoxVector.size()) {
        for (int i = 0; i < mLineEditVector.size(); ++i) {
            ret.push_back(global::FastSecondaryProcessingRequest{mLineEditVector[i]->text(),
                                                                 mAnyComboBoxVector[i]->currentValue<global::FastSecondaryProcessingRequest::NewGraphLocation>() });
        }
    }
    return ret;
}

void FastSecProcRequestVectorEditor::setFastSecProcRequests(QVector<global::FastSecondaryProcessingRequest> requestVector)
{
    clearAll();
    for (const auto &request : requestVector) {
        addRequset(request);
    }
}

void FastSecProcRequestVectorEditor::clearAll()
{
    mGridLayout->removeWidget(bottom0);
    mGridLayout->removeWidget(bottom1);
    mGridLayout->removeWidget(bottom2);


    for (auto item : mLineEditVector) {
        mGridLayout->removeWidget(item);
        item->setParent(NULL);
        item->deleteLater();
    }
    for (auto item : mAnyComboBoxVector) {
        mGridLayout->removeWidget(item);
        item->setParent(NULL);
        item->deleteLater();
    }
    for (auto item : mKillButtonVector) {
        mGridLayout->removeWidget(item);
        item->setParent(NULL);
        item->deleteLater();
    }
    mLineEditVector.clear();
    mAnyComboBoxVector.clear();
    mKillButtonVector.clear();

    mGridLayout->addWidget(bottom0, 1 , 0);
    mGridLayout->addWidget(bottom1, 1 , 1);
    mGridLayout->addWidget(bottom2, 1 , 2);
}

void FastSecProcRequestVectorEditor::addRequset(const global::FastSecondaryProcessingRequest &request)
{
    int sz = mLineEditVector.size();
    if (mLineEditVector.size() != mAnyComboBoxVector.size()
            || mLineEditVector.size() != mKillButtonVector.size())
        return;

    mGridLayout->removeWidget(bottom0);
    mGridLayout->removeWidget(bottom1);
    mGridLayout->removeWidget(bottom2);
    QLineEdit* lineEdit = new QLineEdit(request.mFunctionString);
    AnyComboBox* anyComboBox = new AnyComboBox;
    anyComboBox->addItem("Add to the current plot frame",  global::FastSecondaryProcessingRequest::NewGraphLocation::CurrentPlotFrame);
    anyComboBox->addItem("Add to the new right plot frame",  global::FastSecondaryProcessingRequest::NewGraphLocation::NewRightPlotFrame);
    anyComboBox->addItem("dd to the new bottom plot frame",  global::FastSecondaryProcessingRequest::NewGraphLocation::NewBottomPlotFrame);
    anyComboBox->setCurrentValue(request.mNewGraphLocation);
    QPushButton *killButton = new QPushButton(QIcon(":/remove"), "");
    connect(killButton, SIGNAL(clicked(bool)),
            this, SLOT(treatKillButtonPushed()));

    mGridLayout->addWidget(lineEdit    , sz + 1, 0);
    mGridLayout->addWidget(anyComboBox , sz + 1, 1);
    mGridLayout->addWidget(killButton  , sz + 1, 2);

    mLineEditVector.push_back(lineEdit);
    mAnyComboBoxVector.push_back(anyComboBox);
    mKillButtonVector.push_back(killButton);

    mGridLayout->addWidget(bottom0, sz + 2 , 0);
    mGridLayout->addWidget(bottom1, sz + 2 , 1);
    mGridLayout->addWidget(bottom2, sz + 2 , 2);

}

void FastSecProcRequestVectorEditor::treatKillButtonPushed()
{
    QPushButton *killButton = qobject_cast<QPushButton*>(sender());
    if (killButton) {
        int index = mKillButtonVector.indexOf(killButton);
        if (index < 0 || index >= mKillButtonVector.size())
            return;


        mGridLayout->removeWidget(mLineEditVector[index]);
        mLineEditVector[index]->setParent(NULL);
        mLineEditVector[index]->deleteLater();
        mLineEditVector.remove(index);

        mGridLayout->removeWidget(mAnyComboBoxVector[index]);
        mAnyComboBoxVector[index]->setParent(NULL);
        mAnyComboBoxVector[index]->deleteLater();
        mAnyComboBoxVector.remove(index);

        mGridLayout->removeWidget(mKillButtonVector[index]);
        mKillButtonVector[index]->setParent(NULL);
        mKillButtonVector[index]->deleteLater();
        mKillButtonVector.remove(index);

    }

}

void FastSecProcRequestVectorEditor::treatAddRequestButtonPushed()
{
    addRequset(global::FastSecondaryProcessingRequest {"", global::FastSecondaryProcessingRequest::NewGraphLocation::CurrentPlotFrame});
}



