#include "secondaryprocessingdialog.h"
#include "ui_secondaryprocessingdialog.h"
#include "global/global_definitions.h"
#include "secondaryprocessing/functiondescription.h"
#include "graph/graphiconengine.h"
#include <QCompleter>

QVector<std::tuple<QString, SecondaryProcessingDialog::FunctionSubCategory, QString, QString, int>> SecondaryProcessingDialog::sFunctionDescriptionVector; //name, category, description, inserting text, deviation
QMap<SecondaryProcessingDialog::FunctionCategory, QVector<SecondaryProcessingDialog::FunctionSubCategory>> SecondaryProcessingDialog::sCategoryMap; // category, subcategory

QVector<std::tuple<SecondaryProcessingDialog::FunctionCategory, QString>> SecondaryProcessingDialog::sCategoryNameMap;
QVector<std::tuple<SecondaryProcessingDialog::FunctionSubCategory, QString>> SecondaryProcessingDialog::sSubCategoryNameMap;

SecondaryProcessingDialog::SecondaryProcessingDialog(PlotLayoutModel*model,  QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SecondaryProcessingDialog), mLayoutModel(model)
{
    ui->setupUi(this);
//    ui->functionLineEdit->setFocus();
    ui->extraFunctionsCheckBox->setChecked(global::gIsExtraSecondaryProcessingFunctionsEnabled);


    ui->functionComboBox->setFocus();

    foreach (QString prevFunction, global::gPrevSecondProcesFunctionVector) {
        ui->functionComboBox->addItem(prevFunction);
    }
    auto completer = ui->functionComboBox->completer();
    completer->setCompletionMode(QCompleter::PopupCompletion);


    ui->nanTo0ConversionCheckBox->setChecked(global::gIsAutoNanTo0ConversionEnabled);
    connect(ui->nanTo0ConversionCheckBox, SIGNAL(stateChanged(int)),
            this, SLOT(treatAutoNanTo0CheckBoxChanging()));


    if (sFunctionDescriptionVector.size() == 0) {
        initalizeStaticMembers();
    }

    connect(ui->functionGroupComboBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(fillSubClassChoiceComboBox()));
    connect(ui->functionSubGroupComboBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(fillFunctionChoiceComboBox()));


    connect(ui->functionChoiceComboBox, SIGNAL(currentIndexChanged(int)),
            this, SLOT(fillFunctionDescriptionTextBrowser()));
    connect(ui->insertInFunctionPushButton, SIGNAL(clicked()),
            this, SLOT(insertExtraFunctionInFuctionString()));
    connect(ui->extraFunctionsCheckBox, SIGNAL(clicked()),
            this, SLOT(treatExtraFunctionsEnabledChanging()));

    ui->totalRangeRadioButton->setChecked(true);
    ui->beginXRAngeLineEdit->setEnabled(false);
    ui->endXRAngeLineEdit->setEnabled(false);
    connect(ui->customRangeRadioButton, SIGNAL(toggled(bool)),
            this, SLOT(treatCustomRangeActivation(bool)));

    initializeHierarchyWidget();
    connect(ui->hierarchyTreeWidget, SIGNAL(itemClicked(QTreeWidgetItem*,int)),
            this, SLOT(treatItemClickInHierarchyWidget(QTreeWidgetItem*,int)));

    resize(global::gPreviousSecProcDialogWidth, global::gPreviousSecProcDialogHeight);


    ui->functionGroupComboBox->addItem("All");
    ui->functionGroupComboBox->addItem("Math");
    ui->functionGroupComboBox->addItem("Physics");
    ui->functionGroupComboBox->addItem("Constants");
    fillFunctionChoiceComboBox();
    fillFunctionDescriptionTextBrowser();
    treatExtraFunctionsEnabledChanging();
}

SecondaryProcessingDialog::~SecondaryProcessingDialog()
{
    global::gPreviousSecProcDialogWidth = width();
    global::gPreviousSecProcDialogHeight = height();
    delete ui;
}

QString SecondaryProcessingDialog::functionString() const
{
//    return ui->functionLineEdit->text();
    return ui->functionComboBox->lineEdit()->text();
}

QString SecondaryProcessingDialog::nameString() const
{
//    return ui->nameLineEdit->text() != "" ? ui->nameLineEdit->text() : ui->functionLineEdit->text();
    return ui->nameLineEdit->text() != "" ?
                                           ui->nameLineEdit->text() :
                ui->functionComboBox->lineEdit()->text();
}

std::tuple<double, double> SecondaryProcessingDialog::getXRange() const
{
    if (ui->totalRangeRadioButton->isChecked()) {
        return mTotalRange;
    } else if (ui->visibleRangeRadioButton->isChecked()) {
        return mVisibleRange;
    } else {
        return std::tuple<double, double>(ui->beginXRAngeLineEdit->text().toDouble(), ui->endXRAngeLineEdit->text().toDouble());
    }
}

void SecondaryProcessingDialog::enableOnlyManualRangeSetting(bool onlyManualRangeSettingEnabled)
{

    if (onlyManualRangeSettingEnabled) {
        ui->totalRangeRadioButton->setEnabled(false);
        ui->visibleRangeRadioButton->setEnabled(false);
        ui->customRangeRadioButton->setChecked(true);
    } else {
        ui->totalRangeRadioButton->setEnabled(true);
        ui->visibleRangeRadioButton->setEnabled(true);
    }

}

void SecondaryProcessingDialog::setFunctionString(QString functionString)
{
//    ui->functionLineEdit->setText(functionString);
    ui->functionComboBox->lineEdit()->setText(functionString);
}

void SecondaryProcessingDialog::setNameString(QString nameString)
{
    ui->nameLineEdit->setText(nameString);
}

void SecondaryProcessingDialog::fillFunctionChoiceComboBox()
{
//    ui->functionChoiceComboBox->clear();
//    if (ui->functionGroupComboBox->currentText() == "All") {
//        for (const auto & functionDescription : sFunctionDescriptionVector) {
//            ui->functionChoiceComboBox->addItem(std::get<0>(functionDescription));
//        }
//    } else if (ui->functionGroupComboBox->currentText() == "Math") {
//        for (const auto & functionDescription : sFunctionDescriptionVector) {
//            if (std::get<1>(functionDescription) == FunctionCategory::Math)
//                ui->functionChoiceComboBox->addItem(std::get<0>(functionDescription));
//        }
//    } else if (ui->functionGroupComboBox->currentText() == "Physics") {
//        for (const auto & functionDescription : sFunctionDescriptionVector) {
//            if (std::get<1>(functionDescription) == FunctionCategory::Physics)
//                ui->functionChoiceComboBox->addItem(std::get<0>(functionDescription));
//        }
//    }  else if (ui->functionGroupComboBox->currentText() == "Constants") {
//        for (const auto & functionDescription : sFunctionDescriptionVector) {
//            if (std::get<1>(functionDescription) == FunctionCategory::Constants)
//                ui->functionChoiceComboBox->addItem(std::get<0>(functionDescription));
//        }
//    }


    ui->functionChoiceComboBox->clear();

    FunctionSubCategory functionSubCategory = BaseMath;
    for (int i = 0; i < sSubCategoryNameMap.size(); ++i) {
        if (ui->functionSubGroupComboBox->currentText() == std::get<1>(sSubCategoryNameMap[i])) {
            functionSubCategory = std::get<0>(sSubCategoryNameMap[i]);
        }
    }

    for (const auto & functionDescription : sFunctionDescriptionVector) {
        if (std::get<1>(functionDescription) == functionSubCategory)
            ui->functionChoiceComboBox->addItem(std::get<0>(functionDescription));
    }


}

void SecondaryProcessingDialog::fillSubClassChoiceComboBox()
{
    ui->functionSubGroupComboBox->clear();

    FunctionCategory functionCategory;
    for (int i = 0; i < sCategoryNameMap.size(); ++i) {
        if (ui->functionGroupComboBox->currentText() == std::get<1>(sCategoryNameMap[i])) {
            functionCategory = std::get<0>(sCategoryNameMap[i]);
        }
    }
    QVector<FunctionSubCategory> subCategoryVector = sCategoryMap[functionCategory];

    for (const auto & tup : sSubCategoryNameMap) {
        if (subCategoryVector.indexOf(std::get<0>(tup)) != -1)
            ui->functionSubGroupComboBox->addItem(std::get<1>(tup));
    }

}

void SecondaryProcessingDialog::fillFunctionDescriptionTextBrowser()
{
    QString currentFunction = ui->functionChoiceComboBox->currentText();
    if (currentFunction != "") {
        auto iter = std::find_if(sFunctionDescriptionVector.begin(), sFunctionDescriptionVector.end(),
                                 [currentFunction](const std::tuple<QString, FunctionSubCategory, QString, QString, int>& arg)
                                    {return currentFunction == std::get<0>(arg);}
                                );
        if (iter != sFunctionDescriptionVector.end()) {
            ui->functionDescriptionTextBrowser->setHtml(std::get<2>(*iter));
        }
    }
}

void SecondaryProcessingDialog::insertExtraFunctionInFuctionString()
{
    QString currentFunction = ui->functionChoiceComboBox->currentText();
    if (currentFunction != "") {
        auto iter = std::find_if(sFunctionDescriptionVector.begin(), sFunctionDescriptionVector.end(),
                                 [currentFunction](const std::tuple<QString, FunctionSubCategory, QString, QString, int>& arg)
                                    {return currentFunction == std::get<0>(arg);}
                                );
        if (iter != sFunctionDescriptionVector.end()) {
            insertTextInFormula(std::get<3>(*iter), std::get<4>(*iter));
        }
    }
}

void SecondaryProcessingDialog::treatExtraFunctionsEnabledChanging()
{
    if (ui->extraFunctionsCheckBox->isChecked()) {
        ui->extraFunctionsGroupBox->show();
        global::gIsExtraSecondaryProcessingFunctionsEnabled = true;
    } else {
        ui->extraFunctionsGroupBox->hide();
        global::gIsExtraSecondaryProcessingFunctionsEnabled = false;
    }
}

void SecondaryProcessingDialog::setTotalRange(double begin, double end)
{
    mTotalRange = std::tuple<double, double>(begin, end);
}

void SecondaryProcessingDialog::setVisibleRange(double begin, double end)
{
    mVisibleRange = std::tuple<double, double>(begin, end);
    ui->beginXRAngeLineEdit->setText(QString::number(begin));
    ui->endXRAngeLineEdit->setText(QString::number(end));

    ui->visibleRangeRadioButton->setText(tr("Visible range (from  ") + QString::number(begin)
                                         + tr("  to  ") + QString::number(end) + ")");
}

