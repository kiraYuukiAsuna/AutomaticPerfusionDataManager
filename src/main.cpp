#include <Log/Log.h>
#include <UI/MainWindow.h>
#include <DataImport/DataImporter.hpp>
#include "FileReader/VideoFrameInfo.hpp"
#include "sqlite_orm.h"
#include <QApplication>
#include "ElaApplication.h"

int main(int argc, char* argv[]) {
    Seele::Log::Init();
    QApplication app(argc, argv);
    eApp->init();
    MainWindow w;
    w.show();
    auto ret = app.exec();
    Seele::Log::Shutdown();
    return ret;
}
