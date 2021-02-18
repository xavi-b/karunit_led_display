#include "plugin.h"

QString KU_LedDisplay_Plugin::name() const
{
    return "LedDisplay";
}

QString KU_LedDisplay_Plugin::id() const
{
    return "LedDisplay.me";
}

KU::PLUGIN::PluginVersion KU_LedDisplay_Plugin::version() const
{
    return { 1, 0, 0 };
}

QString KU_LedDisplay_Plugin::license() const
{
    return "LGPL";
}

QIcon KU_LedDisplay_Plugin::icon() const
{
    return QIcon();
}

bool KU_LedDisplay_Plugin::initialize()
{
    return true;
}

bool KU_LedDisplay_Plugin::stop()
{
    return true;
}

QWidget* KU_LedDisplay_Plugin::createWidget()
{
    QWidget* main = new QWidget;
    QVBoxLayout* layout = new QVBoxLayout;
    QHBoxLayout* topLayout = new QHBoxLayout;
    QComboBox* availablePortsComboBox = new QComboBox;
    topLayout->addWidget(availablePortsComboBox, 1);
    QPushButton* refreshBtn = new QPushButton(QIcon::fromTheme("view-refresh"), "");
    topLayout->addWidget(refreshBtn);
    layout->addLayout(topLayout, 0);
    QLineEdit* lineEdit = new QLineEdit;
    layout->addWidget(lineEdit, 1);
    main->setLayout(layout);

    for(auto& p : QSerialPortInfo::availablePorts())
        availablePortsComboBox->addItem(p.manufacturer() + " " + p.serialNumber() + " " + p.portName(), p.portName());

    connect(refreshBtn, &QPushButton::clicked, this, [=](){
        availablePortsComboBox->clear();
        for(auto& p : QSerialPortInfo::availablePorts())
            availablePortsComboBox->addItem(p.manufacturer() + " " + p.serialNumber() + " " + p.portName(), p.portName());
    });

    connect(&port, &QSerialPort::readyRead, this, [this](){
        this->getPluginConnector()->emitLogSignal("Received: " + QString(port.readAll()));
    });

    connect(availablePortsComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=](int index){
        port.close();
        port.setPortName(availablePortsComboBox->itemData(index).toString());
        port.setBaudRate(9600);
        port.open(QIODevice::ReadWrite);
    });

    connect(lineEdit, &QLineEdit::returnPressed, this, [=](){
        if(port.isOpen())
        {
            port.write(lineEdit->text().toUtf8());
        }
        else
        {
            this->getPluginConnector()->emitLogSignal("Port not open: " + port.portName());
        }
    });

    return main;
}

QWidget* KU_LedDisplay_Plugin::createSettingsWidget()
{
    return new QLabel("WhatsApp");
}

QWidget* KU_LedDisplay_Plugin::createAboutWidget()
{
    return nullptr;
}

bool KU_LedDisplay_Plugin::loadSettings()
{
    return true;
}

bool KU_LedDisplay_Plugin::saveSettings() const
{
    return KU::Settings::instance()->status() == QSettings::NoError;
}
