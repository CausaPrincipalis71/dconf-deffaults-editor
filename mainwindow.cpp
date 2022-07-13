#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    getSchemeWithKeys();
    ui->setupUi(this);
    keysButtonsWidget = new QStackedWidget();
    ui->schemePage->layout()->addWidget(keysButtonsWidget);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::getSchemeWithKeys()
{
    // Running command "gsettings list-Scheme"
    QString program = "gsettings";
    QStringList arguments;
    arguments.push_back("list-recursively");
    gsettings = new QProcess();
    gsettings->start(program, arguments);

    // Waiting for finishing of process
    connect(gsettings, qOverload<int, QProcess::ExitStatus>(&QProcess::finished), this, &MainWindow::on_GSettingsListSchemeFinished);
}

void MainWindow::on_GSettingsListSchemeFinished()
{
    // Processing output of "gsettings list-Scheme"
    QByteArray gsettingsOutput = gsettings->readAllStandardOutput();

    auto schemeList = gsettingsOutput.split('\n');
    schemeList.removeLast(); // Last string is empty


    for(int i = 0; i < schemeList.size(); i++)
    {
        // Parsing expression like "org.gnome.desktop.media-handling autorun-x-content-start-app ['x-content/unix-software', 'x-content/ostree-repository']"
        auto splittedString = schemeList[i].split(' ');

        auto schema = splittedString.at(0);
        QVector<QString> keys;
        QVector<QString> values;

        for(int j = i; j < schemeList.size(); j++)      // Nested loop to find all keys of the same schema
        {
            splittedString = schemeList[j].split(' ');
            auto tempSchema = splittedString.at(0);

            if (tempSchema != schema)             // If schema changed, then break the cycle
            {
                i = j - 1;          // Take into account the passed iterations of the cycle
                break;
            }

            keys.push_back(splittedString.at(1));

            QString value = splittedString.at(2);
            for(int k = 3; k < splittedString.size(); k++)      // Cycle runs only if value of key contains space symbol
            {
                value.push_back(" ");
                value.push_back(splittedString.at(k));
            }

            values.push_back(value);
        }
        schemeVector.push_back(new GSchema(schema, keys, values));
    }

    //Waiting for the vector to fill
    QTimer::singleShot(100, this, &MainWindow::wholeKeysCreated);
}

void MainWindow::on_SchemaButtonClicked(int buttonNumber)
{
    // Function called when schema button is clicked. The number of the scheme in the vector was passed to the function.
    if (currentSchema != nullptr)
        disconnect(currentSchema, &GSchema::buttonClicked, this, &MainWindow::on_keyButtonClicked);


    currentSchema = schemeVector.at(buttonNumber);          // Setting actual schema
    auto &keysButtons = currentSchema->keysButtons();

    connect(currentSchema, &GSchema::buttonClicked, this, &MainWindow::on_keyButtonClicked);

    keysButtonsWidget->setCurrentIndex(buttonNumber);
    ui->stackedWidget->setCurrentWidget(ui->schemePage);
    ui->goBackPushButton->setEnabled(true);
}

void MainWindow::on_keyButtonClicked(int buttonNumber)
{

}

void MainWindow::wholeKeysCreated()
{
    for (int i = 0; i < GSchema::undividedSchemeList().size(); i++)
    {
        // Creating pushbutton for every scheme
        schemeButtons.push_back(new QPushButton(GSchema::undividedSchemeList().at(i)));
        ui->schemaButtonsScrollAreaContents->layout()->addWidget(schemeButtons.at(i));
        connect(schemeButtons[i], &QPushButton::clicked, this, [=] () { on_SchemaButtonClicked(i); });

        // Adding layout with keys buttons to stacked layout
        keysButtonsWidget->addWidget(schemeVector.at(i)->keysButtonsWidget());
    }
}

void MainWindow::on_goBackPushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(ui->stackedWidget->currentIndex() - 1);
    if(ui->stackedWidget->currentIndex() == 0)
        ui->goBackPushButton->setEnabled(false);
}

