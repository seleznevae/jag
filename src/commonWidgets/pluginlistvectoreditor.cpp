#include "pluginlistvectoreditor.h"

#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QScrollArea>
#include <QFileDialog>

PluginListVectorEditor::PluginListVectorEditor(QWidget *parent)
    :QWidget(parent), mDefaultPluginDir("."), mGridLayout(nullptr)
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

    QLabel *leftLabel = new QLabel("<p align=\"center\"><b>Path to plugin</b></p>");
    leftLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    mGridLayout->addWidget(leftLabel, 0, 0);
    QLabel *rightLabel = new QLabel("");
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

    QPushButton *addButton = new QPushButton(QIcon(":/add"), "Add");
    mainVLayout->addWidget(addButton);

    connect(addButton, SIGNAL(clicked(bool)),
            this, SLOT(treatAddNewPath()));
}

void PluginListVectorEditor::setPluginsPathList(const QStringList &pathList)
{
    clearAll();
    for (const auto &path : pathList) {
        addNewPluginPath(path);
    }
}

QStringList PluginListVectorEditor::pluginsPathList() const
{
    QStringList result;
    for (const auto item : mPathEditVector) {
        result << item->text();
    }
    return result;
}

void PluginListVectorEditor::clearAll()
{
    mGridLayout->removeWidget(bottom0);
    mGridLayout->removeWidget(bottom1);
    mGridLayout->removeWidget(bottom2);


    for (auto item : mPathEditVector) {
        mGridLayout->removeWidget(item);
        item->setParent(NULL);
        item->deleteLater();
    }
    for (auto item : mEditPathButtonVector) {
        mGridLayout->removeWidget(item);
        item->setParent(NULL);
        item->deleteLater();
    }
    for (auto item : mKillButtonVector) {
        mGridLayout->removeWidget(item);
        item->setParent(NULL);
        item->deleteLater();
    }
    mPathEditVector.clear();
    mEditPathButtonVector.clear();
    mKillButtonVector.clear();

    mGridLayout->addWidget(bottom0, 1 , 0);
    mGridLayout->addWidget(bottom1, 1 , 1);
    mGridLayout->addWidget(bottom2, 1 , 2);
}

void PluginListVectorEditor::addNewPluginPath(const QString &path)
{
    int sz = mPathEditVector.size();
    if (mPathEditVector.size() != mEditPathButtonVector.size()
            || mPathEditVector.size() != mKillButtonVector.size())
        return;

    mGridLayout->removeWidget(bottom0);
    mGridLayout->removeWidget(bottom1);
    mGridLayout->removeWidget(bottom2);

    QLineEdit* lineEdit = new QLineEdit(path);
    QPushButton *editPathButton = new QPushButton("...");
    connect(editPathButton, SIGNAL(clicked(bool)),
            this, SLOT(treatModifyExistingPath()));
    QPushButton *killButton = new QPushButton(QIcon(":/remove"), "");
    connect(killButton, SIGNAL(clicked(bool)),
            this, SLOT(treatKillButtonPushed()));

    mGridLayout->addWidget(lineEdit       , sz + 1, 0);
    mGridLayout->addWidget(editPathButton , sz + 1, 1);
    mGridLayout->addWidget(killButton     , sz + 1, 2);

    mPathEditVector.push_back(lineEdit);
    mEditPathButtonVector.push_back(editPathButton);
    mKillButtonVector.push_back(killButton);

    mGridLayout->addWidget(bottom0, sz + 2 , 0);
    mGridLayout->addWidget(bottom1, sz + 2 , 1);
    mGridLayout->addWidget(bottom2, sz + 2 , 2);
}

void PluginListVectorEditor::treatKillButtonPushed()
{
    QPushButton *killButton = qobject_cast<QPushButton*>(sender());
    if (killButton) {
        int index = mKillButtonVector.indexOf(killButton);
        if (index < 0 || index >= mKillButtonVector.size())
            return;


        mGridLayout->removeWidget(mPathEditVector[index]);
        mPathEditVector[index]->setParent(NULL);
        mPathEditVector[index]->deleteLater();
        mPathEditVector.remove(index);

        mGridLayout->removeWidget(mEditPathButtonVector[index]);
        mEditPathButtonVector[index]->setParent(NULL);
        mEditPathButtonVector[index]->deleteLater();
        mEditPathButtonVector.remove(index);

        mGridLayout->removeWidget(mKillButtonVector[index]);
        mKillButtonVector[index]->setParent(NULL);
        mKillButtonVector[index]->deleteLater();
        mKillButtonVector.remove(index);

    }
}



void PluginListVectorEditor::treatAddNewPath()
{
    QFileDialog::Option option =  QFileDialog::DontUseNativeDialog;

    QString pluginPath = QFileDialog::getOpenFileName(this, "Set plugin library",
                                 mDefaultPluginDir, "" /*tr("Lib (*.so *.dll)")*/, NULL, option);
    if (pluginPath != "") {
        addNewPluginPath(pluginPath);
    }
}

void PluginListVectorEditor::treatModifyExistingPath()
{
    QPushButton *editPathButton = qobject_cast<QPushButton*>(sender());
    int index = mEditPathButtonVector.indexOf(editPathButton);
    if (index < 0 || index >= mEditPathButtonVector.size())
        return;

    QFileDialog::Option option =  QFileDialog::DontUseNativeDialog;

    QString pluginPath = QFileDialog::getOpenFileName(this, "Set plugin library",
                                 mPathEditVector[index]->text(), "" /*tr("Lib (*.so *.dll)")*/, NULL, option);
    if (pluginPath != "") {
        mPathEditVector[index]->setText(pluginPath);
    }
}
