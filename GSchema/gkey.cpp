#include "gkey.h"

GKey::GKey(const QString &name, const QString &description, const QString &value)
{
    this->m_name = name;
    this->m_description = description;
    this->m_value = value;
}

GKey::GKey(const QString &name, const QString &schema)
{
    this->m_name = name;
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
