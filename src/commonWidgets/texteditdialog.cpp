#include "texteditdialog.h"
#include "ui_texteditdialog.h"
#include <QPushButton>
#include <QToolButton>
#include <QScrollBar>
#include <QDebug>

TextEditDialog::TextEditDialog(const QFont& font, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TextEditDialog), mCurrentButton(nullptr), mTimerId(0)
{
    ui->setupUi(this);

    //NOTE: Information about Unicode ranges was taken from http://jrgraphix.net/research/unicode_blocks.php
    mUnicodeRangeInfo << std::tuple<QString, unsigned, unsigned, int>(tr("Basic Latin"                 ) , 0x0020 , 0x007F, -1);
    mUnicodeRangeInfo << std::tuple<QString, unsigned, unsigned, int>(tr("Latin-1 Supplement"          ) , 0x00A0 , 0x00FF, -1);
    mUnicodeRangeInfo << std::tuple<QString, unsigned, unsigned, int>(tr("Latin Extended-A"            ) , 0x0100 , 0x017F, -1);
    mUnicodeRangeInfo << std::tuple<QString, unsigned, unsigned, int>(tr("Latin Extended-B"            ) , 0x0180 , 0x024F, -1);
    mUnicodeRangeInfo << std::tuple<QString, unsigned, unsigned, int>(tr("Spacing Modifier Letters"    ) , 0x02B0 , 0x02FF, -1);
    mUnicodeRangeInfo << std::tuple<QString, unsigned, unsigned, int>(tr("Greek and Coptic"            ) , 0x0370 , 0x03FF, -1);
    mUnicodeRangeInfo << std::tuple<QString, unsigned, unsigned, int>(tr("Cyrillic"                    ) , 0x0400 , 0x04FF, -1);
    mUnicodeRangeInfo << std::tuple<QString, unsigned, unsigned, int>(tr("Latin Extended Additional"   ) , 0x1E00 , 0x1EFF, -1);
    mUnicodeRangeInfo << std::tuple<QString, unsigned, unsigned, int>(tr("General Punctuation"         ) , 0x2000 , 0x206F, -1);
    mUnicodeRangeInfo << std::tuple<QString, unsigned, unsigned, int>(tr("Superscripts and Subscripts" ) , 0x2070 , 0x209F, -1);
    mUnicodeRangeInfo << std::tuple<QString, unsigned, unsigned, int>(tr("Superscripts and Subscripts" ) , 0x2070 , 0x209F, -1);
    mUnicodeRangeInfo << std::tuple<QString, unsigned, unsigned, int>(tr("Currency Symbols"            ) , 0x20A0 , 0x20CF, -1);
    mUnicodeRangeInfo << std::tuple<QString, unsigned, unsigned, int>(tr("Letterlike Symbols"          ) , 0x2100 , 0x214F, -1);
    mUnicodeRangeInfo << std::tuple<QString, unsigned, unsigned, int>(tr("Number Forms"                ) , 0x2150 , 0x218F, -1);
    mUnicodeRangeInfo << std::tuple<QString, unsigned, unsigned, int>(tr("Arrows"                      ) , 0x2190 , 0x21FF, -1);
    mUnicodeRangeInfo << std::tuple<QString, unsigned, unsigned, int>(tr("Mathematical Operators"      ) , 0x2200 , 0x22FF, -1);
    mUnicodeRangeInfo << std::tuple<QString, unsigned, unsigned, int>(tr("Miscellaneous Technical"     ) , 0x2300 , 0x23FF, -1);
    mUnicodeRangeInfo << std::tuple<QString, unsigned, unsigned, int>(tr("Geometric Shapes"            ) , 0x25A0 , 0x25FF, -1);
    mUnicodeRangeInfo << std::tuple<QString, unsigned, unsigned, int>(tr("Miscellaneous Symbols"       ) , 0x2600 , 0x26FF, -1);

    QWidget *w = new QWidget;
    ui->scrollArea->setWidget(w);
    QGridLayout *gridLayout = new QGridLayout();
    w->setLayout(gridLayout);

//    ui->scrollArea->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding));
//    ui->insertPushButton->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred));

//    ui->charachterTextBrowser->set

    ui->charachterTextBrowser->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    ui->horizontalLayout_2->setStretchFactor(ui->scrollArea, 0.00001);

    ui->scrollArea->setWidgetResizable(false);
    ui->scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    gridLayout->setSpacing(0);
    gridLayout->setMargin(0);
    gridLayout->setSizeConstraint(QLayout::SetFixedSize);

    int counter = 0;
    for (auto &unicodeRangeInfo : mUnicodeRangeInfo) {
        QString rangeName;
        unsigned rangeStart;
        unsigned rangeFinish;
        int row;
        std::tie(rangeName, rangeStart, rangeFinish, row) = unicodeRangeInfo;
        for (auto i = rangeStart; i <= rangeFinish; ++i) {
            if (row == -1) {
                row = std::get<3>(unicodeRangeInfo) = counter / 16;
            }
            QPushButton *charButton = new QPushButton();
            charButton->setText(QString(QChar(i)));
            charButton->setFlat(true);
            charButton->setCheckable(true);
            charButton->setChecked(false);
    //        charButton->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum));
            charButton->setFixedWidth(40);
            gridLayout->addWidget(charButton, counter / 16, counter % 16);
            mButtonVector.push_back(charButton);
            connect(charButton, SIGNAL(clicked(bool)),
                    this, SLOT(treatPushButtonClicked()));
            counter++;
        }
        ui->subsetComboBox->addItem(rangeName);
    }

    setCurrentButton(mButtonVector[0]);


    ui->scrollArea->setMinimumWidth(gridLayout->sizeHint().width() + ui->scrollArea->verticalScrollBar()->sizeHint().width());

    connect(ui->fontComboBox, SIGNAL(currentFontChanged(QFont)),
            this, SLOT(setFont(QFont)));
    connect(ui->insertPushButton, SIGNAL(clicked(bool)),
            this, SLOT(insertCurrentChar()));
    connect(ui->subsetComboBox, SIGNAL(activated(int)),
            this, SLOT(setUnicodeRange(int)));

    setFont(font);
}

