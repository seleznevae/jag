#include "fontwidet.h"
#include "ui_fontwidet.h"

FontWidet::FontWidet(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FontWidet)
{
    ui->setupUi(this);
    connectAll();
}

FontWidet::~FontWidet()
{
    delete ui;
}

void FontWidet::setFont(const QFont &new_font)
{
    disconnectAll();
    ui->fontFamilyComboBox->setCurrentFont(new_font);
    ui->fontSizeSpinBox->setValue(new_font.pointSize());
    connectAll();
}

QFont FontWidet::font() const
{
    QFont ret(ui->fontFamilyComboBox->currentFont());
    ret.setPointSize(ui->fontSizeSpinBox->value());
    return ret;
}

void FontWidet::treatParametersChanging()
{
    emit fontEdited(font());
}

void FontWidet::connectAll()
{
    connect(ui->fontFamilyComboBox, SIGNAL(currentFontChanged(QFont)),
            this, SLOT(treatParametersChanging()));
    connect(ui->fontSizeSpinBox, SIGNAL(valueChanged(int)),
            this, SLOT(treatParametersChanging()));
}

void FontWidet::disconnectAll()
{
    disconnect(ui->fontFamilyComboBox, SIGNAL(currentFontChanged(QFont)),
            this, SLOT(treatParametersChanging()));
    disconnect(ui->fontSizeSpinBox, SIGNAL(valueChanged(int)),
            this, SLOT(treatParametersChanging()));
}
