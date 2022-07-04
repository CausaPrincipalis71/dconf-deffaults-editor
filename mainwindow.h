#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include <QVector>
#include <QTimer>

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

    QVector<GSchema*> schemasVector;

    void getSchemasWithKeys();                  //Get list of all schemas, keys and value with QProcess and GSettings

private slots:
    void on_GSettingsListSchemasFinished();

    void wholeKeysCreated();                 // Activated after schemasVector is filled
};
#endif // MAINWINDOW_H
