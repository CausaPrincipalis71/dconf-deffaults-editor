#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include <QVector>
#include <QTimer>
#include <QStackedWidget>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QMessageBox>

#include "GSchema/gschema.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:   
    Ui::MainWindow *ui;

    QProcess * gsettings;

    QVector<GSchema*> schemeVector;
    QVector<QPushButton*> schemeButtons;

    GSchema * currentSchema = nullptr;
    int currentSchemaNumber;

    GKey const * currentKey = nullptr;
    QStackedWidget * keysButtonsWidget;

    QFile * userProfile;
    QFile * dbDefaultFile = new QFile();
    QFile * dbLockFile = new QFile();
    QDir * dbDir = new QDir("/tmp/etc/dconf/db/");

    void getSchemeWithKeys();                  //Get list of all schemas, keys and value with QProcess and GSettings

    QString readKeyValue();
    void writeKeyValueToFile(QString value, QString file);

private slots:
    void on_GSettingsListSchemeFinished();

    void on_SchemaButtonClicked(int buttonNumber);
    void on_keyButtonClicked(int buttonNumber);

    void wholeKeysCreated();                 // Activated after schemasVector is filled

    void on_goBackPushButton_clicked();
    void on_setCurrentValuesAsDefault_pushButton_clicked();
    void on_fixCurrentValueAsDefaulr_pushbutton_clicked();
    void on_schemaSearchLine_textChanged(const QString &arg1);
    void on_keySearchLine_textChanged(const QString &arg1);
    void on_setSchemaAsDefault_pushButton_clicked();
    void on_fixSchemaValue_pushButton_clicked();
};
#endif // MAINWINDOW_H