void SecondaryProcessingDialog::treatCustomRangeActivation(bool enabled)
{
    ui->beginXRAngeLineEdit->setEnabled(enabled);
    ui->endXRAngeLineEdit->setEnabled(enabled);
}

void SecondaryProcessingDialog::treatItemClickInHierarchyWidget(QTreeWidgetItem *item, int /*column*/)
{
    int graphIndex = mGraphItemVector.indexOf(item);
    if (graphIndex != -1) {
        QString graphString = "g" + QString::number(graphIndex+1);
        insertTextInFormula(graphString, graphString.size());
    }
}

void SecondaryProcessingDialog::on_plusPushButton_clicked()
{
    insertTextInFormula("+", 1);
}

void SecondaryProcessingDialog::on_minusPushButton_clicked()
{
    insertTextInFormula("-", 1);
}

void SecondaryProcessingDialog::insertTextInFormula(QString text, int cursorDisplacement)
{
//    QString currentFormula = ui->functionLineEdit->text();
//    int cursorPos = ui->functionLineEdit->cursorPosition();
//    currentFormula.insert(cursorPos, text);
//    ui->functionLineEdit->setText(currentFormula);
//    ui->functionLineEdit->setCursorPosition(cursorPos + cursorDisplacement);
//    ui->functionLineEdit->setFocus();

    QLineEdit *lineEdit = ui->functionComboBox->lineEdit();
    QString currentFormula = lineEdit->text();
    int cursorPos = lineEdit->cursorPosition();
    currentFormula.insert(cursorPos, text);
    lineEdit->setText(currentFormula);
    lineEdit->setCursorPosition(cursorPos + cursorDisplacement);
    lineEdit->setFocus();
}

void SecondaryProcessingDialog::initalizeStaticMembers()
{
    if (sFunctionDescriptionVector.size() == 0) {
        fillFunctionDescriptionVector();
        fillCategoryMap();

        sCategoryNameMap.push_back(std::tuple<FunctionCategory, QString>(FunctionCategory::All, "All"));
        sCategoryNameMap.push_back(std::tuple<FunctionCategory, QString>(FunctionCategory::Math, "Math"));
        sCategoryNameMap.push_back(std::tuple<FunctionCategory, QString>(FunctionCategory::Physics, "Physics"));
        sCategoryNameMap.push_back(std::tuple<FunctionCategory, QString>(FunctionCategory::Constants, "Constants"));

        sSubCategoryNameMap.push_back(std::tuple<FunctionSubCategory, QString>(FunctionSubCategory::BaseMath, "BaseMath"));
        sSubCategoryNameMap.push_back(std::tuple<FunctionSubCategory, QString>(FunctionSubCategory::Calculus, "Calculus"));
        sSubCategoryNameMap.push_back(std::tuple<FunctionSubCategory, QString>(FunctionSubCategory::Filtering,"Filtering"));
        sSubCategoryNameMap.push_back(std::tuple<FunctionSubCategory, QString>(FunctionSubCategory::Interpolation, "Interpolation"));
        sSubCategoryNameMap.push_back(std::tuple<FunctionSubCategory, QString>(FunctionSubCategory::MathConstants, "MathConstants"));
        sSubCategoryNameMap.push_back(std::tuple<FunctionSubCategory, QString>(FunctionSubCategory::NaNConversions, "NaNConversions"));
        sSubCategoryNameMap.push_back(std::tuple<FunctionSubCategory, QString>(FunctionSubCategory::Rounding, "Rounding"));
        sSubCategoryNameMap.push_back(std::tuple<FunctionSubCategory, QString>(FunctionSubCategory::SignalProcessing, "Signal processing"));
        sSubCategoryNameMap.push_back(std::tuple<FunctionSubCategory, QString>(FunctionSubCategory::Trigonometric, "Trigonometric"));
        sSubCategoryNameMap.push_back(std::tuple<FunctionSubCategory, QString>(FunctionSubCategory::USAStAtm76, "USAStAtm76"));


    }
}

