#ifndef DBUI_PLUGIN_H
#define DBUI_PLUGIN_H

#include <QQmlExtensionPlugin>

class DbuiPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface")

public:
    void registerTypes(const char *uri);
};

#endif // DBUI_PLUGIN_H

