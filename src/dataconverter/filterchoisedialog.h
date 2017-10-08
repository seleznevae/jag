#ifndef FILTERCHOISEDIALOG_H
#define FILTERCHOISEDIALOG_H

#include <QDialog>
#include "dataconverter.h"
#include <memory>

namespace Ui {
class FilterchoiseDialog;
}

class FilterchoiseDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FilterchoiseDialog(std::shared_ptr<DataConverter> *filter,QWidget *parent = 0);
    ~FilterchoiseDialog();

public slots:
    void cancel();
    void ok();

private:
    Ui::FilterchoiseDialog *ui;
    std::shared_ptr<DataConverter> *mFilter;
};

#endif // FILTERCHOISEDIALOG_H