void SecondaryProcessingDialog::fillFunctionDescriptionVector()
{
    if (sFunctionDescriptionVector.size() == 0) {

        sFunctionDescriptionVector.push_back(std::tuple<QString, FunctionSubCategory, QString, QString, int>
                                                (
                                                 "abs",
                                                 FunctionSubCategory::BaseMath,

                                                 "<h3>abs( arg )</h3>"
                                                 "Computes the absolute value of a floating point value arg.<br> "

                                                 "<img src=\":/func_abs\" alt=\"abs plot\" >"

                                                 "<h3>Parameters</h3>"
                                                 "<b>arg</b> 	- 	value of a floating-point"

                                                 "<h3>Return value</h3>"
                                                 "If successful, returns the absolute value of arg (|arg|). "
                                                 "The value returned is exact and does not depend on any rounding modes.<br>"


                                                 "<h3>Error handling</h3>"
                                                 "If the implementation supports IEEE floating-point arithmetic (IEC 60559),<br>"
                                                 "<ul>"
                                                 "<li>If the argument is &plusmn;0, +0 is returned</li>"
                                                 "<li>If the argument is &plusmn;&infin;, +&infin; is returned</li>"
                                                 "<li>if the argument is NaN, NaN is returned </li>"
                                                 "</ul>"

                                                 ,
                                                 "abs(  )",
                                                 5
                                                 ));

        sFunctionDescriptionVector.push_back(std::tuple<QString, FunctionSubCategory, QString, QString, int>
                                                (
                                                 "acos",
                                                 FunctionSubCategory::Trigonometric,

                                                 "<h3>acos( arg )</h3>"
                                                 "Computes the principal value of the arc cosine of arg.<br> "

                                                 "<img src=\":/func_asin_acos\" alt=\"sine plot\" >"

                                                 "<h3>Parameters</h3>"
                                                 "<b>arg</b> 	- 	value of a floating-point"

                                                 "<h3>Return value</h3>"
                                                 "If no errors occur, the arc cosine of arg (<i>arccos(arg)</i>) in the range <i>[0 ; &pi;]</i>, is returned.<br>"
                                                 "If a domain error occurs, an implementation-defined value is returned (NaN where supported).<br>"
                                                 "If a range error occurs due to underflow, the correct result (after rounding) is returned. "

                                                 "<h3>Error handling</h3>"
                                                 "Domain error occurs if arg is outside the range [-1.0; 1.0].<br>"
                                                 "If the implementation supports IEEE floating-point arithmetic (IEC 60559), "
                                                 "<ul>"
                                                 "<li>If the argument is <i>+1</i>, the value <i>+0</i> is returned.</li>"
                                                 "<li>If <i>|arg| > 1</i>, a domain error occurs and NaN is returned.</li>"
                                                 "<li>If the argument is NaN, NaN is returned. </li>"
                                                 "</ul>"

                                                 ,
                                                 "acos(  )",
                                                 6
                                                 ));

        sFunctionDescriptionVector.push_back(std::tuple<QString, FunctionSubCategory, QString, QString, int>
                                                (
                                                 "acosh",
                                                 FunctionSubCategory::Trigonometric,

                                                 "<h3>acosh( arg )</h3>"
                                                 "Computes the inverse hyperbolic cosine of arg.<br> "

                                                 "<h3>Parameters</h3>"
                                                 "<b>arg</b> 	- 	value of a floating-point"

                                                 "<img src=\":/func_asinh_acosh\" alt=\"acosh plot\" >"


                                                 "<h3>Return value</h3>"
                                                 "If no errors occur, the inverse hyperbolic cosine of arg (cosh<sup>-1</sup> (arg), or arcosh(arg)) on the interval <i>[0, +&infin;]</i>, is returned.<br>"
                                                 "If a domain error occurs, an implementation-defined value is returned (NaN where supported)."

                                                 "<h3>Error handling</h3>"
                                                 "If the argument is less than 1, a domain error occurs."
                                                 "If the implementation supports IEEE floating-point arithmetic (IEC 60559), "
                                                 "<ul>"
                                                 "<li>If the argument is less than 1, NaN is returned.</li>"
                                                 "<li>If the argument is <i>1</i>, the value <i>+0</i> is returned.</li>"
                                                 "<li>If the argument is +&infin;, +&infin; is returned.</li>"
                                                 "<li>If the argument is NaN, NaN is returned. </li>"
                                                 "</ul>"
                                                 ,
                                                 "acosh(  )",
                                                 7
                                                 ));

        sFunctionDescriptionVector.push_back(std::tuple<QString, FunctionSubCategory, QString, QString, int>
                                                (
                                                 "asin",
                                                 FunctionSubCategory::Trigonometric,

                                                 "<h3>asin( arg )</h3>"
                                                 "Computes the principal value of the arc sine of arg.<br> "

                                                 "<img src=\":/func_asin_acos\" alt=\"asin plot\" >"

                                                 "<h3>Parameters</h3>"
                                                 "<b>arg</b> 	- 	value of a floating-point"

                                                 "<h3>Return value</h3>"
                                                 "If no errors occur, the arc sine of arg (arcsin(arg)) in the range <i>[-&pi;/2 ; +&pi;/2]</i>, is returned.<br>"
                                                 "If a domain error occurs, an implementation-defined value is returned (NaN where supported).<br>"
                                                 "If a range error occurs due to underflow, the correct result (after rounding) is returned."


                                                 "<h3>Error handling</h3>"
                                                 "Domain error occurs if arg is outside the range [-1.0; 1.0].<br>"
                                                 "If the implementation supports IEEE floating-point arithmetic (IEC 60559), "
                                                 "<ul>"
                                                 "<li>If the argument is &plusmn;0, it is returned unmodified.</li>"
                                                 "<li>If <i>|arg| > 1</i>, a domain error occurs and NaN is returned.</li>"
                                                 "<li>If the argument is NaN, NaN is returned. </li>"
                                                 "</ul>"

                                                 ,
                                                 "asin(  )",
                                                 6
                                                 ));

        sFunctionDescriptionVector.push_back(std::tuple<QString, FunctionSubCategory, QString, QString, int>
                                                (
                                                 "asinh",
                                                 FunctionSubCategory::Trigonometric,

                                                 "<h3>asinh( arg )</h3>"
                                                 "Computes the inverse hyperbolic sine of arg.<br> "
                                                 "<img src=\":/func_asinh_acosh\" alt=\"asinh plot\" >"

                                                 "<h3>Parameters</h3>"
                                                 "<b>arg</b> 	- 	value of a floating-point"

                                                 "<h3>Return value</h3>"
                                                 "If no errors occur, the inverse hyperbolic sine of arg (sinh<sup>-1</sup> (arg), or arsinh(arg)) is returned.<br>"
                                                 "If a domain error occurs, an implementation-defined value is returned (NaN where supported)."

                                                 "<h3>Error handling</h3>"
                                                 "If the implementation supports IEEE floating-point arithmetic (IEC 60559), "
                                                 "<ul>"
                                                 "<li>If the argument is &plusmn;0 or &plusmn;&infin;,  it is returned unmodified.</li>"
                                                 "<li>If the argument is NaN, NaN is returned. </li>"
                                                 "</ul>"

                                                 ,
                                                 "asinh(  )",
                                                 7
                                                 ));


        sFunctionDescriptionVector.push_back(std::tuple<QString, FunctionSubCategory, QString, QString, int>
                                                (
                                                 "atan",
                                                 FunctionSubCategory::Trigonometric,

                                                 "<h3>atan( arg )</h3>"
                                                 "Computes the principal value of the arc tangent of arg.<br> "

                                                 "<h3>Parameters</h3>"
                                                 "<b>arg</b> 	- 	value of a floating-point"

                                                 "<h3>Return value</h3>"
                                                 "If no errors occur, the arc tangent of arg (arctan(arg)) in the range <i>[-&pi;/2 ; +&pi;/2]</i> radians, is returned.<br>"
                                                 "If a range error occurs due to underflow, the correct result (after rounding) is returned."

                                                 "<h3>Error handling</h3>"
                                                 "If the implementation supports IEEE floating-point arithmetic (IEC 60559), "
                                                 "<ul>"
                                                 "<li>If the argument is &plusmn;0, it is returned unmodified.</li>"
                                                 "<li>If the argument is +&infin;, +&pi;/2 is returned</li>"
                                                 "<li>If the argument is -&infin;, -&pi;/2 is returned</li>"
                                                 "<li>If the argument is NaN, NaN is returned. </li>"
                                                 "</ul>"

                                                 ,
                                                 "atan(  )",
                                                 6
                                                 ));

        sFunctionDescriptionVector.push_back(std::tuple<QString, FunctionSubCategory, QString, QString, int>
                                                (
                                                 "atan2",
                                                 FunctionSubCategory::Trigonometric,

                                                 "<h3>atan2( y, x )</h3>"
                                                 "Computes the arc tangent of y/x using the signs of arguments to determine the correct quadrant.<br> "

                                                 "<img src=\":/func_atan2\" alt=\"atan2 plot\" >"

                                                 "<h3>Parameters</h3>"
                                                 "<b>y, x</b> 	- 	values of floating-point<br>"
                                                 "<img src=\":/func_atan2_2\" alt=\"atan22 plot\" >"

                                                 "<h3>Return value</h3>"
                                                 "If no errors occur, the arc tangent of arg (arctan(y/x)) in the range <i>[-&pi; ; +&pi;]</i> radians, is returned.<br>"
                                                 "<br>If a domain error occurs, an implementation-defined value is returned (NaN where supported).</br>"
                                                 "If a range error occurs due to underflow, the correct result (after rounding) is returned."

                                                 "<h3>Error handling</h3>"
                                                 "Domain error may occur if x and y are both zero.<br>"
                                                 "If the implementation supports IEEE floating-point arithmetic (IEC 60559), "
                                                 "<ul>"
                                                 "<li>If x and y are both zero, domain error does not occur.</li>"
                                                 "<li>If x and y are both zero, range error does not occur either.</li>"
                                                 "<li>If y is zero, pole error does not occur.</li>"
                                                 "<li>If y is &plusmn;0 and x is negative or -0, &plusmn;&pi; is returned.</li>"
                                                 "<li>If y is &plusmn;0 and x is positive or +0, &plusmn;0 is returned.</li>"
                                                 "<li>If y is &plusmn;&infin; and x is finite, &plusmn;&pi;/2 is returned.</li>"
                                                 "<li>If y is &plusmn;&infin; and x is -&infin;, &plusmn;3&pi;/4 is returned.</li>"
                                                 "<li>If y is &plusmn;&infin; and x is +&infin;, &plusmn;&pi;/4 is returned.</li>"
                                                 "<li>If x is &plusmn;0 and y is negative, -&pi;/2 is returned.</li>"
                                                 "<li>If x is &plusmn;0 and y is positive, +&pi;/2 is returned.</li>"
                                                 "<li>If x is -&infin; and y is finite and positive, +&pi; is returned.</li>"
                                                 "<li>If x is -&infin; and y is finite and negative, -&pi; is returned.</li>"
                                                 "<li>If x is +&infin; and y is finite and positive, +0 is returned.</li>"
                                                 "<li>If x is +&infin; and y is finite and negative, -0 is returned.</li>"
                                                 "<li>If either x is NaN or y is NaN, NaN is returned. </li>"
                                                 "</ul>"
                                                 ,
                                                 "atan2(  ,  )",
                                                 7
                                                 ));


        sFunctionDescriptionVector.push_back(std::tuple<QString, FunctionSubCategory, QString, QString, int>
                                                (
                                                 "atanh",
                                                 FunctionSubCategory::Trigonometric,

                                                 "<h3>atanh( arg )</h3>"
                                                 "Computes the inverse hyperbolic tangent of arg.<br> "

                                                 "<h3>Parameters</h3>"
                                                 "<b>arg</b> 	- 	value of a floating-point"

                                                 "<h3>Return value</h3>"
                                                 "If no errors occur, the inverse hyperbolic tangent of arg (<i>tanh<sup>-1</sup>(arg)</i>, or <i>artanh(arg)</i>) is returned.<br>"
                                                 "If a domain error occurs, an implementation-defined value is returned (NaN where supported).<br>"
                                                 "If a pole error occurs, &plusmn;HUGE_VAL, &plusmn;HUGE_VALF, or &plusmn;HUGE_VALL is returned (with the correct sign).<br>"
                                                 "If a range error occurs due to underflow, the correct result (after rounding) is returned."

                                                 "<h3>Error handling</h3>"
                                                 "If the argument is not on the interval <i>[-1, +1]</i>, a range error occurs.<br>"
                                                 "If the argument is &plusmn;1, a pole error occurs.<br>"
                                                 "If the implementation supports IEEE floating-point arithmetic (IEC 60559), "
                                                 "<ul>"
                                                 "<li>If the argument is &plusmn;0,  it is returned unmodified.</li>"
                                                 "<li>If the argument is &plusmn;1 &plusmn;&infin; is returned.</li>"
                                                 "<li>If <i>|arg|>1</i>, NaN is returned.</li>"
                                                 "<li>If the argument is NaN, NaN is returned. </li>"
                                                 "</ul>"

                                                 "<h3>Notes</h3>"
                                                 "Although the C standard (to which C++ refers for this function) names this function "
                                                 "\"arc hyperbolic tangent\", the inverse functions of the hyperbolic functions are the"
                                                 "area functions. Their argument is the area of a hyperbolic sector, not an arc. The correct name "
                                                 "is \"inverse hyperbolic tangent\" (used by POSIX) or \"area hyperbolic tangent\"."

                                                 ,
                                                 "atanh(  )",
                                                 7
                                                 ));

        sFunctionDescriptionVector.push_back(std::tuple<QString, FunctionSubCategory, QString, QString, int>
                                                (
                                                 "bartlett",
                                                 FunctionSubCategory::SignalProcessing,

                                                 "<h3>bartlett( arg )</h3>"
                                                 "Description for bartlett window is not available yet.<br> "

                                                 "<img src=\":/func_bartlett_hamming_hann_welch\" alt=\"bartlett plot\" >"


                                                 ,
                                                 "bartlett(  )",
                                                 10
                                                 ));

        sFunctionDescriptionVector.push_back(std::tuple<QString, FunctionSubCategory, QString, QString, int>
                                                (
                                                 "cbrt",
                                                 FunctionSubCategory::BaseMath,

                                                 "<h3>cbrt( arg )</h3>"
                                                 "Computes the cubic root of arg.<br> "

                                                 "<img src=\":/func_cbrt\" alt=\"cbrt plot\" >"

                                                 "<h3>Parameters</h3>"
                                                 "<b>arg</b> 	- 	value of a floating-point"

                                                 "<h3>Return value</h3>"
                                                 "If no errors occur, the cubic root of arg, is returned.<br>"
                                                 "If a range error occurs due to underflow, the correct result (after rounding) is returned.<br>"

                                                 "<h3>Error handling</h3>"
                                                 "If the implementation supports IEEE floating-point arithmetic (IEC 60559), "
                                                 "<ul>"
                                                 "<li>If the argument is &plusmn;0 or &plusmn;&infin;  it is returned unchanged.</li>"
                                                 "<li>If the argument is NaN, NaN is returned. </li>"
                                                 "</ul>"

                                                 "<h3>Notes</h3>"
                                                 "std::cbrt(arg) is not equivalent to std::pow(arg, 1.0/3) because std::pow cannot raise a negative base to a fractional exponent."

                                                 ,
                                                 "cbrt(  )",
                                                 6
                                                 ));

        sFunctionDescriptionVector.push_back(std::tuple<QString, FunctionSubCategory, QString, QString, int>
                                                (
                                                 "ceil",
                                                 FunctionSubCategory::Rounding,

                                                 "<h3>ceil( arg )</h3>"
                                                 "Computes the smallest integer value not less than arg.<br> "

                                                 "<img src=\":/func_ceil\" alt=\"ceil plot\" >"

                                                 "<h3>Parameters</h3>"
                                                 "<b>arg</b> 	- 	value of a floating-point"

                                                 "<h3>Return value</h3>"
                                                 "If no errors occur, the smallest integer value not less than arg is returned.<br>"

                                                 "<h3>Error handling</h3>"
                                                 "<ul>"
                                                 "<li>The current <b>rounding mode</b> has no effect.</li>"
                                                 "<li>If arg is &plusmn;&infin;, it is returned unmodified.</li>"
                                                 "<li>If arg is &plusmn;0, it is returned unmodified.</li>"
                                                 "<li>If the argument is NaN, NaN is returned. </li>"
                                                 "</ul>"

                                                 "<h3>Notes</h3>"
                                                 "The largest representable floating-point values are exact integers in all standard floating-point formats,"
                                                 " so this function never overflows on its own; however the result may overflow any integer type (including"
                                                 " std::intmax_t), when stored in an integer variable."
                                                 ,
                                                 "ceil(  )",
                                                 6
                                                 ));

        sFunctionDescriptionVector.push_back(std::tuple<QString, FunctionSubCategory, QString, QString, int>
                                                (
                                                 "clamp",
                                                 FunctionSubCategory::BaseMath,

                                                 "<h3>clamp( arg, low, high )</h3>"
                                                 "Computes the arg clamped between a pair of boundary values. <br> "

                                                 "<img src=\":/func_clamp\" alt=\"clamp plot\" >"

                                                 "<h3>Parameters</h3>"
                                                 "<b>arg</b> 	- 	argument (value of a floating-point)<br>"
                                                 "<b>low</b> 	- 	low boundary (value of a floating-point)<br>"
                                                 "<b>high</b> 	- 	high boundary (value of a floating-point)"

                                                 "<h3>Return value</h3>"
                                                 "If argument <i>&gt; high</i> returns <i>high</i>. If argument <i>&lt; low</i> returns <i>low</i>. "
                                                 "Otherwise returns argument unmodified.<br>"


                                                 "<h3>Notes</h3>"
                                                 "Using clamp may give unexpected results if one of the values is NaN. "
                                                 ,
                                                 "clamp(  ,  ,  )",
                                                 7
                                                 ));

        //cosine
        sFunctionDescriptionVector.push_back(std::tuple<QString, FunctionSubCategory, QString, QString, int>
                                                (
                                                 "cos",
                                                 FunctionSubCategory::Trigonometric,

                                                 "<h3>cos( arg )</h3>"
                                                 "Computes the cosine of arg (measured in radians).<br> "

                                                 "<img src=\":/func_sin_cos\" alt=\"sine plot\" >"

                                                 "<h3>Parameters</h3>"
                                                 "<b>arg</b> 	- 	value representing angle in radians"

                                                 "<h3>Return value</h3>"
                                                 "If no errors occur, the cosine of arg <i>(cos(arg))</i> in the range <i>[-1 ; +1]</i>, is returned. <br>"
                                                 "If a domain error occurs, an implementation-defined value is returned (NaN where supported)<br>"
                                                 "If a range error occurs due to underflow, the correct result (after rounding) is returned. "

                                                 "<h3>Error handling</h3>"
                                                 "If the implementation supports IEEE floating-point arithmetic (IEC 60559),<br>"
                                                 "<ul>"
                                                 "<li>if the argument is &plusmn;0, the result is 1.0  </li>"
                                                 "<li>if the argument is &plusmn;&infin;, NaN is returned</li>"
                                                 "<li>if the argument is NaN, NaN is returned </li>"
                                                 "</ul>"

                                                 ,
                                                 "cos(  )",
                                                 5
                                                 ));

        sFunctionDescriptionVector.push_back(std::tuple<QString, FunctionSubCategory, QString, QString, int>
                                                (
                                                 "cosh",
                                                 FunctionSubCategory::Trigonometric,

                                                 "<h3>cosh( arg )</h3>"
                                                 "Computes the hyperbolic cosine of arg.<br> "

                                                 "<img src=\":/func_sinh_cosh\" alt=\"cosh plot\" >"

                                                 "<h3>Parameters</h3>"
                                                 "<b>arg</b> 	- 	value of a floating-point"

                                                 "<h3>Return value</h3>"
                                                 "If no errors occur, the hyperbolic cosine of arg <i>(cosh(arg))</i>  is returned. <br>"
                                                 "If a range error due to overflow occurs, +HUGE_VAL, +HUGE_VALF, or +HUGE_VALL is returned."

                                                 "<h3>Error handling</h3>"
                                                 "If the implementation supports IEEE floating-point arithmetic (IEC 60559),<br>"
                                                 "<ul>"
                                                 "<li>if the argument is &plusmn;0, 1 is returned unmodified</li>"
                                                 "<li>if the argument is &plusmn;&infin;, +&infin; is returned unmodified</li>"
                                                 "<li>if the argument is NaN, NaN is returned </li>"
                                                 "</ul>"

                                                 "<h3>Notes</h3>"
                                                 "For the IEEE-compatible type double, if <b>|arg| > 710.5</b>, then <i>cosh(arg)</i> overflows."


                                                 ,
                                                 "cosh(  )",
                                                 6
                                                 ));

        sFunctionDescriptionVector.push_back(std::tuple<QString, FunctionSubCategory, QString, QString, int>
                                                (
                                                 "decim",
                                                 FunctionSubCategory::Filtering,

                                                 "<h3>decim( arg, n )</h3>"
                                                 "Description for decim is not available yet.<br> "
                                                 ,
                                                 "decim(  ,  )",
                                                 7
                                                 ));

        sFunctionDescriptionVector.push_back(std::tuple<QString, FunctionSubCategory, QString, QString, int>
                                                (
                                                 "delay",
                                                 FunctionSubCategory::BaseMath,

                                                 "<h3>delay( arg, del_value )</h3>"
                                                 "Returns arg with x values  delayed by <i>del_value</i><br> "

                                                 "<img src=\":/func_delay\" alt=\"delay plot\" >"

                                                 ,
                                                 "delay(  )",
                                                 7
                                                 ));

        sFunctionDescriptionVector.push_back(std::tuple<QString, FunctionSubCategory, QString, QString, int>
                                                (
                                                 "deriv",
                                                 FunctionSubCategory::Calculus,

                                                 "deriv description"
                                                 ,
                                                 "deriv(  )",
                                                 7
                                                 ));

        sFunctionDescriptionVector.push_back(std::tuple<QString, FunctionSubCategory, QString, QString, int>
                                                (
                                                 "e",
                                                 FunctionSubCategory::MathConstants,

                                                 "<h3>e = 2,718281828..</h3>"
                                                 "The number <i>e</i> is an important mathematical constant that is the base of the natural logarithm. "
                                                 "It is approximately equal to 2.71828, and is the limit of <i>(1 + 1/n)<sup>n</sup></i>"
                                                 ,
                                                 "e",
                                                 1
                                                 ));

        sFunctionDescriptionVector.push_back(std::tuple<QString, FunctionSubCategory, QString, QString, int>
                                                (
                                                 "exp",
                                                 FunctionSubCategory::BaseMath,

                                                 "<h3>exp( arg )</h3>"
                                                 "Computes the e (Euler's number, 2.7182818) raised to the given power arg.<br> "

                                                 "<img src=\":/func_exp\" alt=\"exp plot\" >"

                                                 "<h3>Parameters</h3>"
                                                 "<b>arg</b> 	- 	value of a floating-point"

                                                 "<h3>Return value</h3>"
                                                 "If no errors occur, the base-e exponential of arg (e<sup>arg</sup>) is returned.<br>"
                                                 "If a range error due to overflow occurs, +HUGE_VAL, +HUGE_VALF, or +HUGE_VALL is returned.<br>"
                                                 "If a range error occurs due to underflow, the correct result (after rounding) is returned.<br>"


                                                 "<h3>Error handling</h3>"
                                                 "If the implementation supports IEEE floating-point arithmetic (IEC 60559),<br>"
                                                 "<ul>"
                                                 "<li>if the argument is &plusmn;0, 1 is returned</li>"
                                                 "<li>if the argument is -&infin;, +0 is returned</li>"
                                                 "<li>if the argument is +&infin;, +&infin; is returned</li>"
                                                 "<li>if the argument is NaN, NaN is returned </li>"
                                                 "</ul>"

                                                 "<h3>Notes</h3>"
                                                 "For IEEE-compatible type <b>double</b>, overflow is guaranteed if <i>709.8 &lt; arg</i>, and underflow is guaranteed if <i>arg &lt; -708.4</i>"

                                                 ,
                                                 "exp(  )",
                                                 5
                                                 ));




        sFunctionDescriptionVector.push_back(std::tuple<QString, FunctionSubCategory, QString, QString, int>
                                                (
                                                 "floor",
                                                 FunctionSubCategory::Rounding,

                                                 "<h3>floor( arg )</h3>"
                                                 "Computes the largest integer value not greater than arg.<br> "

                                                 "<img src=\":/func_floor\" alt=\"floor plot\" >"

                                                 "<h3>Parameters</h3>"
                                                 "<b>arg</b> 	- 	value of a floating-point"

                                                 "<h3>Return value</h3>"
                                                 "If no errors occur, the largest integer value not greater than arg is returned.<br>"

                                                 "<h3>Error handling</h3>"
                                                 "<ul>"
                                                 "<li>The current <b>rounding mode</b> has no effect.</li>"
                                                 "<li>If arg is &plusmn;&infin;, it is returned unmodified.</li>"
                                                 "<li>If arg is &plusmn;0, it is returned unmodified.</li>"
                                                 "<li>If the argument is NaN, NaN is returned. </li>"
                                                 "</ul>"

                                                 "<h3>Notes</h3>"
                                                 "The largest representable floating-point values are exact integers in all standard floating-point formats,"
                                                 " so this function never overflows on its own; however the result may overflow any integer type (including"
                                                 " std::intmax_t), when stored in an integer variable."

                                                 ,
                                                 "floor(  )",
                                                 7
                                                 ));

        QString geoConvertersKeysDescriptions = "<ul>"
                                                "<li> 0 = reserverd</li>"
                                                "<li> 1 = from SK-42 to PZ-90.02</li>"
                                                "<li> 2 = from PZ-90.02 to SK-42</li>"
                                                "<li> 3 = from SK-95 to PZ-90.02</li>"
                                                "<li> 4 = from PZ-90.02 to SK-95</li>"
                                                "<li> 5 = from SK-42 to PZ-90</li>"
                                                "<li> 6 = from PZ-90 to SK-42</li>"
                                                "<li> 7 = from SK-95 to PZ-90</li>"
                                                "<li> 8 = from PZ-90 to SK-95</li>"
                                                "<li> 9 = from PZ-90.02 to WGS 84</li>"
                                                "<li> 10 = from WGS 84 to PZ-90.02</li>"
                                                "<li> 11 = from PZ-90 to WGS 84</li>"
                                                "<li> 12 = from WGS 84 to PZ-90</li>"
                                                "<li> 13 = from PZ-90 to PZ-90.02</li>"
                                                "<li> 14 = from PZ-90.02 to PZ-90</li>"
                                                "</ul>";

        sFunctionDescriptionVector.push_back(std::tuple<QString, FunctionSubCategory, QString, QString, int>
                                                (
                                                 "fftA",
                                                 FunctionSubCategory::SignalProcessing,

                                                 "<h3>fftA( arg )</h3>"
                                                 "Computes the amplitude of discrete Fourier transform of arg.<br> "

                                                 "<img src=\":/fftA\" alt=\"fftA plot\" >"

                                                 "<h3>Parameters</h3>"
                                                 "<b>arg</b> 	- 	value of a floating-point"

                                                 "<h3>Return value</h3>"
                                                 "If no errors occur, the amplitude of discrete Fourier transform of arg in range <i>[0, f]</i>"
                                                 " (<i>f</i> - sampling frequency) is returned.<br>"

                                                 ,
                                                 "fftA(  )",
                                                 6
                                                 ));

        sFunctionDescriptionVector.push_back(std::tuple<QString, FunctionSubCategory, QString, QString, int>
                                                (
                                                 "fftAN",
                                                 FunctionSubCategory::SignalProcessing,

                                                 "<h3>fftAN( arg )</h3>"
                                                 "Computes the normalized amplitude of discrete Fourier transform of arg.<br> "

                                                 "<img src=\":/fftAN\" alt=\"fftAN plot\" >"

                                                 "<h3>Parameters</h3>"
                                                 "<b>arg</b> 	- 	value of a floating-point"

                                                 "<h3>Return value</h3>"
                                                 "If no errors occur, the normalized amplitude of discrete Fourier transform of arg in range <i>[0, f/2]</i>"
                                                 " (<i>f</i> - sampling frequency) is returned.<br>"

                                                 ,
                                                 "fftAN(  )",
                                                 7
                                                 ));

        sFunctionDescriptionVector.push_back(std::tuple<QString, FunctionSubCategory, QString, QString, int>
                                                (
                                                 "fftES",
                                                 FunctionSubCategory::SignalProcessing,

                                                 "<h3>fftES( arg )</h3>"
                                                 "Computes the energy spectral density of arg.<br> "

                                                 "<img src=\":/fftES\" alt=\"fftES plot\" >"

                                                 "<h3>Parameters</h3>"
                                                 "<b>arg</b> 	- 	value of a floating-point"

                                                 "<h3>Return value</h3>"
                                                 "If no errors occur, the energy spectral density of arg in range <i>[0, f/2]</i>"
                                                 " (<i>f</i> - sampling frequency) is returned.<br>"

                                                 ,
                                                 "fftES(  )",
                                                 7
                                                 ));

        sFunctionDescriptionVector.push_back(std::tuple<QString, FunctionSubCategory, QString, QString, int>
                                                (
                                                 "fftI",
                                                 FunctionSubCategory::SignalProcessing,

                                                 "<h3>fftR( arg )</h3>"
                                                 "Computes the imaginary part of discrete Fourier transform of arg.<br> "

                                                 "<img src=\":/fftI\" alt=\"fftI plot\" >"

                                                 "<h3>Parameters</h3>"
                                                 "<b>arg</b> 	- 	value of a floating-point"

                                                 "<h3>Return value</h3>"
                                                 "If no errors occur, the imaginary part of discrete Fourier transform of arg in range <i>[0, f]</i>"
                                                 " (<i>f</i> - sampling frequency) is returned.<br>"

                                                 ,
                                                 "fftI(  )",
                                                 6
                                                 ));

        sFunctionDescriptionVector.push_back(std::tuple<QString, FunctionSubCategory, QString, QString, int>
                                                (
                                                 "fftPS",
                                                 FunctionSubCategory::SignalProcessing,

                                                 "<h3>fftPS( arg )</h3>"
                                                 "Computes the power spectral density of arg.<br> "

                                                 "<img src=\":/fftPS\" alt=\"fftPS plot\" >"

                                                 "<h3>Parameters</h3>"
                                                 "<b>arg</b> 	- 	value of a floating-point"

                                                 "<h3>Return value</h3>"
                                                 "If no errors occur, the power spectral density of arg in range <i>[0, f/2]</i>"
                                                 " (<i>f</i> - sampling frequency) is returned.<br>"

                                                 ,
                                                 "fftPS(  )",
                                                 7
                                                 ));

        sFunctionDescriptionVector.push_back(std::tuple<QString, FunctionSubCategory, QString, QString, int>
                                                (
                                                 "fftR",
                                                 FunctionSubCategory::SignalProcessing,

                                                 "<h3>fftR( arg )</h3>"
                                                 "Computes the real part of discrete Fourier transform of arg.<br> "

                                                 "<img src=\":/fftR\" alt=\"fftR plot\" >"

                                                 "<h3>Parameters</h3>"
                                                 "<b>arg</b> 	- 	value of a floating-point"

                                                 "<h3>Return value</h3>"
                                                 "If no errors occur, the real part of discrete Fourier transform of arg in range <i>[0, f]</i>"
                                                 " (<i>f</i> - sampling frequency) is returned.<br>"

                                                 ,
                                                 "fftR(  )",
                                                 6
                                                 ));


        sFunctionDescriptionVector.push_back(std::tuple<QString, FunctionSubCategory, QString, QString, int>
                                                (
                                                 "getBit",
                                                 FunctionSubCategory::BaseMath,

                                                 "<h3>getBit( arg, N )</h3>"
                                                 "Converts <i>arg</i> and <i>N</i> to unsigned integer and extracts <i>N</i>th bit from the bitwise representation of <i>arg</i>.<br>"

                                                 "<h3>Parameters</h3>"
                                                 "<b>arg</b> 	- 	value of a floating-point<br>"
                                                 "<b>N</b> 	- 	value of a floating-point<br>"


                                                 ,
                                                 "getBit(  ,  )",
                                                 8
                                                 ));

        sFunctionDescriptionVector.push_back(std::tuple<QString, FunctionSubCategory, QString, QString, int>
                                                (
                                                 "getNBits",
                                                 FunctionSubCategory::BaseMath,

                                                 "<h3>getNBits( arg, K, N )</h3>"
                                                 "Converts <i>arg</i>, <i>K</i> and <i>N</i> to unsigned integer and extracts <i>N</i> bits starting from position <i>K</i> from the bitwise representation of <i>arg</i>.<br>"

                                                 "<h3>Parameters</h3>"
                                                 "<b>arg</b> 	- 	value of a floating-point<br>"
                                                 "<b>K</b> 	- 	value of a floating-point<br>"
                                                 "<b>N</b> 	- 	value of a floating-point<br>"

                                                 ,
                                                 "getNBits(  ,  , )",
                                                 10
                                                 ));



        sFunctionDescriptionVector.push_back(std::tuple<QString, FunctionSubCategory, QString, QString, int>
                                                (
                                                 "hann",
                                                 FunctionSubCategory::SignalProcessing,

                                                 "<h3>hann( arg )</h3>"

                                                 "<img src=\":/func_bartlett_hamming_hann_welch\" alt=\"hann plot\" >"

                                                 ,
                                                 "hann(  )",
                                                 6
                                                 ));

        sFunctionDescriptionVector.push_back(std::tuple<QString, FunctionSubCategory, QString, QString, int>
                                                (
                                                 "hamming",
                                                 FunctionSubCategory::SignalProcessing,

                                                 "<h3>hamming( arg )</h3>"

                                                 "<img src=\":/func_bartlett_hamming_hann_welch\" alt=\"hamming plot\" >"

                                                 ,
                                                 "hamming(  )",
                                                 9
                                                 ));

         sFunctionDescriptionVector.push_back(std::tuple<QString, FunctionSubCategory, QString, QString, int>
                                                (
                                                 "heavis",
                                                 FunctionSubCategory::BaseMath,

                                                 "<h3>heavis( arg )</h3>"
                                                 "heavis description"

                                                  "<img src=\":/func_heavis\" alt=\"heavis plot\" >"

                                                 ,
                                                 "heavis(  )",
                                                 8
                                                 ));

        sFunctionDescriptionVector.push_back(std::tuple<QString, FunctionSubCategory, QString, QString, int>
                                                (
                                                 "heavis_t",
                                                 FunctionSubCategory::BaseMath,


                                                 "<h3>heavis_t( arg )</h3>"
                                                 "heavis_t description"

                                                 "<img src=\":/func_heavis\" alt=\"heavis_t plot\" >"

                                                 ,
                                                 "heavis_t(  )",
                                                 10
                                                 ));


        sFunctionDescriptionVector.push_back(std::tuple<QString, FunctionSubCategory, QString, QString, int>
                                                (
                                                 "hypot",
                                                 FunctionSubCategory::BaseMath,


                                                 "<h3>hypot( x, y )</h3>"
                                                 "Computes the square root of the sum of the squares of x and y, <u>without undue overflow "
                                                 "or underflow at intermediate stages of the computation.</u>"

                                                 "<h3>Parameters</h3>"
                                                 "<b>x, y</b> 	- 	value of a floating-point"

                                                 "<h3>Return value</h3>"
                                                 "If no errors occur, the hypotenuse of a right-angled triangle, !!! !!! x<sup>2</sup>+y<sup>2</sup> is returned.<br>"
                                                 "If a range error due to overflow occurs, +HUGE_VAL, +HUGE_VALF, or +HUGE_VALL is returned.<br>"
                                                 "If a range error due to underflow occurs, the correct result (after rounding) is returned."

                                                 "<h3>Error handling</h3>"
                                                 "If the implementation supports IEEE floating-point arithmetic (IEC 60559),<br>"
                                                 "<ul>"
                                                 "<li><b>hypot(x, y)</b>, <b>hypot(y, x)</b>, and <b>hypot(x, -y)</b> are equivalent.</li>"
                                                 "<li>If one of the arguments is &plusmn;0, hypot is equivalent to <b>fabs</b> called with the non-zero argument"
                                                 "<li>If the argument is &plusmn;&infin;, NaN is returned</li>"
                                                 "<li>If one of the arguments is &plusmn;&infin;, hypot returns +&infin; even if the other argument is NaN.</li>"
                                                 "<li>Otherwise, if any of the arguments is NaN, NaN is returned<li>"
                                                 "</ul>"

                                                 "<h3>Notes</h3>"
                                                 "POSIX specifies that underflow may only occur when both arguments are subnormal and the correct"
                                                 " result is also subnormal (this forbids naive implementations)."

                                                 ,
                                                 "hypot(  ,  )",
                                                 7
                                                 ));

        sFunctionDescriptionVector.push_back(std::tuple<QString, FunctionSubCategory, QString, QString, int>
                                                (
                                                 "integr",
                                                 FunctionSubCategory::Calculus,

                                                 "<h3>integr( arg )</h3>"
                                                 " Computes the definite integral of <i>arg = f(x)</i><br> "

                                                 "<img src=\":/func_integral\" alt=\"func_integral plot\" >"

                                                 ,
                                                 "integr(  )",
                                                 8
                                                 ));

        sFunctionDescriptionVector.push_back(std::tuple<QString, FunctionSubCategory, QString, QString, int>
                                                (
                                                 "linInterp",
                                                 FunctionSubCategory::Interpolation,

                                                 "<h3>linInterp( arg, n )</h3>"
                                                 "linInterp description is not yet available"

                                                 ,
                                                 "linInterp(  ,  )",
                                                 11
                                                 ));

        sFunctionDescriptionVector.push_back(std::tuple<QString, FunctionSubCategory, QString, QString, int>
                                                (
                                                 "log",
                                                 FunctionSubCategory::BaseMath,

                                                 "<h3>log( arg )</h3>"
                                                 " Computes the the natural (base e) logarithm of arg.<br> "

                                                 "<img src=\":/func_log\" alt=\"log plot\" >"

                                                 "<h3>Parameters</h3>"
                                                 "<b>arg</b> 	- 	value of a floating-point"

                                                 "<h3>Return value</h3>"
                                                 "If no errors occur,  the natural (base-e) logarithm of arg (<i>ln(arg)</i> or <i>log<sub>e</sub>(arg)</i>) is returned.<br>"
                                                 "If a domain error occurs, an implementation-defined value is returned (NaN where supported).<br>"
                                                 "If a pole error occurs, -HUGE_VAL, -HUGE_VALF, or -HUGE_VALL is returned.<br>"


                                                 "<h3>Error handling</h3>"
                                                 "Domain error occurs if arg is less than zero.<br>"
                                                 "Pole error may occur if arg is zero.<br>"
                                                 "If the implementation supports IEEE floating-point arithmetic (IEC 60559),<br>"
                                                 "<ul>"
                                                 "<li>If the argument is &plusmn;0, -&infin; is returned.</li>"
                                                 "<li>If the argument is 1, +0 is returned.</li>"
                                                 "<li>If the argument is negative, NaN is returned.</li>"
                                                 "<li>If the argument is +&infin;, +&infin; is returned.</li>"
                                                 "<li>if the argument is NaN, NaN is returned </li>"
                                                 "</ul>"

                                                 ,
                                                 "log(  )",
                                                 5
                                                 ));

        sFunctionDescriptionVector.push_back(std::tuple<QString, FunctionSubCategory, QString, QString, int>
                                                (
                                                 "log10",
                                                 FunctionSubCategory::BaseMath,

                                                 "<h3>log10( arg )</h3>"
                                                 " Computes the the common (base 10) logarithm of arg.<br> "

                                                 "<img src=\":/func_log10\" alt=\"log10 plot\" >"

                                                 "<h3>Parameters</h3>"
                                                 "<b>arg</b> 	- 	value of a floating-point"

                                                 "<h3>Return value</h3>"
                                                 "If no errors occur,  the natural (base-10) logarithm of arg (<i>lg(arg)</i> or <i>log<sub>10</sub>(arg)</i>) is returned.<br>"
                                                 "If a domain error occurs, an implementation-defined value is returned (NaN where supported).<br>"
                                                 "If a pole error occurs, -HUGE_VAL, -HUGE_VALF, or -HUGE_VALL is returned.<br>"


                                                 "<h3>Error handling</h3>"
                                                 "Domain error occurs if arg is less than zero.<br>"
                                                 "Pole error may occur if arg is zero.<br>"
                                                 "If the implementation supports IEEE floating-point arithmetic (IEC 60559),<br>"
                                                 "<ul>"
                                                 "<li>If the argument is &plusmn;0, -&infin; is returned.</li>"
                                                 "<li>If the argument is 1, +0 is returned.</li>"
                                                 "<li>If the argument is negative, NaN is returned.</li>"
                                                 "<li>If the argument is +&infin;, +&infin; is returned.</li>"
                                                 "<li>if the argument is NaN, NaN is returned </li>"
                                                 "</ul>"

                                                 ,
                                                 "log10(  )",
                                                 7
                                                 ));

        sFunctionDescriptionVector.push_back(std::tuple<QString, FunctionSubCategory, QString, QString, int>
                                                (
                                                 "log2",
                                                 FunctionSubCategory::BaseMath,


                                                 "<h3>log2( arg )</h3>"
                                                 " Computes the the common (base 2) logarithm of arg.<br> "

                                                 "<img src=\":/func_log2\" alt=\"log2 plot\" >"

                                                 "<h3>Parameters</h3>"
                                                 "<b>arg</b> 	- 	value of a floating-point"

                                                 "<h3>Return value</h3>"
                                                 "If no errors occur,  the natural (base-2) logarithm of arg (<i>lb(arg)</i> or <i>log<sub>2</sub>(arg)</i>) is returned.<br>"
                                                 "If a domain error occurs, an implementation-defined value is returned (NaN where supported).<br>"
                                                 "If a pole error occurs, -HUGE_VAL, -HUGE_VALF, or -HUGE_VALL is returned.<br>"


                                                 "<h3>Error handling</h3>"
                                                 "Domain error occurs if arg is less than zero.<br>"
                                                 "Pole error may occur if arg is zero.<br>"
                                                 "If the implementation supports IEEE floating-point arithmetic (IEC 60559),<br>"
                                                 "<ul>"
                                                 "<li>If the argument is &plusmn;0, -&infin; is returned.</li>"
                                                 "<li>If the argument is 1, +0 is returned.</li>"
                                                 "<li>If the argument is negative, NaN is returned.</li>"
                                                 "<li>If the argument is +&infin;, +&infin; is returned.</li>"
                                                 "<li>if the argument is NaN, NaN is returned </li>"
                                                 "</ul>"


                                                 ,
                                                 "log2(  )",
                                                 6
                                                 ));

        sFunctionDescriptionVector.push_back(std::tuple<QString, FunctionSubCategory, QString, QString, int>
                                                (
                                                 "max",
                                                 FunctionSubCategory::BaseMath,

                                                 "<h3>max( arg )</h3>"
                                                 "Computes the greatest value of arg.<br> "

                                                 "<img src=\":/func_min_mean_max\" alt=\"max plot\" >"

                                                 "<h3>Parameters</h3>"
                                                 "<b>arg</b> 	- 	value of a floating-point"

                                                 "<h3>Notes</h3>"
                                                 "Not finite data are passed by this function."

                                                 ,
                                                 "max(  )",
                                                 5
                                                 ));

        sFunctionDescriptionVector.push_back(std::tuple<QString, FunctionSubCategory, QString, QString, int>
                                                (
                                                 "mean",
                                                 FunctionSubCategory::BaseMath,

                                                 "<h3>mean( arg )</h3>"
                                                 "Computes the mean value of arg.<br> "

                                                 "<img src=\":/func_min_mean_max\" alt=\"mean plot\" >"

                                                 "<h3>Parameters</h3>"
                                                 "<b>arg</b> 	- 	value of a floating-point"


                                                 ,
                                                 "mean(  )",
                                                 6
                                                 ));

        sFunctionDescriptionVector.push_back(std::tuple<QString, FunctionSubCategory, QString, QString, int>
                                                (
                                                 "min",
                                                 FunctionSubCategory::BaseMath,

                                                 "<h3>min( arg )</h3>"
                                                 "Computes the smallest value of arg.<br> "

                                                 "<img src=\":/func_min_mean_max\" alt=\"min plot\" >"

                                                 "<h3>Parameters</h3>"
                                                 "<b>arg</b> 	- 	value of a floating-point"

                                                 "<h3>Notes</h3>"
                                                 "Not finite data are passed by this function."

                                                 ,
                                                 "max(  )",
                                                 5
                                                 ));


        sFunctionDescriptionVector.push_back(std::tuple<QString, FunctionSubCategory, QString, QString, int>
                                                (
                                                 "nanTo0",
                                                 FunctionSubCategory::NaNConversions,

                                                 "<h3>nanTo0( arg )</h3>"
                                                 "Converts all <i>NaN</i> values in <i>arg</i> to <i>0</i>.<br>"

                                                 "<h3>Parameters</h3>"
                                                 "<b>arg</b> 	- 	value of a floating-point"

                                                 ,
                                                 "nanTo0(  )",
                                                 8
                                                 ));

        sFunctionDescriptionVector.push_back(std::tuple<QString, FunctionSubCategory, QString, QString, int>
                                                (
                                                 "negToNan",
                                                 FunctionSubCategory::NaNConversions,

                                                 "<h3>nanTo0( arg )</h3>"
                                                 "Converts all values  in <i>arg</i> that <i>&lt; 0</i> to <i>NaN</i>.<br>"

                                                 "<h3>Parameters</h3>"
                                                 "<b>arg</b> 	- 	value of a floating-point"

                                                 ,
                                                 "negToNan(  )",
                                                 10
                                                 ));

        sFunctionDescriptionVector.push_back(std::tuple<QString, FunctionSubCategory, QString, QString, int>
                                                (
                                                 "paramCurve",
                                                 FunctionSubCategory::BaseMath,

                                                 "<h3>paramCurve( argY, argX )</h3>"
                                                 "Computes parametric curve argY = f(t), argX = f(t). "

                                                 "<h3>Parameters</h3>"
                                                 "<b>argY</b> 	- 	function of parameter t<br>"
                                                 "<b>argX</b> 	- 	function of parameter t<br>"
                                                 ,
                                                 "paramCurve(  ,  )",
                                                 12
                                                 ));

        sFunctionDescriptionVector.push_back(std::tuple<QString, FunctionSubCategory, QString, QString, int>
                                                (
                                                 "pi",
                                                 FunctionSubCategory::MathConstants,

                                                 "<h3>pi (&pi;) = 3.14159265359...</h3>"
                                                 "The number <i>&pi;</i> is a mathematical constant, the ratio of a circle's circumference to its diameter, commonly approximated as 3.14159."
                                                 ,
                                                 "pi",
                                                 2
                                                 ));

        sFunctionDescriptionVector.push_back(std::tuple<QString, FunctionSubCategory, QString, QString, int>
                                                (
                                                 "round",
                                                 FunctionSubCategory::Rounding,

                                                 "<h3>round( arg )</h3>"
                                                 "Computes the nearest integer value to arg (in floating-point format),"
                                                 " rounding halfway cases away from zero, regardless of the current rounding mode.<br> "

                                                 "<img src=\":/func_round\" alt=\"round plot\" >"

                                                 "<h3>Parameters</h3>"
                                                 "<b>arg</b> 	- 	value of a floating-point"

                                                 "<h3>Return value</h3>"
                                                 "If no errors occur, the nearest integer value to arg, rounding halfway cases away from zero, is returned.<br>"

                                                 "<h3>Error handling</h3>"
                                                 "<ul>"
                                                 "<li>The current <b>rounding mode</b> has no effect.</li>"
                                                 "<li>If arg is &plusmn;&infin;, it is returned unmodified.</li>"
                                                 "<li>If arg is &plusmn;0, it is returned unmodified.</li>"
                                                 "<li>If the argument is NaN, NaN is returned. </li>"
                                                 "</ul>"

                                                 "<h3>Notes</h3>"
                                                 "The largest representable floating-point values are exact integers in all standard floating-point formats,"
                                                 " so this function never overflows on its own; however the result may overflow any integer type (including"
                                                 " std::intmax_t), when stored in an integer variable."

                                                 ,
                                                 "round(  )",
                                                 7
                                                 ));



        sFunctionDescriptionVector.push_back(std::tuple<QString, FunctionSubCategory, QString, QString, int>
                                                (
                                                 "sign",
                                                 FunctionSubCategory::BaseMath,

                                                 "<h3>sign( arg )</h3>"
                                                 "Computes <i>sign</i> function of argument.<br> "

                                                 "<img src=\":/func_sign\" alt=\"sign plot\" >"

                                                 "<h3>Parameters</h3>"
                                                 "<b>arg</b> 	- 	value of a floating-point"

                                                 "<h3>Return value</h3>"
                                                 "If argument <i>&gt;0</i> returns 1. If argument <i>&lt;0</i> returns -1. "
                                                 "If argument <i>=0</i> returns 0.<br>"

                                                 "<h3>Notes</h3>"
                                                 "If the argument is not finite, it is returned unmodified."
                                                 ,
                                                 "sign(  )",
                                                 6
                                                 ));


         sFunctionDescriptionVector.push_back(std::tuple<QString, FunctionSubCategory, QString, QString, int>
                                                (
                                                 "sin",
                                                 FunctionSubCategory::Trigonometric,
                                                 "<h3>sin( arg )</h3>"
                                                 "Computes the sine of arg (measured in radians).<br> "

                                                 "<img src=\":/func_sin_cos\" alt=\"sine plot\" >"

                                                 "<h3>Parameters</h3>"
                                                 "<b>arg</b> 	- 	value representing angle in radians"

                                                 "<h3>Return value</h3>"
                                                 "If no errors occur, the sine of arg <i>(sin(arg))</i> in the range <i>[-1 ; +1]</i>, is returned. <br>"
                                                 "If a domain error occurs, an implementation-defined value is returned (NaN where supported)<br>"
                                                 "If a range error occurs due to underflow, the correct result (after rounding) is returned. "

                                                 "<h3>Error handling</h3>"
                                                 "If the implementation supports IEEE floating-point arithmetic (IEC 60559),<br>"
                                                 "<ul>"
                                                 "<li>if the argument is &plusmn;0, it is returned unmodified </li>"
                                                 "<li>if the argument is &plusmn;&infin;, NaN is returned</li>"
                                                 "<li>if the argument is NaN, NaN is returned </li>"
                                                 "</ul>"

                                                 "<h3>Notes</h3>"
                                                 "POSIX specifies that in case of underflow, arg is returned unmodified,"
                                                 "and if that is not supported, an implementation-defined value no greater than DBL_MIN, FLT_MIN, and LDBL_MIN is returned."

                                                 ,
                                                 "sin(  )",
                                                 5
                                                 ));

        sFunctionDescriptionVector.push_back(std::tuple<QString, FunctionSubCategory, QString, QString, int>
                                                (
                                                 "sinh",
                                                 FunctionSubCategory::Trigonometric,
                                                 "<h3>sinh( arg )</h3>"
                                                 "Computes the hyperbolic sine of arg.<br> "

                                                 "<img src=\":/func_sinh_cosh\" alt=\"sinh plot\" >"

                                                 "<h3>Parameters</h3>"
                                                 "<b>arg</b> 	- 	value"

                                                 "<h3>Return value</h3>"
                                                 "If no errors occur, the hyperbolic sine of arg <i>(sinh(arg))</i>  is returned. <br>"
                                                 "If a range error due to overflow occurs, &plusmn;HUGE_VAL, &plusmn;HUGE_VALF, or &plusmn;HUGE_VALL is returned. <br>"
                                                 "If a range error occurs due to underflow, the correct result (after rounding) is returned. "

                                                 "<h3>Error handling</h3>"
                                                 "If the implementation supports IEEE floating-point arithmetic (IEC 60559),<br>"
                                                 "<ul>"
                                                 "<li>if the argument is &plusmn;0 or &plusmn;&infin;, it is returned unmodified </li>"
                                                 "<li>if the argument is NaN, NaN is returned </li>"
                                                 "</ul>"

                                                 ,
                                                 "sinh(  )",
                                                 6
                                                 ));

        sFunctionDescriptionVector.push_back(std::tuple<QString, FunctionSubCategory, QString, QString, int>
                                                (
                                                 "sqrt",
                                                 FunctionSubCategory::BaseMath,

                                                 "<h3>sqrt( arg )</h3>"
                                                 "Computes the square root of arg.<br> "

                                                 "<img src=\":/func_sqrt\" alt=\"sqrt plot\" >"

                                                 "<h3>Parameters</h3>"
                                                 "<b>arg</b> 	- 	value of a floating-point"

                                                 "<h3>Return value</h3>"
                                                 "If no errors occur, the square root of arg is returned.<br>"
                                                 "If a domain error occurs, an implementation-defined value is returned (NaN where supported).<br>"
                                                 "If a range error occurs due to underflow, the correct result (after rounding) is returned.<br>"

                                                 "<h3>Error handling</h3>"
                                                 "If the implementation supports IEEE floating-point arithmetic (IEC 60559), "
                                                 "<ul>"
                                                 "<li>If the argument is less than -0, NaN is returned.</li>"
                                                 "<li>If the argument is +&infin; or &plusmn;0, it is returned, unmodified.</li>"
                                                 "<li>If the argument is NaN, NaN is returned. </li>"
                                                 "</ul>"

                                                 ,
                                                 "sqrt(  )",
                                                 6
                                                 ));

        sFunctionDescriptionVector.push_back(std::tuple<QString, FunctionSubCategory, QString, QString, int>
                                                (
                                                 "tan",
                                                 FunctionSubCategory::Trigonometric,
                                                 "<h3>tan( arg )</h3>"
                                                 "Computes the tangent of arg (measured in radians).<br> "

                                                 "<img src=\":/func_tan\" alt=\"sine plot\">"

                                                 "<h3>Parameters</h3>"
                                                 "<b>arg</b> 	- 	value representing angle in radians"

                                                 "<h3>Return value</h3>"
                                                 "If no errors occur, the tangent of arg <i>(tan(arg))</i>  is returned. <br>"
                                                 "If a domain error occurs, an implementation-defined value is returned (NaN where supported)<br>"
                                                 "If a range error occurs due to underflow, the correct result (after rounding) is returned. "

                                                 "<h3>Error handling</h3>"
                                                 "If the implementation supports IEEE floating-point arithmetic (IEC 60559),<br>"
                                                 "<ul>"
                                                 "<li>if the argument is &plusmn;0, it is returned unmodified </li>"
                                                 "<li>if the argument is &plusmn;&infin;, NaN is returned</li>"
                                                 "<li>if the argument is NaN, NaN is returned </li>"
                                                 "</ul>"

                                                 "<h3>Notes</h3>"
                                                 "The function has mathematical poles at &pi;(1/2 + n);"
                                                 "however no common floating-point representation is able to represent &pi;/2 exactly,"
                                                 "thus there is no value of the argument for which a pole error occurs. "

                                                 ,
                                                 "tan(  )",
                                                 5
                                                 ));

        sFunctionDescriptionVector.push_back(std::tuple<QString, FunctionSubCategory, QString, QString, int>
                                                (
                                                 "tanh",
                                                 FunctionSubCategory::Trigonometric,

                                                 "<h3>tanh( arg )</h3>"
                                                 "Computes the hyperbolic tangent of arg<br> "

                                                 "<img src=\":/func_tanh\" alt=\"tanh plot\" >"


                                                 "<h3>Parameters</h3>"
                                                 "<b>arg</b> 	- 	value representing angle in radians"

                                                 "<h3>Return value</h3>"
                                                 "If no errors occur, the hyperbolic tangent of arg (<i>tanh(arg)</i>) is returned.<br>"
                                                 "If a range error occurs due to underflow, the correct result (after rounding) is returned. "

                                                 "<h3>Error handling</h3>"
                                                 "If the implementation supports IEEE floating-point arithmetic (IEC 60559),<br>"
                                                 "<ul>"
                                                 "<li>If the argument is &plusmn;0, &plusmn;0 is returned.</li>"
                                                 "<li>If the argument is &plusmn;&infin;, &plusmn;1 is returned.</li>"
                                                 "<li>If the argument is NaN, NaN is returned.</li>"
                                                 "</ul>"

                                                 "<h3>Notes</h3>"
                                                 "POSIX specifies that in case of underflow, arg is returned unmodified, and if that is not "
                                                 "supported, and implementation-defined value no greater than DBL_MIN, FLT_MIN, and LDBL_MIN is returned. "

                                                 ,
                                                 "tanh(  )",
                                                 6
                                                 ));

        sFunctionDescriptionVector.push_back(std::tuple<QString, FunctionSubCategory, QString, QString, int>
                                                (
                                                 "time",
                                                 FunctionSubCategory::BaseMath,

                                                 "<h3>time( start, stop, num )</h3>"
                                                 "Returns <i>num</i> evenly spaced numbers over the interval <i>[start; stop]</i>br> "

                                                 "<img src=\":/func_time\" alt=\"time plot\" >"


                                                 ,
                                                 "time(  ,  ,  )",
                                                 6
                                                 ));

        sFunctionDescriptionVector.push_back(std::tuple<QString, FunctionSubCategory, QString, QString, int>
                                                (
                                                 "trunc",
                                                 FunctionSubCategory::Rounding,

                                                 "<h3>trunc( arg )</h3>"
                                                 "Computes the nearest integer not greater in magnitude than arg.<br> "

                                                 "<img src=\":/func_trunc\" alt=\"trunc plot\" >"

                                                 "<h3>Parameters</h3>"
                                                 "<b>arg</b> 	- 	value of a floating-point"

                                                 "<h3>Return value</h3>"
                                                 "If no errors occur, the nearest integer value not greater in magnitude than arg (in other words, arg rounded towards zero), is returned.<br>"

                                                 "<h3>Error handling</h3>"
                                                 "<ul>"
                                                 "<li>The current <b>rounding mode</b> has no effect.</li>"
                                                 "<li>If arg is &plusmn;&infin;, it is returned unmodified.</li>"
                                                 "<li>If arg is &plusmn;0, it is returned unmodified.</li>"
                                                 "<li>If the argument is NaN, NaN is returned. </li>"
                                                 "</ul>"

                                                 "<h3>Notes</h3>"
                                                 "The largest representable floating-point values are exact integers in all standard floating-point formats,"
                                                 " so this function never overflows on its own; however the result may overflow any integer type (including"
                                                 " std::intmax_t), when stored in an integer variable.<br>"
                                                 "The <b>implicit conversion</b> from floating-point to integral types also rounds towards zero,"
                                                 " but is limited to the values that can be represented by the target type."

                                                 ,
                                                 "trunc(  )",
                                                 7
                                                 ));


        sFunctionDescriptionVector.push_back(std::tuple<QString, FunctionSubCategory, QString, QString, int>
                                                (
                                                 "USAStAtm76_HToA",
                                                 FunctionSubCategory::USAStAtm76,
                                                 "USAStAtm76_HToA description"

                                                 ,
                                                 "USAStAtm76_HToA(  )",
                                                 17
                                                 ));

        sFunctionDescriptionVector.push_back(std::tuple<QString, FunctionSubCategory, QString, QString, int>
                                                (
                                                 "USAStAtm76_HToG",
                                                 FunctionSubCategory::USAStAtm76,
                                                 "USAStAtm76_HToG description"

                                                 ,
                                                 "USAStAtm76_HToG(  )",
                                                 17
                                                 ));

        sFunctionDescriptionVector.push_back(std::tuple<QString, FunctionSubCategory, QString, QString, int>
                                                (
                                                 "USAStAtm76_HToP",
                                                 FunctionSubCategory::USAStAtm76,
                                                 "USAStAtm76_HToRho description"

                                                 ,
                                                 "USAStAtm76_HToP(  )",
                                                 17
                                                 ));

        sFunctionDescriptionVector.push_back(std::tuple<QString, FunctionSubCategory, QString, QString, int>
                                                (
                                                 "USAStAtm76_HToRho",
                                                 FunctionSubCategory::USAStAtm76,
                                                 "USAStAtm76_HToRho description"

                                                 ,
                                                 "USAStAtm76_HToRho(  )",
                                                 19
                                                 ));

        sFunctionDescriptionVector.push_back(std::tuple<QString, FunctionSubCategory, QString, QString, int>
                                                (
                                                 "USAStAtm76_HToT",
                                                 FunctionSubCategory::USAStAtm76,
                                                 "USAStAtm76_HToT description"

                                                 ,
                                                 "USAStAtm76_HToT(  )",
                                                 17
                                                 ));

        sFunctionDescriptionVector.push_back(std::tuple<QString, FunctionSubCategory, QString, QString, int>
                                                (
                                                 "welch",
                                                 FunctionSubCategory::SignalProcessing,

                                                 "<h3>welch( arg )</h3>"
                                                 "Description for bartlett window is not available yet.<br> "

                                                 "<img src=\":/func_bartlett_hamming_hann_welch\" alt=\"welch plot\" >"

                                                 ,
                                                 "welch(  )",
                                                 7
                                                 ));

    }
}

