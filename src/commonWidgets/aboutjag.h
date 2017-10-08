#ifndef ABOUTJAG_H
#define ABOUTJAG_H

#include <QDialog>

namespace Ui {
class AboutJag;
}


/*!
 \brief Represents information about JaG, version history and license.

*/
class AboutJag : public QDialog
{
    Q_OBJECT

public:
    explicit AboutJag(QWidget *parent = 0);
    ~AboutJag();

private:
    Ui::AboutJag *ui;
};

#endif // ABOUTJAG_H
