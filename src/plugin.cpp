#include "plugin.h"
#include <QQmlEngine>

KU_LedDisplay_PluginConnector::KU_LedDisplay_PluginConnector(QObject* parent)
{
    connect(&port, &QSerialPort::readyRead, this, [this]() {
        emitLogSignal("Received: " + QString(port.readAll()));
    });
}

void KU_LedDisplay_PluginConnector::pluginSlot(const QString& signal, const QVariantMap& data)
{
    if (signal == "send_led")
    {
        send(data["text"].toString());
    }
}

void KU_LedDisplay_PluginConnector::selectPort(int index)
{
    port.close();

    if (index < QSerialPortInfo::availablePorts().size())
    {
        port.setPortName(QSerialPortInfo::availablePorts().at(index).portName());
        port.setBaudRate(9600);
        port.open(QIODevice::ReadWrite);
    }
}

void KU_LedDisplay_PluginConnector::refresh()
{
    availablePorts.clear();
    for (auto& p : QSerialPortInfo::availablePorts())
        availablePorts.append(p.manufacturer() + " " + p.serialNumber() + " " + p.portName());

    emit availablePortsChanged();
}

void KU_LedDisplay_PluginConnector::send(QString const& text)
{
    if (port.isOpen())
        port.write(text.toUtf8());
    else
        emitLogSignal("Port not open: " + port.portName());
}

void KU_LedDisplay_PluginConnector::save(const QString& text)
{
    this->savedTexts.append(text);
    emit savedTextsChanged();
}

void KU_LedDisplay_PluginConnector::remove(int index)
{
    this->savedTexts.removeAt(index);
    emit savedTextsChanged();
}

QStringList KU_LedDisplay_PluginConnector::getSavedTexts()
{
    return this->savedTexts;
}

void KU_LedDisplay_PluginConnector::setSavedTexts(const QStringList& list)
{
    this->savedTexts = list;
    emit savedTextsChanged();
}

QString KU_LedDisplay_Plugin::name() const
{
    return "LedDisplay";
}

QString KU_LedDisplay_Plugin::id() const
{
    return "karunit_led_display";
}

KU::PLUGIN::PluginVersion KU_LedDisplay_Plugin::version() const
{
    return {1, 0, 0};
}

QString KU_LedDisplay_Plugin::license() const
{
    return "LGPL";
}

QString KU_LedDisplay_Plugin::icon() const
{
    return QString();
}

bool KU_LedDisplay_Plugin::initialize()
{
    qmlRegisterSingletonInstance("KarunitPlugins", 1, 0, "KUPLedDisplayPluginConnector", this->pluginConnector);

    return true;
}

bool KU_LedDisplay_Plugin::stop()
{
    return true;
}

bool KU_LedDisplay_Plugin::loadSettings()
{
    this->getPluginConnector()->setSavedTexts(KU::Settings::instance()->get(id() + "/savedTexts", QStringList()).toStringList());
    return true;
}

bool KU_LedDisplay_Plugin::saveSettings()
{
    KU::Settings::instance()->save(id() + "/savedTexts", this->getPluginConnector()->getSavedTexts());
    return KU::Settings::instance()->status() == QSettings::NoError;
}

KU_LedDisplay_PluginConnector* KU_LedDisplay_Plugin::getPluginConnector()
{
    if (this->pluginConnector == nullptr)
        this->pluginConnector = new KU_LedDisplay_PluginConnector;
    return qobject_cast<KU_LedDisplay_PluginConnector*>(this->pluginConnector);
}