void SecondaryProcessingDialog::fillCategoryMap()
{
    if (sCategoryMap.size() == 0) {
        sCategoryMap.insert(FunctionCategory::All, QVector<FunctionSubCategory>({BaseMath, Calculus, Filtering, GeoConverters, Interpolation, MathConstants, NaNConversions, Rounding, SignalProcessing, Trigonometric, USAStAtm76}));
        sCategoryMap.insert(FunctionCategory::Math, QVector<FunctionSubCategory>({BaseMath, Calculus, Filtering, Interpolation, NaNConversions, Rounding, SignalProcessing, Trigonometric}));
        sCategoryMap.insert(FunctionCategory::Physics, QVector<FunctionSubCategory>({GeoConverters, USAStAtm76}));
        sCategoryMap.insert(FunctionCategory::Constants, QVector<FunctionSubCategory>({MathConstants}));
    }
}

void SecondaryProcessingDialog::initializeHierarchyWidget()
{
    if (mLayoutModel) {

        QStringList strL;
        strL << "Plot hierarchy";
        ui->hierarchyTreeWidget->setHeaderLabels(strL);

        QTreeWidgetItem *mPlotItem = new QTreeWidgetItem((QTreeWidget*)0, QStringList(QString("Plot")));
        ui->hierarchyTreeWidget->addTopLevelItem(mPlotItem);

        //
        QVector<AxisModel *> mAxisModelVector = mLayoutModel->getAxisModelVector();
        int currGraphNumber = 0;
        for (int axisIndex = 0; axisIndex < mAxisModelVector.size(); ++axisIndex) {
            QTreeWidgetItem* newAxisItem = new QTreeWidgetItem(mPlotItem, QStringList(QString("Axis: %1").arg(QString::number(axisIndex + 1) )));
            auto graphVector = mAxisModelVector[axisIndex]->getFunctionModelVector();
            for (int graphIndex = 0; graphIndex < graphVector.size(); ++graphIndex) {
                QTreeWidgetItem* newGraphItem = new QTreeWidgetItem(newAxisItem,
                                               QStringList(QString("g%1: ").arg(QString::number(++currGraphNumber)) + graphVector[graphIndex]->getName()));

                //icon for item
                GraphIconEngine *eng = new GraphIconEngine(graphVector[graphIndex]);
                QIcon icon(eng);
                newGraphItem->setIcon (0,icon);

                mGraphItemVector.push_back(newGraphItem);
            }
        }
        ui->hierarchyTreeWidget->expandAll();
    }
}

