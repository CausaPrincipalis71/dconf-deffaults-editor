#ifndef GSCHEMA_H
#define GSCHEMA_H

#include <QObject>
#include <QVector>
#include <QProcess>
#include "gkey.h"

// A class containing a schema and its keys

class GSchema : public QObject
{
Q_OBJECT

public:
    explicit GSchema(QString schema, QString key, QString value, QObject *parent = nullptr);

    const QString &undividedSchema() const;

    const QStringList &dividedSchema() const;

    GKey *key() const;

private:
    GKey* m_key;                   // Object that contains keys, it`s value and description

    QString m_undividedSchema;
    QStringList m_dividedSchema;  // Schema name converted from "org.gnome.desktop.background" to splitted view "org" "gnome" "desktop" "background"

private slots:

};

#endif // GSCHEMA_H
