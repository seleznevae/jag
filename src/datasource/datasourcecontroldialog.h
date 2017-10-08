#ifndef DATASOURCECONTROLDIALOG_H
#define DATASOURCECONTROLDIALOG_H

#include <QDialog>
#include "datasource/datasource.h"
#include "datasource/datasourcecontrolwidget.h"

namespace Ui {
class DataSourceControlDialog;
}

class DataSourceControlDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DataSourceControlDialog(QVector<DataSource*> dataSourceVector, QWidget *parent = 0);
    ~DataSourceControlDialog();


public slots:
    void finishDataSourceEditing();
    void addDataSource(DataSource*);
    void deleteDataSourceTab(DataSource*);

    void treatDataSourceDeletingRequest(int index);
    void treatDataSourceDeletingRequest(DataSource* dataSource);


private:
    Ui::DataSourceControlDialog *ui;
    QVector<DataSource*> mDataSourceVector;
//    QVector<DataSourceControlWidget*> mDataSourceWidgetVector;

//public:
//    static bool isDataSourceControlDialogActive;
};

#endif // DATASOURCECONTROLDIALOG_H