void SecondaryProcessingDialog::on_multPushButton_clicked()
{
    insertTextInFormula("*", 1);
}

void SecondaryProcessingDialog::on_divPushButton_clicked()
{
    insertTextInFormula("/", 1);
}

void SecondaryProcessingDialog::on_parenthPushButton_clicked()
{
    insertTextInFormula("(  )", 2);
}

void SecondaryProcessingDialog::on_powPushButton_clicked()
{
    insertTextInFormula("^", 1);
}


void SecondaryProcessingDialog::on_sqrtPushButton_clicked()
{
    insertTextInFormula("sqrt(  )", 6);
}


void SecondaryProcessingDialog::on_log2PushButton_clicked()
{
    insertTextInFormula("log2(  )", 6);
}


void SecondaryProcessingDialog::on_sinPushButton_clicked()
{
    insertTextInFormula("sin(  )", 5);
}

void SecondaryProcessingDialog::on_cosPushButton_clicked()
{
    insertTextInFormula("cos(  )", 5);
}

void SecondaryProcessingDialog::on_asinPushButton_clicked()
{
    insertTextInFormula("asin(  )", 6);
}

void SecondaryProcessingDialog::on_acosPushButton_clicked()
{
    insertTextInFormula("acos(  )", 6);
}

void SecondaryProcessingDialog::on_ceilPushButton_clicked()
{
    insertTextInFormula("ceil(  )", 6);
}

