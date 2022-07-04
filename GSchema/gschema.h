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
    explicit GSchema(QString schema, QObject *parent = nullptr);

    QStringList keysName();

    const QString &undividedSchema() const;

private:
    QVector<GKey*> keysVector;   // Vector that contains keys, it`s value and description

    QString m_undividedSchema;
    QStringList m_dividedSchema;  // Schema name converted from "org.gnome.desktop.background" to splitted view "org" "gnome" "desktop" "background"

    QProcess * gsettings;       // QProcess running gsettings

private slots:
    void on_GSettingsListKeysFinished();
};

#endif // GSCHEMA_H
