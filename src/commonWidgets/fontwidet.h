#ifndef FONTWIDET_H
#define FONTWIDET_H

#include <QWidget>

namespace Ui {
class FontWidet;
}

class FontWidet : public QWidget
{
    Q_OBJECT

public:
    explicit FontWidet(QWidget *parent = 0);
    ~FontWidet();
    void setFont(const QFont&);

    QFont font() const;

signals:
    void fontEdited(const QFont&);

private slots:
    void treatParametersChanging();

private:
    Ui::FontWidet *ui;


    void connectAll();
    void disconnectAll();
};

#endif // FONTWIDET_H
