#ifndef COLORSLISTCHOICEWIDGET_H
#define COLORSLISTCHOICEWIDGET_H

#include <QWidget>

class QGridLayout;
class QPushButton;
class ColorChoiseWidget;
class QLabel;

class ColorsListChoiceWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ColorsListChoiceWidget(QWidget *parent = 0);
    virtual ~ColorsListChoiceWidget();

    std::tuple<QList<QColor>, QList<QColor>> colorLists() const;
    void setHeader(const QString &header0, const QString &header1);
signals:
    void colorListsChanged();

public slots:
    void setColorLists(const QList<QColor>& colorList0, const QList<QColor>& colorList1);

protected slots:
    void addNewRandomColor();
    void treatColorDeletingRequest();
private:
    void addColorWidgetWithButton(const QColor& color0, const QColor& color1);
    void clearState();
protected:
    QLabel *mHeaderLabel0;
    QLabel *mHeaderLabel1;
    QGridLayout *mGridLayout;
    QVector<ColorChoiseWidget*> mColorChoiceWidgetsVector0;
    QVector<ColorChoiseWidget*> mColorChoiceWidgetsVector1;
    QVector<QPushButton*> mKillButtonsVector;
    QPushButton *mAddColorButton;


};

#endif // COLORSLISTCHOICEWIDGET_H
