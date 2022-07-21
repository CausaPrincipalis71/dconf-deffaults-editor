#ifndef GSCHEMA_H
#define GSCHEMA_H

#include <QObject>
#include <QVector>
#include <QList>
#include <QProcess>
#include <QPushButton>
#include <QLayout>
#include <QScrollArea>
#include <QSpacerItem>
#include "gkey.h"

// A class containing a schema and its keys

class GSchema : public QObject
{
Q_OBJECT

public:
    explicit GSchema(QString schema, QVector<QString> key, QVector<QString> value, QObject *parent = nullptr);

    // Getters
    const QVector<GKey *> &keys() const;

    static const QStringList &undividedSchemeList();
    static const QList<QStringList> &dividedSchemeList();

    const QVector<QPushButton *> &keysButtons() const;

    const GKey * getKeyAt(int number) const;

    QScrollArea *keysButtonsWidget() const;

    const QString &name() const;

private:
    QVector<GKey*> m_keys;                                        // Object that contains keys, it`s value and description
    QVector<QPushButton*> m_keysButtons;                          // Button for each key

    QScrollArea * m_keysButtonsWidget;

    QString m_name;                                               // Name of schema

    // Requires access to information about all schemes from one object
    static QStringList m_undividedSchemeList;
    static QList<QStringList> m_dividedSchemeList;     // Schema name converted from "org.gnome.desktop.background" to splitted view "org" "gnome" "desktop" "background"

signals:
    void buttonClicked(int number);

private slots:
    void on_buttonClicked(int number);
};

#endif // GSCHEMA_H
