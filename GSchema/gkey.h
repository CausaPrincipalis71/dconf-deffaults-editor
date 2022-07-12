#ifndef GKEY_H
#define GKEY_H

#include <QObject>
#include <QString>
#include <QProcess>

// A class containing key for schema, its description and value;

class GKey : public QObject
{
Q_OBJECT

public:
    explicit GKey(const QString &name, const QString &description, const QString &value, const QString &range, QObject * parent = nullptr);
    explicit GKey(const QString &name, const QString &schema, const QString &value, QObject * parent = nullptr);

    const QString &description() const;

    const QString &value() const;

    const QString &name() const;

private:
    QString m_name;
    QString m_description;
    QString m_value;
    QString m_range;

    QProcess * gsettingsDescribe;           // QProcess running gsettings describe
    QProcess * gsettingsRange;              // QPRocess running gsettings range

private slots:
    void GSettingsDescribeCommandFinished();
    void GSettingsRangeCommandFinished();
};

#endif // GKEY_H
