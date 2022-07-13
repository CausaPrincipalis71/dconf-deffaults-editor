#include "gschema.h"

QStringList GSchema::m_undividedSchemasList;
QList<QStringList> GSchema::m_dividedSchemasList;
QVector<QPushButton*> GSchema::m_schemasButtons;

GSchema::GSchema(QString schema, QString key, QString value, QObject *parent)
{
    if(!m_dividedSchemasList.contains(schema.split('.')))       // Add a new schema if it doesn't already exist
        m_dividedSchemasList.push_back(schema.split('.'));      // Schema name converted from "org.gnome.desktop.background" to splitted view "org" "gnome" "desktop" "background" for wxample

    if(!m_undividedSchemasList.contains(schema))                // Add a new schema if it doesn't already exist
        m_undividedSchemasList.push_back(schema);

    m_key = new GKey(key, schema, value);
}

void GSchema::createSchemasButtons()
{
    for(int i = 0; i < m_undividedSchemasList.size(); i++)
    {
        m_schemasButtons.push_back(new QPushButton(m_undividedSchemasList.at(i)));
    }
}

GKey *GSchema::key() const
{
    return m_key;
}

const QStringList &GSchema::undividedSchemasList()
{
    return m_undividedSchemasList;
}

const QList<QStringList> &GSchema::dividedSchemasList()
{
    return m_dividedSchemasList;
}

const QVector<QPushButton *> &GSchema::schemasButtons()
{
    return m_schemasButtons;
}


