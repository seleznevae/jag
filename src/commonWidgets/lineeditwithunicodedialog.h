#ifndef LINEEDITWITHUNICODEDIALOG_H
#define LINEEDITWITHUNICODEDIALOG_H

#include <QWidget>

namespace Ui {
class LineEditWithUnicodeDialog;
}

class LineEditWithUnicodeDialog : public QWidget
{
    Q_OBJECT

public:
    explicit LineEditWithUnicodeDialog(QWidget *parent = 0);
    ~LineEditWithUnicodeDialog();

    void setFont(const QFont &);
    void clearFocus();
    QString text() const;
    QFont font() const;

public slots:
    void setText(const QString &);

signals:
    void editingFinished();
private:
    Ui::LineEditWithUnicodeDialog *ui;
    QFont mFont;
private slots:
    void showUnicodeDialog();

};

#endif // LINEEDITWITHUNICODEDIALOG_H
