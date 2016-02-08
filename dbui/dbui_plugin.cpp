#include "dbui_plugin.h"
#include "db.h"
#include "localstoragetable.h"
#include "localstoragetablemodel.h"
#include "localstoragedatabase.h"

#include <qqml.h>

void DbuiPlugin::registerTypes(const char *uri)
{
    // @uri com.hempnall
   //   qmlRegisterType<db>(uri, 1, 0, "db");
    qmlRegisterType<LocalStorageTable>(uri, 1, 0, "LocalStorageTable");
    qmlRegisterType<LocalStorageTableModel>(uri, 1, 0, "LocalStorageTableModel");
    qmlRegisterType<LocalStorageDatabase>(uri, 1, 0, "LocalStorageDatabase");

}


