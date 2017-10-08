#ifndef FASTSECPROCREQUESTVECTOREDITOR_H
#define FASTSECPROCREQUESTVECTOREDITOR_H

#include <QWidget>
#include <QVector>

#include "global/global_definitions.h"
#include "commonWidgets/anycombobox.h"

class QLineEdit;
class QPushButton;
class QGridLayout;

class FastSecProcRequestVectorEditor : public QWidget
{
    Q_OBJECT
public:
    explicit FastSecProcRequestVectorEditor(QWidget *parent = 0);
    ~FastSecProcRequestVectorEditor();

    QVector<global::FastSecondaryProcessingRequest>
    fastSecProcRequests() const;

    void setFastSecProcRequests(QVector<global::FastSecondaryProcessingRequest>);
signals:

public slots:

private:
    QGridLayout *mGridLayout;
    QVector<QLineEdit*> mLineEditVector;
    QVector<AnyComboBox*> mAnyComboBoxVector;
    QVector<QPushButton*> mKillButtonVector;

    QWidget *bottom0;
    QWidget *bottom1;
    QWidget *bottom2;

    void clearAll();

    void addRequset(const global::FastSecondaryProcessingRequest &);

private slots:
    void treatKillButtonPushed();
    void treatAddRequestButtonPushed();
};

#endif // FASTSECPROCREQUESTVECTOREDITOR_H
