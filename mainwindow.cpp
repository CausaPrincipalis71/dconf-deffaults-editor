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
    arguments.push_back("list-recursively");
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
        // Parsing expression like "org.gnome.desktop.media-handling autorun-x-content-start-app ['x-content/unix-software', 'x-content/ostree-repository']"
        auto splittedString = schemasList[i].split(' ');
        auto &schema = splittedString.at(0);
        auto &key = splittedString.at(1);
        QString value = splittedString.at(2);

        for(int j = 3; j < splittedString.size(); j++)      // Cycle runs only if value of key contains space symbol
        {
            value.push_back(" ");
            value.push_back(splittedString.at(j));
        }

        schemasVector.push_back(new GSchema(schema, key, value));
    }

    //Waiting for the vector to fill
    QTimer::singleShot(5000, this, &MainWindow::wholeKeysCreated);
}

void MainWindow::wholeKeysCreated()
{
    for (int i = 0; i < schemasVector.size(); i++)
    {

    }

}

