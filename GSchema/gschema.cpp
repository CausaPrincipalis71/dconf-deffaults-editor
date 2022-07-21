#include "gschema.h"

QStringList GSchema::m_undividedSchemeList;
QList<QStringList> GSchema::m_dividedSchemeList;

GSchema::GSchema(QString schema, QVector<QString> keys, QVector<QString> values, QObject *parent)
{
    if(!m_dividedSchemeList.contains(schema.split('.')))       // Add a new schema if it doesn't already exist
        m_dividedSchemeList.push_back(schema.split('.'));      // Schema name converted from "org.gnome.desktop.background" to splitted view "org" "gnome" "desktop" "background" for wxample

    if(!m_undividedSchemeList.contains(schema))                // Add a new schema if it doesn't already exist
        m_undividedSchemeList.push_back(schema);

    m_name = schema;

    // Create a scroll area for buttons
    m_keysButtonsWidget = new QScrollArea();
    m_keysButtonsWidget->setWidget(new QWidget);
    m_keysButtonsWidget->widget()->setLayout(new QVBoxLayout());
    m_keysButtonsWidget->setWidgetResizable(true);

    for(int i = 0; i < keys.size(); i++)
    {
        m_keys.push_back(new GKey(keys.at(i), schema, values.at(i)));
        m_keysButtons.push_back(new QPushButton(keys.at(i)));
        m_keysButtons.back()->setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum));
        m_keysButtonsWidget->widget()->layout()->addWidget(m_keysButtons.at(i));
        connect(m_keysButtons.at(i), &QPushButton::clicked, this, [=]() {on_buttonClicked(i);});
    }

    m_keysButtonsWidget->widget()->layout()->addItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));
}

void GSchema::on_buttonClicked(int number)
{
    emit buttonClicked(number);
}

const QStringList &GSchema::undividedSchemeList()
{
    return m_undividedSchemeList;
}

const QList<QStringList> &GSchema::dividedSchemeList()
{
    return m_dividedSchemeList;
}

const QVector<QPushButton *> &GSchema::keysButtons() const
{
    return m_keysButtons;
}

const GKey *GSchema::getKeyAt(int number) const
{
    return m_keys.at(number);
}

QScrollArea *GSchema::keysButtonsWidget() const
{
    return m_keysButtonsWidget;
}

const QString &GSchema::name() const
{
    return m_name;
}

const QVector<GKey *> &GSchema::keys() const
{
    return m_keys;
}