TextEditDialog::~TextEditDialog()
{
    delete ui;
}

QFont TextEditDialog::font() const
{
    return ui->fontComboBox->font();
}

QString TextEditDialog::text() const
{
    return ui->textLineEdit->text();
}

void TextEditDialog::setText(const QString &str)
{
    ui->textLineEdit->setText(str);
}

void TextEditDialog::setFont(const QFont &newFont)
{
    if (font() != newFont) {
        ui->fontComboBox->setFont(newFont);
        ui->textLineEdit->setFont(newFont);
        for (auto button : mButtonVector) {
            button->setFont(newFont);
        }
        ui->fontComboBox->setCurrentFont(newFont);

    }
}

void TextEditDialog::setFontChangingEnable(bool enabled)
{
    ui->fontComboBox->setVisible(enabled);
    ui->fontLabel->setVisible(enabled);
}

void TextEditDialog::timerEvent(QTimerEvent *event)
{
    QDialog::timerEvent(event);
    if (event->timerId() == mTimerId)
        mTimerId = 0;
}

void TextEditDialog::setCurrentButton(QPushButton *newCurrentButton)
{

    int oldIndex = mButtonVector.indexOf(mCurrentButton);
    int newIndex = mButtonVector.indexOf(newCurrentButton);
    if (newIndex >= 0) {
        if (oldIndex >= 0) {
            mCurrentButton->setChecked(false);
        }
        newCurrentButton->setChecked(true);
        mCurrentButton = newCurrentButton;

        ui->scrollArea->ensureWidgetVisible(mCurrentButton);

        //setting visible info about current character
        QString newChar(mCurrentButton->text());
        ushort unicodeCode = newChar[0].unicode();
        QString unicodeCodeString = QString::number(unicodeCode, 16);
        unicodeCodeString = QString(4 - unicodeCodeString.size(), '0') + unicodeCodeString;

        QString htmlText = QString("<p align=center>U+" + unicodeCodeString + "</p><br>");
        htmlText += QString("<h1 align=center>") + newChar + "</h1><br>";
        ui->charachterTextBrowser->setHtml(htmlText);

        //setting Unicode range
        for (int i = 0; i < mUnicodeRangeInfo.size(); ++i) {
            if (unicodeCode >= std::get<1>(mUnicodeRangeInfo[i])
                    && unicodeCode <= std::get<2>(mUnicodeRangeInfo[i])) {
                setUnicodeRange(i);
                break;
            }
        }
    }

}

void TextEditDialog::treatPushButtonClicked()
{
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if (button) {
        auto oldButton = mCurrentButton;

        setCurrentButton(button);


        if (mTimerId != 0) {
            //insert current char if double click was detected
            if (oldButton == mCurrentButton) {
                insertCurrentChar();
            }
            killTimer(mTimerId);
        }
        mTimerId = startTimer(500);

    }
}

void TextEditDialog::insertCurrentChar()
{
    if (mCurrentButton) {
        int index = mButtonVector.indexOf(mCurrentButton);
        if (index >= 0) {
//            int unicodeCode = index + 32;
//            QString newCharString = QChar(unicodeCode);

            QString newCharString = mCurrentButton->text();

            QString text = ui->textLineEdit->text();
            int cursor_pos = ui->textLineEdit->cursorPosition();
            text.insert(cursor_pos, newCharString);
            ui->textLineEdit->setText(text);
            ui->textLineEdit->setCursorPosition(cursor_pos + 1);
        }
    }
}

void TextEditDialog::setUnicodeRange(int index)
{
    if (index < 0 || index >= mUnicodeRangeInfo.size())
        return;
    if (ui->subsetComboBox->currentIndex() != index) {
        ui->subsetComboBox->setCurrentIndex(index);
    }
    int curIndex = mButtonVector.indexOf(mCurrentButton);
    if (curIndex != -1) {
        int curUnicodeCode = mCurrentButton->text()[0].unicode();
        if (curUnicodeCode >= static_cast<int>(std::get<1>(mUnicodeRangeInfo[index]))
                && curUnicodeCode <= static_cast<int>(std::get<2>(mUnicodeRangeInfo[index]))) {
            return;  // return if the current symbol is already in the desired range
        }
        int currentColumn = curIndex % 16;
        int newIndex = std::get<3>(mUnicodeRangeInfo[index]) * 16 + currentColumn;
        if (newIndex >= mButtonVector.size()) {
            newIndex = mButtonVector.size();
        }
        setCurrentButton(mButtonVector[newIndex]);
    }
}

