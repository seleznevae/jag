#ifndef TEXTDATASOURCEDIALOG_H
#define TEXTDATASOURCEDIALOG_H

#include <QDialog>

namespace Ui {
class TextDataSourceDialog;
}

class TextDataSourceDialog : public QDialog
{
    Q_OBJECT

public:

    explicit TextDataSourceDialog(QWidget *parent = 0);
    ~TextDataSourceDialog();

    void loadFile(const QString &fileName);
    char fieldSeparator() const;
    int parameterNameLineNumber() const;
    int parameterDescriptionLineNumber() const;
    int parameterUnitsLineNumber() const;
    int argumentPosition() const;
    QTextCodec* textCodec() const;

public slots:
    void treatFieldSeparatorChanging();

    void treatNameLineNumberChanging();
    void treatDescriptionLineNumberChanging();
    void treatUnitsLineNumberChanging();
    void treatEncodingChanging();

private:
    Ui::TextDataSourceDialog *ui;
    QString mFileName;
    char mFieldSeparator;
    QTextCodec *mTextCodec;
};

#endif // TEXTDATASOURCEDIALOG_H
