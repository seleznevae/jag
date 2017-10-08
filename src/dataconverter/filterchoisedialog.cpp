#include "filterchoisedialog.h"
#include "ui_filterchoisedialog.h"
#include "dataconverter/windowfilter.h"
#include "dataconverter/borderfilter.h"
#include "dataconverter/medianwindowfilter.h"
#include "dataconverter/clusterfilter.h"

FilterchoiseDialog::FilterchoiseDialog(std::shared_ptr<DataConverter> *filter, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FilterchoiseDialog), mFilter(filter)
{
    ui->setupUi(this);
//    connect(ui->okPushButton, SIGNAL(clicked()),
//            this, SLOT(ok()));
//    connect(ui->cancelPushButton, SIGNAL(clicked()),
//            this, SLOT(cancel()));

    connect(ui->okCancelButtonBox, SIGNAL(accepted()),
            this, SLOT(ok()));
    connect(ui->okCancelButtonBox, SIGNAL(rejected()),
            this, SLOT(cancel()));
}

FilterchoiseDialog::~FilterchoiseDialog()
{
    delete ui;
}

void FilterchoiseDialog::cancel()
{
    close();
}

void FilterchoiseDialog::ok()
{
    if (ui->borderFilterRadioButton->isChecked()) {
        *mFilter = std::make_shared<BorderFilter>();
    } else if (ui->windowFilterRadioButton->isChecked()) {
        *mFilter = std::make_shared<WindowFilter>();
    } else if (ui->medianWindowFilterRadioButton->isChecked()) {
        *mFilter = std::make_shared<MedianWindowFilter>();
    } else if (ui->clusterFilterRadioButton->isChecked()) {
        *mFilter = std::make_shared<ClusterFilter>();
    }
    close();
}
