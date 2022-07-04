#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    getSchemasWithKeys();
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::getSchemasWithKeys()
{
    // Running command "gsettings list-schemas"
    QString program = "gsettings";
    QStringList arguments;
    arguments.push_back("list-schemas");
    gsettings = new QProcess();
    gsettings->start(program, arguments);

    // Waiting for finishing of process
    connect(gsettings, qOverload<int, QProcess::ExitStatus>(&QProcess::finished), this, &MainWindow::on_GSettingsListSchemasFinished);
}

void MainWindow::on_GSettingsListSchemasFinished()
{
    // Processing output of "gsettings list-schemas"
    QByteArray gsettingsOutput = gsettings->readAllStandardOutput();

    auto schemasList = gsettingsOutput.split('\n');
    schemasList.removeLast(); // Last string is empty

    for(int i = 0; i < schemasList.size(); i++)
    {
        schemasVector.push_back(new GSchema(schemasList[i]));
    }

    //Waiting for the vector to fill
    QTimer::singleShot(400, this, &MainWindow::wholeKeysCreated);
}

void MainWindow::wholeKeysCreated()
{
    for (auto i = schemasVector.begin(); i != schemasVector.end(); i++)
    {
        ui->plainTextEdit->appendPlainText((*i)->undividedSchema());

        auto schemaKeys = (*i)->keysName();
        for (auto j = schemaKeys.begin(); j != schemaKeys.end(); j++)
        {
            ui->plainTextEdit->appendPlainText(*j);
        }

        ui->plainTextEdit->appendPlainText("\n");
    }
}

