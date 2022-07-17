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

    // Check if user profile file exists, if not, create it and set the value
    userProfile = new QFile("/etc/dconf/profile/user");
    if(userProfile->exists() == 0)
    {
        userProfile->open(QIODevice::WriteOnly);

        QTextStream out(userProfile);
        out << "user-db:user" << Qt::endl << "system-db:local" << Qt::endl;

        userProfile->close();
        delete userProfile;
    }
    else
    {
        userProfile->open(QIODevice::ReadWrite);
        auto fileData = userProfile->readAll();
        QTextStream out(userProfile);

        if(fileData.contains("user-db:user") == 0)
        {
            out << Qt::endl <<"user-db:user" << Qt::endl;
        }

        if(fileData.contains("system-db:local") == 0)
        {
            out << Qt::endl << "system-db:local" << Qt::endl;
        }

        userProfile->close();
        delete userProfile;
    }

    // Checking if the database directory exists. If not, it creates the necessary directories.
    if(dbDir->exists("local.d/locks") == 0)
    {
        dbDir->mkpath("local.d/locks");
    }


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

void MainWindow::setKeyValueAsDefault(QString value, QString file)
{
    // Open file with name "00_*Schema Name*"
    dbDefaultFile->setFileName(file);
    dbDefaultFile->open(QIODevice::ReadWrite);

    // Read data from file
    QString fileData = dbDefaultFile->readAll();

    if(fileData.isEmpty())
    {
        // Convert from "org.gnome.calculator" to "org/gnome/calculator"
        QString schemaName = GSchema::undividedSchemeList().at(currentSchemaNumber);
        schemaName.replace(".", "/");
        // File must start with the schema name
        fileData.append("[" + schemaName + "]\n");
    }

    // If already some value is set to default
    if(fileData.contains(currentKey->name()))
    {
        // Clear data and then rewrite
        int i = fileData.indexOf(currentKey->name());
        while(fileData.at(i) != "\n" && i < fileData.size())
        {
            fileData.remove(i);
        }
    }

    fileData.append(currentKey->name() + "=" + value + "\r\n");

    dbDefaultFile->close();
    dbDefaultFile->remove();
    dbDefaultFile->open(QIODevice::WriteOnly);

    QTextStream out(dbDefaultFile);

    dbDefaultFile->write(fileData.toStdString().data());

    dbDefaultFile->close();
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
    currentSchemaNumber = buttonNumber;
    auto &keysButtons = currentSchema->keysButtons();

    connect(currentSchema, &GSchema::buttonClicked, this, &MainWindow::on_keyButtonClicked);

    keysButtonsWidget->setCurrentIndex(buttonNumber);

    ui->schemaName->setText(GSchema::undividedSchemeList().at(currentSchemaNumber));
    ui->keyPage_schemaName->setText(GSchema::undividedSchemeList().at(currentSchemaNumber));

    ui->stackedWidget->setCurrentWidget(ui->schemePage);
    ui->goBackPushButton->setEnabled(true);
}

void MainWindow::on_keyButtonClicked(int buttonNumber)
{
    ui->stackedWidget->setCurrentWidget(ui->keyPage);

    currentKey = currentSchema->getKeyAt(buttonNumber);

    ui->currentKeyLabel->setText(currentKey->name());
    ui->currentValue->setText(currentKey->value());
    ui->keyDescription->setPlainText(currentKey->description());

    if(currentKey->type() == GKey::NUMBER)
    {
        ui->keyValue_spinBox->show();
        ui->keyValue_spinBox->setValue(currentKey->value().toFloat());

        ui->keyValue_lineEdit->hide();
        ui->keyValue_comboBox->hide();
        ui->keyValue_doubleSpinBox->hide();
    }
    else if(currentKey->type() == GKey::DOUBLE)
    {
        ui->keyValue_doubleSpinBox->show();
        ui->keyValue_doubleSpinBox->setValue(currentKey->value().toFloat());

        ui->keyValue_lineEdit->hide();
        ui->keyValue_comboBox->hide();
        ui->keyValue_spinBox->hide();
    }
    else if(currentKey->type() == GKey::ENUM || currentKey->type() == GKey::BOOL)
    {
        ui->keyValue_comboBox->show();
        ui->keyValue_comboBox->clear();
        ui->keyValue_comboBox->addItems(currentKey->enumAvailableOptions());
        ui->keyValue_comboBox->setCurrentText(currentKey->value());

        ui->keyValue_lineEdit->hide();
        ui->keyValue_doubleSpinBox->hide();
        ui->keyValue_spinBox->hide();
    }
    else
    {
        ui->keyValue_lineEdit->show();
        ui->keyValue_lineEdit->setText(currentKey->value());

        ui->keyValue_comboBox->hide();
        ui->keyValue_doubleSpinBox->hide();
        ui->keyValue_spinBox->hide();
    }
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

QString MainWindow::readKeyValue()
{
    QString value;

    if(currentKey->type() == GKey::NUMBER)
    {
        value = QString::number(ui->keyValue_spinBox->value());
    }
    else if(currentKey->type() == GKey::DOUBLE)
    {
        value = QString::number(ui->keyValue_doubleSpinBox->value());
    }
    else if(currentKey->type() == GKey::ENUM || currentKey->type() == GKey::BOOL)
    {
        value = ui->keyValue_comboBox->currentText();
    }
    else
    {
        value = ui->keyValue_lineEdit->text();
    }

    return value;
}

void MainWindow::on_setCurrentValuesAsDefault_pushButton_clicked()
{
    QString value = readKeyValue();

    setKeyValueAsDefault(value, QString("/etc/dconf/db/local.d/00_") + GSchema::undividedSchemeList().at(currentSchemaNumber));

    QProcess::execute("dconf", QStringList("update"));

    QMessageBox::information(this, tr("Успешное завершение"), "Параметру " + currentKey->name() + " присвоено значение " + value + " по умолчанию");
}


void MainWindow::on_fixCurrentValueAsDefaulr_pushbutton_clicked()
{
    QString value = readKeyValue();

    setKeyValueAsDefault(value, QString("/etc/dconf/db/local.d/00_") + GSchema::undividedSchemeList().at(currentSchemaNumber));
    setKeyValueAsDefault(value, QString("/etc/dconf/db/local.d/locks/00_") + GSchema::undividedSchemeList().at(currentSchemaNumber));

    QProcess::execute("dconf", QStringList("update"));

    QMessageBox::information(this, tr("Успешное завершение"), "Параметру " + currentKey->name() + " зафиксировано значение " + value + " по умолчанию");

}

