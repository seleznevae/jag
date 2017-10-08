#ifndef WIDGETWRAPPERWITHHEADER_H
#define WIDGETWRAPPERWITHHEADER_H

#include <QWidget>

namespace Ui {
class WidgetWrapperWithHeader;
}

class WidgetWrapperWithHeader : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetWrapperWithHeader(QWidget *parent = 0);
    ~WidgetWrapperWithHeader();


    void setHeaderText(const QString&);
    void setHeader(bool);
    void setExpandable(bool);
    void addWidget(QWidget*);

protected:
    void paintEvent(QPaintEvent* event);

private:
    Ui::WidgetWrapperWithHeader *ui;

    QWidget *mWidget;

private slots:
    void showExtendedDialog();
};

#endif // WIDGETWRAPPERWITHHEADER_H
