cmake_minimum_required(VERSION 3.20)
project(AutomaticPerfusionDataManager)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        Charts
        REQUIRED)

add_subdirectory("ThirdParty/ElaWidgetTools/src")

set(PROJECT_SOURCES
        main.cpp

        ThirdParty/sqlite3/sqlite3.c

        src/Backtrace/backward.hpp
        src/Backtrace/backward.cpp

        src/Log/EditorConsoleSink.h
        src/Log/Log.h
        src/Log/Log.cpp

        src/FileReader/Resource.hpp
        src/FileReader/CellMapInfo.hpp
        src/FileReader/CellTissueInfo.hpp
        src/FileReader/CellTrackInfo.hpp
        src/FileReader/NeedleTrackInfo.hpp
        src/FileReader/StateTimeDifferencesInfo.hpp
        src/FileReader/VideoFrameInfo.hpp

        src/UI/MainWindow.cpp
        src/UI/MainWindow.h

        src/DataImport/DataImporter.hpp

        src/Util/Util.hpp

        src/Database/SqliteSchema.hpp
        src/UI/Pages/BasePage.cpp
        src/UI/Pages/BasePage.h
        src/UI/Pages/OverviewPage.cpp
        src/UI/Pages/OverviewPage.h

)


add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} )

target_include_directories(${PROJECT_NAME}
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        src
        ThirdParty/ElaWidgetTools/src/include
        ThirdParty/magic_enum/include
        ThirdParty/nlohmann_json/include
        ThirdParty/rapidcsv/include
        ThirdParty/spdlog/include
        ThirdParty/sqlite3
        ThirdParty/sqlite_orm/include/sqlite_orm
        ThirdParty/vfspp/vendor/miniz-cpp
        ThirdParty/vfspp/include
)

target_link_directories(${PROJECT_NAME}
        PUBLIC
        ThirdParty
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
