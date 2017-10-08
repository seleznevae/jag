#ifndef DATASOURCECONTROLWIDGET_H
#define DATASOURCECONTROLWIDGET_H

#include <QWidget>
#include <datasource/datasource.h>
#include <tuple>

namespace Ui {
class DataSourceControlWidget;
}

class DataSourceControlWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DataSourceControlWidget(QWidget *parent = 0);

    ~DataSourceControlWidget();

    void setDataSource(DataSource *dataSource);

public slots:
    void treatAliasChanging();
    void saveAs();
    void setStyleOptionsVisibility(bool visibility);

    void initiazeStyleWidgets();

    void treatLineWidthChanging();
    void treatLineStyleChanging();
    void treatLineInterpolationChanging();
    void treatScatterShapeChanging();
    void treatScatterSizeChanging();
    void treatScatterDecimationChanging();
    void treatTimeDeviationChanging();
    void treatLineColorFixing();
    void treatLineColorChanging();

protected:
    virtual void timerEvent(QTimerEvent* event);

private:
    Ui::DataSourceControlWidget *ui;
    DataSource *mDataSource;
    int mDataSourceObserverTimer;

    QVector<std::tuple<Qt::PenStyle, QString>> lineStyleMap;
    QVector<std::tuple< QCPScatterStyle::ScatterShape, QIcon>> scatterShapeMap;
    QVector<std::tuple< GraphModel::LineStyle, QString>> lineInterpolationMap;


    void connectStyleWidgets();
    void disconnectStyleWidgets();
};

#endif // DATASOURCECONTROLWIDGET_H
