#ifndef PLUGINLISTVECTOREDITOR_H
#define PLUGINLISTVECTOREDITOR_H

#include <QString>
#include <QWidget>

class QGridLayout;
class QLineEdit;
class QPushButton;

class PluginListVectorEditor: public QWidget
{
    Q_OBJECT
public:
    explicit PluginListVectorEditor(QWidget *parent = 0);

    void setPluginsPathList(const QStringList &pathList);
    QStringList pluginsPathList() const;

private:
    QString mDefaultPluginDir;

    QGridLayout *mGridLayout;
    QVector<QLineEdit*> mPathEditVector;
    QVector<QPushButton*> mEditPathButtonVector;
    QVector<QPushButton*> mKillButtonVector;


    QWidget *bottom0;
    QWidget *bottom1;
    QWidget *bottom2;

    void clearAll();
    void addNewPluginPath(const QString &path);

private slots:
    void treatKillButtonPushed();
    void treatAddNewPath();
    void treatModifyExistingPath();
};

#endif // PLUGINLISTVECTOREDITOR_H