void SecondaryProcessingDialog::on_floorPushButton_clicked()
{
    insertTextInFormula("floor(  )", 7);
}


void SecondaryProcessingDialog::on_derivPushButton_clicked()
{
    insertTextInFormula("deriv(  )", 7);
}

void SecondaryProcessingDialog::on_integrPushButton_clicked()
{
    insertTextInFormula("integr(  )", 8);
}

void SecondaryProcessingDialog::on_nanTo0PushButton_clicked()
{
    insertTextInFormula("nanTo0(  )", 8);
}

void SecondaryProcessingDialog::treatAutoNanTo0CheckBoxChanging()
{
    if (ui->nanTo0ConversionCheckBox->isChecked())
        global::gIsAutoNanTo0ConversionEnabled = true;
    else
        global::gIsAutoNanTo0ConversionEnabled = false;
}

void SecondaryProcessingDialog::on_atan2PushButton_clicked()
{
    insertTextInFormula("atan2(  ,  )", 7);
}

void SecondaryProcessingDialog::on_hypotPushButton_clicked()
{
    insertTextInFormula("hypot(  ,  )", 7);
}

void SecondaryProcessingDialog::on_getBitPushButton_clicked()
{
    insertTextInFormula("getBit(  ,  )", 8);
}

void SecondaryProcessingDialog::closeEvent(QCloseEvent */*event*/)
{
    global::gPreviousSecProcDialogWidth = width();
    global::gPreviousSecProcDialogHeight = height();
}



void SecondaryProcessingDialog::on_absPushButton_clicked()
{
    insertTextInFormula("abs(  )", 5);
}

void SecondaryProcessingDialog::on_maxpushButton_clicked()
{
    insertTextInFormula("max(  )", 5);
}

void SecondaryProcessingDialog::on_minPushButton_clicked()
{
    insertTextInFormula("min(  )", 5);
}

void SecondaryProcessingDialog::on_signPushButton_clicked()
{
    insertTextInFormula("sign(  )", 6);
}

void SecondaryProcessingDialog::on_timePushButton_clicked()
{
    insertTextInFormula("time(  ,  ,  )", 6);
}

void SecondaryProcessingDialog::on_fftANPushButton_clicked()
{
    insertTextInFormula("fftAN(  )", 7);
}

void SecondaryProcessingDialog::on_fftPSPushButton_clicked()
{
    insertTextInFormula("fftPS(  )", 7);
}

void SecondaryProcessingDialog::on_paramCurvePushButton_clicked()
{
    insertTextInFormula("paramCurve(  ,  )", 12);
}
