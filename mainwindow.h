#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include <QVector>
#include <QTimer>
#include <QStackedWidget>

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
    QStackedWidget * keysButtonsWidget;

    void getSchemeWithKeys();                  //Get list of all schemas, keys and value with QProcess and GSettings

private slots:
    void on_GSettingsListSchemeFinished();

    void on_SchemaButtonClicked(int buttonNumber);
    void on_keyButtonClicked(int buttonNumber);

    void wholeKeysCreated();                 // Activated after schemasVector is filled
    void on_goBackPushButton_clicked();
};
#endif // MAINWINDOW_H
