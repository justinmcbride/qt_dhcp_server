#pragma once

#include <QString>
#include <QDir>
#include <QSettings>

class global_config
{
  public:
    global_config() {}

    bool interface_enabled( QString name ) const { return m_settings.value( m_key_interface.arg(name), false ).toBool(); }
    void interface_enabled( QString name, bool enabled ) { m_settings.setValue( m_key_interface.arg(name), enabled ); }
  private:
    QSettings m_settings;

    const QString m_key_interface{ "interfaces/%0" };
};
