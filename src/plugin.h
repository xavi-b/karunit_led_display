#ifndef WHATSAPPPLUGIN_H
#define WHATSAPPPLUGIN_H

#include <QtPlugin>
#include <QIcon>
#include <QDebug>
#include <QDateTime>
#include <QSerialPort>
#include <QSerialPortInfo>
#include "plugininterface.h"
#include "settings.h"

class KU_LedDisplay_PluginConnector : public KU::PLUGIN::PluginConnector
{
    Q_OBJECT

    Q_PROPERTY(QStringList availablePorts MEMBER availablePorts NOTIFY availablePortsChanged)
    Q_PROPERTY(QStringList savedTexts READ getSavedTexts WRITE setSavedTexts NOTIFY savedTextsChanged)

public:
    KU_LedDisplay_PluginConnector(QObject* parent = nullptr);

    Q_INVOKABLE void selectPort(int index);
    Q_INVOKABLE void refresh();
    Q_INVOKABLE void send(QString const& text);
    Q_INVOKABLE void save(QString const& text);
    Q_INVOKABLE void remove(int index);

    QStringList getSavedTexts();
    void        setSavedTexts(QStringList const& list);

signals:
    void availablePortsChanged();
    void savedTextsChanged();

private:
    QStringList availablePorts;
    QSerialPort port;
    QStringList savedTexts;
};

class KU_LedDisplay_Plugin : public QObject, public KU::PLUGIN::PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "xavi-b.karunit.PluginInterface")
    Q_INTERFACES(KU::PLUGIN::PluginInterface)

public:
    virtual QString                   name() const override;
    virtual QString                   id() const override;
    virtual KU::PLUGIN::PluginVersion version() const override;
    virtual QString                   license() const override;
    virtual QString                   icon() const override;
    virtual bool                      initialize() override;
    virtual bool                      stop() override;

    virtual bool loadSettings() override;
    virtual bool saveSettings() override;

    virtual KU_LedDisplay_PluginConnector* getPluginConnector() override;
};

#endif // WHATSAPPPLUGIN_H
