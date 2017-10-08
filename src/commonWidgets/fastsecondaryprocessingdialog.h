#ifndef FASTSECONDARYPROCESSINGDIALOG_H
#define FASTSECONDARYPROCESSINGDIALOG_H

#include <QDialog>
#include <QVector>
#include "global/global_definitions.h"



class QPushButton;
class AnyComboBox;
class QLineEdit;

class FastSecondaryProcessingDialog : public QDialog
{
    Q_OBJECT
public:
    explicit FastSecondaryProcessingDialog(QWidget* parent = 0);

    ~FastSecondaryProcessingDialog();

    global::FastSecondaryProcessingRequest request() const;

public slots:
    void treatPushButtonPressed();
private:
    int mIndexOfChosenRequest;
    QVector<QPushButton*> mButtonVector;
    QVector<QLineEdit*> mLineEditVector;
    QVector<AnyComboBox*> mAnyComboBoxVector;
};

#endif // FASTSECONDARYPROCESSINGDIALOG_H
