#include "gkey.h"

GKey::GKey(const QString &name, const QString &description, const QString &range, const QString &value, QObject * parent)
{
    this->m_name = name;
    this->m_description = description;
    this->m_value = value;
}

GKey::GKey(const QString &name, const QString &schema, const QString &value, QObject * parent)
{
    this->m_name = name;
    this->m_value = value;

    // Running command "gsettings describe *schema* *key*"
    QString program = "gsettings";
    QStringList arguments;
    arguments.push_back("describe");
    arguments.push_back(schema);
    arguments.push_back(name);

    gsettingsDescribe = new QProcess();
    gsettingsDescribe->start(program, arguments);

    connect(gsettingsDescribe, qOverload<int, QProcess::ExitStatus>(&QProcess::finished), this, &GKey::GSettingsDescribeCommandFinished);

    // Running command "gsettings range *schema* *key*"
    arguments.pop_front();
    arguments.push_front("range");

    gsettingsRange = new QProcess();
    gsettingsRange->start(program, arguments);

    connect(gsettingsRange, qOverload<int, QProcess::ExitStatus>(&QProcess::finished), this, &GKey::GSettingsRangeCommandFinished);
}

const QString &GKey::description() const
{
    return m_description;
}

const QString &GKey::value() const
{
    return m_value;
}

const QString &GKey::name() const
{
    return m_name;
}

const enum GKey::type GKey::type() const
{
    return m_type;
}

void GKey::GSettingsDescribeCommandFinished()
{
    QByteArray gsettingsOutput = gsettingsDescribe->readAllStandardOutput();
    gsettingsOutput.chop(1);                                                    // Removing '\n' at the end of string

    this->m_description = gsettingsOutput;

    disconnect(gsettingsDescribe, qOverload<int, QProcess::ExitStatus>(&QProcess::finished), this, &GKey::GSettingsDescribeCommandFinished);
    delete gsettingsDescribe;
}

void GKey::GSettingsRangeCommandFinished()
{
    QByteArray gsettingsOutput = gsettingsRange->readAllStandardOutput();

    this->m_range = gsettingsOutput.split('\n').at(0);                  // Split command is used to remove multiline values;

    if(m_range == "type i")
        m_type = NUMBER;
    else if(m_range == "type(ii)")
        m_type = ARRAY;
    else if(m_range == "type b")
        m_type = BOOL;
    else if(m_range == "type s")
        m_type = STRING;
    else if(m_range == "enum")
    {
        m_type = ENUM;

        auto gsettingsSplittedOutput = gsettingsOutput.split('\n');

        for(int i = 1; i < gsettingsSplittedOutput.size() - 1; i++)
        {
            enumAvailableOptions.push_back(gsettingsSplittedOutput.at(i));
        }
    }

    disconnect(gsettingsRange, qOverload<int, QProcess::ExitStatus>(&QProcess::finished), this, &GKey::GSettingsRangeCommandFinished);
    delete gsettingsRange;
}
