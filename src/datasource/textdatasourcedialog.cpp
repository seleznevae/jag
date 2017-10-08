#include "textdatasourcedialog.h"
#include "ui_textdatasourcedialog.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QScrollBar>
#include <QTextCodec>

static const char sDecimalMark = '.';


TextDataSourceDialog::TextDataSourceDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TextDataSourceDialog), mFieldSeparator(','), mTextCodec(QTextCodec::codecForName("Windows-1251")/*codecForCStrings()*/)
{
    ui->setupUi(this);

    ui->namesLineComboBox->addItem("Generate automatically", -1);
    ui->namesLineComboBox->addItem("Use 1st line", 0);
    ui->namesLineComboBox->addItem("Use 2nd line", 1);
    ui->namesLineComboBox->addItem("Use 3rd line", 2);

    ui->descriptionLineComboBox->addItem("No descriptions", -1);
    ui->descriptionLineComboBox->addItem("Use 1st line", 0);
    ui->descriptionLineComboBox->addItem("Use 2nd line", 1);
    ui->descriptionLineComboBox->addItem("Use 3rd line", 2);

    ui->unitsLineComboBox->addItem("No units", -1);
    ui->unitsLineComboBox->addItem("Use 1st line", 0);
    ui->unitsLineComboBox->addItem("Use 2nd line", 1);
    ui->unitsLineComboBox->addItem("Use 3rd line", 2);

    ui->namesLineComboBox->setCurrentValue(-1);
    ui->descriptionLineComboBox->setCurrentValue(-1);
    ui->unitsLineComboBox->setCurrentValue(-1);

    ui->argumentPositionComboBox->addItem("Line number");
    ui->argumentPositionComboBox->addItem("1st column");
    ui->argumentPositionComboBox->addItem("2nd column");
    ui->argumentPositionComboBox->addItem("3rd column");
    ui->argumentPositionComboBox->setCurrentIndex(1);

    for (QByteArray codec : QTextCodec::availableCodecs()) {
        ui->encodingComboBox->addItem(codec, codec);
    }
    QByteArray currentCodec = QTextCodec::codecForName("Windows-1251")/*codecForCStrings()*/ ?
                              QTextCodec::codecForName("Windows-1251")/*codecForCStrings()*/->name() :
                              "latin1";
    ui->encodingComboBox->setCurrentValue(currentCodec);


    connect(ui->namesLineComboBox, SIGNAL(valueActivated(boost::any)),
            this, SLOT(treatNameLineNumberChanging()));
    connect(ui->descriptionLineComboBox, SIGNAL(valueActivated(boost::any)),
            this, SLOT(treatDescriptionLineNumberChanging()));
    connect(ui->unitsLineComboBox, SIGNAL(valueActivated(boost::any)),
            this, SLOT(treatUnitsLineNumberChanging()));
    connect(ui->encodingComboBox, SIGNAL(valueActivated(boost::any)),
            this, SLOT(treatEncodingChanging()));
}

TextDataSourceDialog::~TextDataSourceDialog()
{
    delete ui;
}


