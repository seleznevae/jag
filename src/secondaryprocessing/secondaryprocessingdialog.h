#ifndef SECONDARYPROCESSINGDIALOG_H
#define SECONDARYPROCESSINGDIALOG_H

#include <QDialog>
#include <tuple>
#include "core/plotlayoutmodel.h"
#include <QTreeWidgetItem>

namespace Ui {
class SecondaryProcessingDialog;
}

class SecondaryProcessingDialog : public QDialog
{
    Q_OBJECT

public:
    enum FunctionCategory { All,
                            Math,
                            Physics,
                            Constants
                            };
    enum FunctionSubCategory{
                            BaseMath,
                            Calculus,
                            Filtering,
                            GeoConverters,
                            Interpolation,
                            MathConstants,
                            NaNConversions,
                            Rounding,
                            SignalProcessing,
                            Trigonometric,
                            USAStAtm76
                            };

    explicit SecondaryProcessingDialog(PlotLayoutModel*model,  QWidget *parent = 0);
    ~SecondaryProcessingDialog();

    QString functionString() const;
    QString nameString() const;
    std::tuple<double, double> getXRange() const;

    void enableOnlyManualRangeSetting(bool);
public slots:
    void setFunctionString(QString);
    void setNameString(QString);
    void fillFunctionChoiceComboBox();
    void fillSubClassChoiceComboBox();

    void fillFunctionDescriptionTextBrowser();
    void insertExtraFunctionInFuctionString();
    void treatExtraFunctionsEnabledChanging();

    void setTotalRange(double begin, double end);
    void setVisibleRange(double begin, double end);
    void treatCustomRangeActivation(bool enabled);
    void treatItemClickInHierarchyWidget(QTreeWidgetItem * item, int column);

private slots:
    void on_plusPushButton_clicked();

    void on_minusPushButton_clicked();

    void on_multPushButton_clicked();

    void on_divPushButton_clicked();

    void on_parenthPushButton_clicked();

    void on_powPushButton_clicked();

    void on_sqrtPushButton_clicked();

    void on_log2PushButton_clicked();

    void on_sinPushButton_clicked();

    void on_cosPushButton_clicked();

    void on_asinPushButton_clicked();

    void on_acosPushButton_clicked();

    void on_ceilPushButton_clicked();

    void on_floorPushButton_clicked();

    void on_derivPushButton_clicked();

    void on_integrPushButton_clicked();

    void on_nanTo0PushButton_clicked();

    void treatAutoNanTo0CheckBoxChanging();

    void on_atan2PushButton_clicked();

    void on_hypotPushButton_clicked();

    void on_getBitPushButton_clicked();

    void on_absPushButton_clicked();

    void on_maxpushButton_clicked();

    void on_minPushButton_clicked();

    void on_signPushButton_clicked();

    void on_timePushButton_clicked();

    void on_fftANPushButton_clicked();

    void on_fftPSPushButton_clicked();

    void on_paramCurvePushButton_clicked();

protected:
    virtual void closeEvent(QCloseEvent * event);
private:
    Ui::SecondaryProcessingDialog *ui;
    void insertTextInFormula(QString text, int cursorDisplacement);

    std::tuple<double, double> mTotalRange;
    std::tuple<double, double> mVisibleRange;
    PlotLayoutModel *mLayoutModel;
    QVector<QTreeWidgetItem*> mGraphItemVector;

    void initalizeStaticMembers();
    void fillFunctionDescriptionVector();
    void fillCategoryMap();

    void initializeHierarchyWidget();
    static QVector<std::tuple<QString, FunctionSubCategory, QString, QString, int>> sFunctionDescriptionVector; //name, subcategory, description, inserting text, deviation
    static QMap<FunctionCategory, QVector<FunctionSubCategory>> sCategoryMap; // category, subcategory
    static QVector<std::tuple<FunctionCategory, QString>> sCategoryNameMap;
    static QVector<std::tuple<FunctionSubCategory, QString>> sSubCategoryNameMap;
};

#endif // SECONDARYPROCESSINGDIALOG_H
