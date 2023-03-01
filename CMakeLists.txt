cmake_minimum_required(VERSION 3.5)

project(FileSystem LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check http://doc.qt.io/qt-5/deployment-android.html for more information.
# They need to be set before the find_package(Qt5 ...) call.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

find_package(Qt5 COMPONENTS
        Widgets
        Gui
        Core
        Sql
        REQUIRED)
qt5_add_resources(SOURCES res.qrc) # *.qrc文件

if (ANDROID)
    add_library(
            FileSystem SHARED
            main.cpp
            ${SOURCES}
#            ${app_WINRC}
#            logo.rc
            widget.cpp
            widget.h
            widget.ui
            myWidgets/myTitleBar/title_bar.cpp
            myWidgets/myTitleBar/title_bar.h
            myWidgets/myTitleBar/title_bar.ui
            myWidgets/myPushButton/mypushbutton.cpp
            myWidgets/myPushButton/mypushbutton.h
            myWidgets/myPushButton/mypushbutton.ui
            myWidgets/myLineEdit/mylineedit.cpp
            myWidgets/myLineEdit/mylineedit.h
            myWidgets/myLineEdit/mylineedit.ui
            myWidgets/myChangeableFrame/framelesswidget.h
            myWidgets/myChangeableFrame/framelesswidget.cpp
            myWidgets/myLogin/login.cpp
            myWidgets/myLogin/login.h
            myWidgets/myLogin/login.ui
            myWidgets/myMenuDialog/newbuild_menudialog.cpp
            myWidgets/myMenuDialog/newbuild_menudialog.h
            myWidgets/myMenuDialog/newbuild_menudialog.ui
            myWidgets/myItemProperty/itempropertydialog.cpp
            myWidgets/myItemProperty/itempropertydialog.h
            myWidgets/myItemProperty/itempropertydialog.ui
            myWidgets/myCreatePackageDialog/mycreatepackagedialog.cpp myWidgets/myCreatePackageDialog/mycreatepackagedialog.h myWidgets/myCreatePackageDialog/mycreatepackagedialog.ui myWidgets/mySettingDialog/settingdialog.cpp myWidgets/mySettingDialog/settingdialog.h myWidgets/mySettingDialog/settingdialog.ui)
else ()
    add_executable(
            FileSystem
            WIN32
            main.cpp
            ${SOURCES}
#            ${app_WINRC}
#            logo.rc
            widget.cpp
            widget.h
            widget.ui
            myWidgets/myTitleBar/title_bar.cpp
            myWidgets/myTitleBar/title_bar.h
            myWidgets/myTitleBar/title_bar.ui
            myWidgets/myPushButton/mypushbutton.cpp
            myWidgets/myPushButton/mypushbutton.h
            myWidgets/myPushButton/mypushbutton.ui
            myWidgets/myLineEdit/mylineedit.cpp
            myWidgets/myLineEdit/mylineedit.h
            myWidgets/myLineEdit/mylineedit.ui
            myWidgets/myChangeableFrame/framelesswidget.h
            myWidgets/myChangeableFrame/framelesswidget.cpp
            myWidgets/myLogin/login.cpp
            myWidgets/myLogin/login.h
            myWidgets/myLogin/login.ui
            myWidgets/myMenuDialog/newbuild_menudialog.cpp
            myWidgets/myMenuDialog/newbuild_menudialog.h
            myWidgets/myMenuDialog/newbuild_menudialog.ui
            myWidgets/myItemProperty/itempropertydialog.cpp
            myWidgets/myItemProperty/itempropertydialog.h
            myWidgets/myItemProperty/itempropertydialog.ui
            myWidgets/myCreatePackageDialog/mycreatepackagedialog.cpp myWidgets/myCreatePackageDialog/mycreatepackagedialog.h myWidgets/myCreatePackageDialog/mycreatepackagedialog.ui myWidgets/mySettingDialog/settingdialog.cpp myWidgets/mySettingDialog/settingdialog.h myWidgets/mySettingDialog/settingdialog.ui)
endif ()

target_link_libraries(FileSystem PRIVATE
        Qt5::Widgets
        Qt5::Core
        Qt5::Gui
        Qt5::Sql
        )
