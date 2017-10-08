#ifndef TEXTEDITDIALOG_H
#define TEXTEDITDIALOG_H

#include <QDialog>
#include <QVector>
#include <tuple>

namespace Ui {
class TextEditDialog;
}

class QPushButton;

class TextEditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TextEditDialog(const QFont& font, QWidget *parent = 0);
    ~TextEditDialog();

    QFont font() const;
    QString text() const;

    void setText(const QString &);
public slots:
    void setFont(const QFont& newFont);
    void setFontChangingEnable(bool);

protected:
    virtual void timerEvent(QTimerEvent * event);

private:
    Ui::TextEditDialog *ui;

    QVector<QPushButton*> mButtonVector;
    QPushButton *mCurrentButton;

    QVector<std::tuple<QString, unsigned, unsigned, int>> mUnicodeRangeInfo;  //tuple::<Range name, range start, range finish, row in grid> (range finish -  included)
    int mTimerId; //to detect double clicking on the same characters


    void setCurrentButton(QPushButton*);
private slots:
    void treatPushButtonClicked();
    void insertCurrentChar();
    void setUnicodeRange(int index); //! index - index of Unicode range in mUnicodeRangeInfo vector
};

#endif // TEXTEDITDIALOG_H