void TextDataSourceDialog::loadFile(const QString &fileName)
{
    mFileName = fileName;
    QFile inputFile(mFileName);

    if (!inputFile.open(QIODevice::ReadOnly | QIODevice::Text))
        return;  //TODO: Add normal treatment

    ui->groupBox->setTitle(fileName + " contents");

    QTextStream in(&inputFile);
    int nLines = 0;

    QMap<QChar, int> symbolsCount;
    QStringList fileContents;
    while (!in.atEnd() && nLines < 10) {
        QString line = in.readLine();
        fileContents << line;
        ui->textBrowser->append("<font color=\"blue\"><b><i>" + QString::number(++nLines)
                                + ":</i></b></font>&nbsp;&nbsp;&nbsp;" + line);
        for (auto ch : line) {
            if (symbolsCount.find(ch) == symbolsCount.end()) {
                symbolsCount[ch] = 0;
            }
            symbolsCount[ch]++;
        }
    }
    if (!in.atEnd()) {
        ui->textBrowser->append("<font color=\"blue\"><b><i> ...</i></b></font> " );
    }
    //TODO: the stuff below is to move scrollBar to the top (doesn't seem to work), need to be fixed
    ui->textBrowser->verticalScrollBar()->setValue(ui->textBrowser->verticalScrollBar()->minimum());

    //determine separator
    auto isValidSeparator = [] (QChar ch) {
      return !ch.isLetterOrNumber() && !ch.isSpace() && (ch != sDecimalMark)
              && (ch != '-') && (ch != '+');
    };
    int separatorCounter = 0;
    for (const auto& sym : symbolsCount.keys()) {
        if (isValidSeparator(sym)) {
            if (symbolsCount[sym] > separatorCounter) {
                separatorCounter = symbolsCount[sym];
                mFieldSeparator = sym.toLatin1();
            }
        }
    }
    ui->fieldSeparatorLineEdit->setText(QString(mFieldSeparator));

    auto countPercentageOfLetters = [separator = this->mFieldSeparator] (const QString& str) {
        int totalNumber = 0;
        int numberLetters = 0;
        for (auto i = str.begin(); i != str.end(); ++i) {
            if (*i != separator && ! i->isSpace()) {
                totalNumber++;
                if (i->isLetter()) {
                    numberLetters++;
                }
            }
        }
        return totalNumber ? (double)numberLetters / totalNumber : 0;
    };


    if (fileContents.size() > 0 && countPercentageOfLetters(fileContents[0]) > 0.5) {
        ui->namesLineComboBox->setCurrentValue(0);
        if (fileContents.size() > 1 && countPercentageOfLetters(fileContents[1]) > 0.5) {
            ui->descriptionLineComboBox->setCurrentValue(1);
            if (fileContents.size() > 2 && countPercentageOfLetters(fileContents[2]) > 0.5) {
                ui->unitsLineComboBox->setCurrentValue(2);
            }
        }
    }


}

char TextDataSourceDialog::fieldSeparator() const
{
    return mFieldSeparator;
}

int TextDataSourceDialog::parameterNameLineNumber() const
{
    return ui->namesLineComboBox->currentValue<int>();
}

int TextDataSourceDialog::parameterDescriptionLineNumber() const
{
    return ui->descriptionLineComboBox->currentValue<int>();
}

int TextDataSourceDialog::parameterUnitsLineNumber() const
{
    return ui->unitsLineComboBox->currentValue<int>();
}

int TextDataSourceDialog::argumentPosition() const
{
    return ui->argumentPositionComboBox->currentIndex() - 1;
}

QTextCodec *TextDataSourceDialog::textCodec() const
{
    return mTextCodec;
}

void TextDataSourceDialog::treatFieldSeparatorChanging()
{
    QString fieldSeparatorString = ui->fieldSeparatorLineEdit->text();
    if (fieldSeparatorString.size() != 0) {
        mFieldSeparator = fieldSeparatorString[0].toLatin1();
    }
    ui->fieldSeparatorLineEdit->setText(QString(mFieldSeparator));

}

void TextDataSourceDialog::treatNameLineNumberChanging()
{
    int value = ui->namesLineComboBox->currentValue<int>();
    if (value == -1)
        return;

    if (ui->descriptionLineComboBox->currentValue<int>() == value)
        ui->descriptionLineComboBox->setCurrentValue(-1);
    if (ui->unitsLineComboBox->currentValue<int>() == value)
        ui->unitsLineComboBox->setCurrentValue(-1);
}

void TextDataSourceDialog::treatDescriptionLineNumberChanging()
{
    int value = ui->descriptionLineComboBox->currentValue<int>();
    if (value == -1)
        return;

    if (ui->namesLineComboBox->currentValue<int>() == value)
        ui->namesLineComboBox->setCurrentValue(-1);
    if (ui->unitsLineComboBox->currentValue<int>() == value)
        ui->unitsLineComboBox->setCurrentValue(-1);
}

void TextDataSourceDialog::treatUnitsLineNumberChanging()
{
    int value = ui->unitsLineComboBox->currentValue<int>();
    if (value == -1)
        return;

    if (ui->namesLineComboBox->currentValue<int>() == value)
        ui->namesLineComboBox->setCurrentValue(-1);
    if (ui->descriptionLineComboBox->currentValue<int>() == value)
        ui->descriptionLineComboBox->setCurrentValue(-1);
}

void TextDataSourceDialog::treatEncodingChanging()
{
    mTextCodec =
            QTextCodec::codecForName(ui->encodingComboBox->currentValue<QByteArray>());
    ui->encodingComboBox->clearFocus();
}

