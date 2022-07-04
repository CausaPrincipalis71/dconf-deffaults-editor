#ifndef GKEY_H
#define GKEY_H

#include <QObject>
#include <QString>

// A class containing key for schema, its description and value;

class GKey
{
public:
    GKey(const QString &name, const QString &description, const QString &value);
    GKey(const QString &name, const QString &schema);

    const QString &description() const;

    const QString &value() const;

    const QString &name() const;

private:
    QString m_name;
    QString m_description;
    QString m_value;
};

#endif // GKEY_H
