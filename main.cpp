#include <iostream>
#include <QApplication>
#include <Log/Log.h>
#include <UI/MainWindow.h>
#include "sqlite_orm.h"
#include "FileReader/CellTissueInfo.hpp"
#include "FileReader/StateTimeDifferencesInfo.hpp"
#include "FileReader/VideoFrameInfo.hpp"
#include <DataImport/DataImporter.hpp>

#include "DataImport/DataImporter.hpp"
#include "Database/SqliteSchema.hpp"

struct User {
    int id;
    std::string firstName;
    std::string lastName;
    int birthDate;
    std::unique_ptr<std::string> imageUrl;
    int typeId;
};

struct UserType {
    int id;
    std::string name;
};


int smain(int argc, char* argv[]) {
    Seele::Log::Init();

    QApplication app(argc, argv);

    MainWindow window;
    window.show();


    using namespace sqlite_orm;
    auto storage = make_storage("DataBase.sqlite",
                                make_table("CellTissueInfo",
                                           make_column("id", &User::id, primary_key().autoincrement()),
                                           make_column("first_name", &User::firstName),
                                           make_column("last_name", &User::lastName),
                                           make_column("birth_date", &User::birthDate),
                                           make_column("image_url", &User::imageUrl),
                                           make_column("type_id", &User::typeId)));

    storage.sync_schema();


    User user{-1, "Asuna", "Doe", 664416000, std::make_unique<std::string>("url_to_heaven"), 3};

    auto insertedId = storage.insert(user);
    std::cout << "insertedId = " << insertedId << std::endl; //  insertedId = 8
    user.id = insertedId;

    User secondUser{-1, "Alice", "Inwonder", 831168000, {}, 2};
    insertedId = storage.insert(secondUser);
    secondUser.id = insertedId;

    auto a = ReadVideoFrameInfoListFromFile(
        R"(D:\Workspace\AutomaticPerfusionDataManager\TissueRecord\P00106_T001_R004_S019_B1\videoFrames\cell_1_2024-10-24_13-29-23-771.csv)");


    auto res = app.exec();
    Seele::Log::Shutdown();
    return res;
}

#include <QApplication>
#include <QScreen>

#include "ElaApplication.h"

int main(int argc, char* argv[])
{
    DataImporter importer;
    importer.importRecordFromFolder(R"(C:\Users\kiraYuukiAsuna\Desktop\TissueRecord)");


    QApplication app(argc, argv);
    eApp->init();
    MainWindow w;
    w.show();
    return app.exec();
}

