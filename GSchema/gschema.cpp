#include "gschema.h"

GSchema::GSchema(QString schema, QObject *parent)
{
    m_dividedSchema = schema.split('.');      // Schema name converted from "org.gnome.desktop.background" to splitted view "org" "gnome" "desktop" "background" for wxample
    m_undividedSchema = schema;

    // Running command "gsettings list-keys *schema*"
    QString program = "gsettings";
    QStringList arguments;
    arguments.push_back("list-keys");
    arguments.push_back(schema);

    gsettings = new QProcess();
    gsettings->start(program, arguments);

    // Waiting for finishing of process
    connect(gsettings, qOverload<int, QProcess::ExitStatus>(&QProcess::finished), this, &GSchema::on_GSettingsListKeysFinished);
}

QStringList GSchema::keysName()
{
    QStringList keysNameList;

    for(auto i = keysVector.begin(); i != keysVector.end(); i++)
        keysNameList.push_back((*i)->name());

    return keysNameList;
}

const QString &GSchema::undividedSchema() const
{
    return m_undividedSchema;
}

void GSchema::on_GSettingsListKeysFinished()
{
    // Split program output to get separated keys
    QByteArray gsettingsOutput = gsettings->readAllStandardOutput();
    auto keysList = gsettingsOutput.split('\n');
    keysList.removeLast();          // Last string is empty

    for(auto i = keysList.begin(); i != keysList.end(); i++)
    {
        keysVector.push_back(new GKey(*i, m_undividedSchema));
    }
}
