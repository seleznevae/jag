#ifndef COLORLISTCHOICEWIDGET_H
#define COLORLISTCHOICEWIDGET_H

#include <QWidget>

class QGridLayout;
class QPushButton;
class ColorChoiseWidget;

class ColorListChoiceWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ColorListChoiceWidget(QWidget *parent = 0);
    virtual ~ColorListChoiceWidget();

    QList<QColor> colorList() const;
signals:

public slots:
    void setColorList(const QList<QColor>& colorList);

protected slots:
    void addNewRandomColor();
    void treatColorDeletingRequest();
private:
    void addColorWidgetWithButton(const QColor& color);
    void clearState();
protected:
    QGridLayout *mGridLayout;
    QVector<ColorChoiseWidget*> mColorChoiceWidgetsVector;
    QVector<QPushButton*> mKillButtonsVector;
    QPushButton *mAddColorButton;

};

#endif // COLORLISTCHOICEWIDGET_H
