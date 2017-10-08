#include "datasource/datasourcebasic.h"
#include <iostream>
#include <string.h>
#include <fstream>
#include "datasource/csvdatasource.h"
#include "datasource/tabledatasource.h"
#include "datasource/plugindatasource.h"
#include "global/global_definitions.h"

namespace {

bool isCSVFile(QString fileName);
}

DataSource *createDataSourceFromFileName(QString fileName)
{
    for (const auto & plugin : global::gPlugins) {
        if (plugin.match(fileName)) {
            struct jag_data_source_api *loader = plugin.load(fileName);
            DataSource * dataSource = new PluginDataSource(loader, fileName);
            if (dataSource)
                return dataSource;
        }
    }

    if (isCSVFile(fileName)){
        return new CSVDataSource(fileName);
    } else {
        return new TableDataSource(fileName);
    }

}






namespace {




bool isCSVFile(QString fileName_)
{
    if (fileName_.toLower().endsWith(".csv")) {
        return true;
    }
    return false;
}

}
