#include "gschema.h"

GSchema::GSchema(QString schema, QString key, QString value, QObject *parent)
{
    m_dividedSchema = schema.split('.');      // Schema name converted from "org.gnome.desktop.background" to splitted view "org" "gnome" "desktop" "background" for wxample
    m_undividedSchema = schema;

    m_key = new GKey(key, schema, value);
}

const QString &GSchema::undividedSchema() const
{
    return m_undividedSchema;
}

const QStringList &GSchema::dividedSchema() const
{
    return m_dividedSchema;
}

GKey *GSchema::key() const
{
    return m_key;
}

