#ifndef GSCHEMA_H
#define GSCHEMA_H

#include <QObject>
#include <QVector>
#include <QList>
#include <QProcess>
#include <QPushButton>
#include "gkey.h"

// A class containing a schema and its keys

class GSchema : public QObject
{
Q_OBJECT

public:
    explicit GSchema(QString schema, QString key, QString value, QObject *parent = nullptr);

    static void createSchemasButtons();                 // Create button for each schemes

    // Getters
    GKey *key() const;

    static const QStringList &undividedSchemasList();
    static const QList<QStringList> &dividedSchemasList();
    static const QVector<QPushButton *> &schemasButtons();

private:
    GKey* m_key;                                        // Object that contains keys, it`s value and description

    // Requires access to information about all schemes from one object
    static QStringList m_undividedSchemasList;
    static QList<QStringList> m_dividedSchemasList;     // Schema name converted from "org.gnome.desktop.background" to splitted view "org" "gnome" "desktop" "background"
    static QVector <QPushButton*> m_schemasButtons;     // Button that is used to select this scheme


private slots:

};

#endif // GSCHEMA_H
