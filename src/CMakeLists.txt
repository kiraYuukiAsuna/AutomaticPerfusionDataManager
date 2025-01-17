cmake_minimum_required(VERSION 3.20)
project(AutomaticPerfusionDataManager)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        Charts
        REQUIRED)

set(PROJECT_SOURCES
        main.cpp

        ../ThirdParty/sqlite3/sqlite3.c

        Backtrace/backward.hpp
        Backtrace/backward.cpp

        Log/EditorConsoleSink.h
        Log/Log.h
        Log/Log.cpp

        FileReader/Resource.hpp
        FileReader/CellMapInfo.hpp
        FileReader/CellTissueInfo.hpp
        FileReader/CellTrackInfo.hpp
        FileReader/NeedleTrackInfo.hpp
        FileReader/StateTimeDifferencesInfo.hpp
        FileReader/VideoFrameInfo.hpp

        UI/MainWindow.cpp
        UI/MainWindow.h

        DataImport/DataImporter.hpp

        Util/Util.hpp

        Database/SqliteSchema.hpp
        Database/CellInformationRepository.hpp

        UI/Pages/BasePage.cpp
        UI/Pages/BasePage.h
        UI/Pages/OverviewPage.h
        UI/Pages/OverviewPage.cpp
        UI/Pages/DataImportPage.h
        UI/Pages/DataImportPage.cpp
        UI/Pages/QueryByTimePage.cpp
        UI/Pages/QueryByTimePage.h
        UI/Pages/ConfigDialog.h
        UI/Pages/ConfigDialog.cpp

        Analysis/AnalysisBase.hpp
        Config/Config.hpp
)


add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

target_include_directories(${PROJECT_NAME}
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        "../ThirdParty/ElaWidgetTools/src/include"
        "../ThirdParty/magic_enum/include"
        "../ThirdParty/nlohmann_json/include"
        "../ThirdParty/rapidcsv/include"
        "../ThirdParty/spdlog/include"
        "../ThirdParty/sqlite3"
        "../ThirdParty/sqlite_orm/include/sqlite_orm"
        "../ThirdParty/vfspp/vendor/miniz-cpp"
        "../ThirdParty/vfspp/include"
        ../ThirdParty/asio/include
)

target_link_directories(${PROJECT_NAME}
        PRIVATE
        "ThirdParty"
)

target_link_libraries(${PROJECT_NAME}
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::Charts
        elawidgettools
)

if (WIN32 AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(DEBUG_SUFFIX)
    if (MSVC AND CMAKE_BUILD_TYPE MATCHES "Debug")
        set(DEBUG_SUFFIX "d")
    endif ()
    set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}")
    if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
        set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
            set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        endif ()
    endif ()
    if (EXISTS "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
    endif ()
    foreach (QT_LIB Core Gui Widgets)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/bin/Qt6${QT_LIB}${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
    endforeach (QT_LIB)
endif ()
