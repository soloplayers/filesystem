#include <QLabel>
#include <QPainter>
#include "./ui_widget.h"
#include "QPixmap"
#include "QDesktopWidget"
#include <QDir>
#include "QStorageInfo"
#include <QBoxLayout>
#include <QDebug>
#include <QMouseEvent>
#include <QScrollBar>
#include <QEvent>
#include <qt_windows.h>
#include <Windowsx.h>
#include <QWidget>
#include "QImage"
#include <QFile>
#include <QScrollArea>
#include <QStandardPaths>
#include <QStackedWidget>
#include <QMenu>
#include <QAction>
#include <QMenuBar>
#include <QtMath>
#include <QGraphicsDropShadowEffect>
#include <QFileSystemModel>
#include <QTableView>
#include <QHeaderView>
#include <shlobj.h>
#include <QMessageBox>
#include <QDateTime>
#include <QFileIconProvider>
#include <QDesktopServices>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>
#include <QFileDialog>
#include <QIODevice>
#include "widget.h"
#include "myWidgets/myTitleBar/title_bar.h"
#include "myWidgets/myLineEdit/mylineedit.h"
#include "myWidgets/myMenuDialog/newbuild_menudialog.h"
#include "myWidgets/myItemProperty/itempropertydialog.h"
#include "myWidgets/myChangeableFrame/framelesswidget.h"
#include "myWidgets/myCreatePackageDialog/mycreatepackagedialog.h"
#include "myWidgets/myPushButton/mypushbutton.h"
#include "myWidgets/mySettingDialog/settingdialog.h"


Widget::Widget(QWidget *parent)
        : QWidget(parent), ui(new Ui::Widget) {
    ui->setupUi(this);
    auto *pTitleBar = new TitleBar(this);
    installEventFilter(pTitleBar);
    //进行标题栏界面布局
    auto *titleBarLayout = new QVBoxLayout;
    titleBarLayout->addWidget(pTitleBar);
    titleBarLayout->addStretch();
    titleBarLayout->setSpacing(0);
    titleBarLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(titleBarLayout);

    installEventFilter(this);
    auto dsw = QApplication::desktop();  //获取电脑的桌面事件
    setGeometry((dsw->width() - dsw->width() * 4 / 5) / 2, (dsw->height() - dsw->height() * 4 / 5) / 2,
                dsw->width() * 4 / 5, dsw->height() * 4 / 5);
    setStyleSheet("QWidget{font-family:kaiTi;color:white;}");
    setWindowTitle("文件子系统");
    // 设置无边框窗口，只填一个type类型，不然后面窗口无法进行放缩 + 设置鼠标追踪
    setMouseTracking(true);
    // 设置窗口类型，与windows应用程序类型匹配
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint |
                   Qt::WindowMaximizeButtonHint);
    setWindowIcon(QPixmap(":/res/myTitleBar_imgs/fileLogo.png"));
    setMinimumSize(600, 400);
    // 设置成可缩放的窗体
    auto *ResizeableWidget = new FramelessWidget(this);
    ResizeableWidget->setResizeEnable(true);

    // 初始化值
    //m_nBorder表示鼠标位于边框缩放范围的宽度，可以设置为3
    m_nBorderWidth = 3;
    // 默认值设为true
    leftLayout_flag_collection = true;
    leftLayout_flag_driver = true;
    leftLayout_flag_oneDrive = true;
    leftLayout_flag_network = true;
    rightLayout_flag_showFolders = true;
    rightLayout_flag_showDrivers = true;
    rightLayout_flag_showPackages = true;
    rightLayout_flag_showRecentUsing = true;
    manageItems_menu_toggleFolders_flag = true;
    manageItems_menu_toggleDrivers_flag = true;
    manageItems_menu_togglePackages_flag = true;
    manageItems_menu_toggleRecentUsing_flag = true;
    for (int i = 0; i < 1000; ++i) {
        leftScrollAreaSelectedBtn[i] = nullptr;
        leftScrollAreaSelectedBtn_selected_flag[i] = false;
    }
    // 默认的固定按钮有11个：TODO 还要解决count值，动态处理加上添加的收藏文件夹个数的值
    leftScrollAreaSelectedBtn_count = 11;
    // 默认添加的收藏文件夹数为0；
    addCollectionFolders = 0;
    // 设置absolutePathIterator的下标索引值为0
    absolutePathIterator_index = 0;
    // 索引栈容器的下标 默认从3开始
    stackedWidget_index = 3;
    tempSize = 0;
    // 布局行列值初始化为0
    packageWrapperStoredWidgetGridLayout_row = 0;
    packageWrapperStoredWidgetGridLayout_col = 0;



    // 旁支内容的宏观布局
    asideWidget = new QWidget(this);
    // 标题栏下方的一行的背景色
    asideWidget->setStyleSheet("background-color:rgb(25,25,25)");
    // 左widget
    auto *asideWidgetHLayout_leftWidget = new QWidget;
    asideWidgetHLayout_leftWidget->setFixedSize(200, 42);
    // 右widget
    auto *asideWidgetHLayout_rightWidget = new QWidget;
    // 唯一一个设置按钮，
    auto *setting = new myPushButton(6, "", ":/res/mainScene_imgs/setting_white.png");
    setting->setFixedSize(45, 40);
    setting->setIconSize(QSize(28, 28));
    setting->setToolTip("设置");

    // 设置setting一栏的菜单(伪菜单) TODO
    connect(setting, &myPushButton::clicked, [=]() {
        auto *sDialog = new settingDialog;
        sDialog->show();
    });

    // 采用水平布局
    auto *asideWidgetHLayout = new QHBoxLayout;
    // 左边的按钮组也采用水平布局
    auto *asideWidgetHLayout_left = new QHBoxLayout;
    // 右边的lineEdit组也采用水平布局
    auto *asideWidgetHLayout_right = new QHBoxLayout;

    // 进行旁支内容左边界面布局
    auto *asideBtn_top = new myPushButton(3);
    auto *asideBtn_repaint = new myPushButton(3);
    auto *asideBtn_left = new myPushButton(3);
    auto *asideBtn_right = new myPushButton(3);
    // 设置按钮的样式
    asideBtn_repaint->setStyleSheet("myPushButton{border:none}"
                                    "myPushButton:hover{background-color:rgb(60,60,60);border-radius:6px;}"
                                    "myPushButton:pressed{background-color:rgb(50,50,50);}");
    asideBtn_top->setStyleSheet("myPushButton{border:none}"
                                "myPushButton:hover{background-color:rgb(60,60,60);border-radius:6px;}"
                                "myPushButton:pressed{background-color:rgb(50,50,50);}");
    asideBtn_left->setStyleSheet("myPushButton{border:none}"
                                 "myPushButton:hover{background-color:rgb(60,60,60);border-radius:6px;}"
                                 "myPushButton:pressed{background-color:rgb(50,50,50);}");
    asideBtn_right->setStyleSheet("myPushButton{border:none}"
                                  "myPushButton:hover{background-color:rgb(60,60,60);border-radius:6px;}"
                                  "myPushButton:pressed{background-color:rgb(50,50,50);}");
    asideBtn_repaint->setIcon(QIcon(":/res/mainScene_imgs/refresh_white.png"));
    asideBtn_top->setIcon(QIcon(":/res/mainScene_imgs/direction_top_gray.png"));
    asideBtn_left->setIcon(QIcon(":/res/mainScene_imgs/direction_left_gray.png"));
    asideBtn_right->setIcon(QIcon(":/res/mainScene_imgs/direction_right_gray.png"));
    asideBtn_top->setFixedSize(45, 40);
    asideBtn_repaint->setFixedSize(45, 40);
    asideBtn_left->setFixedSize(45, 40);
    asideBtn_right->setFixedSize(45, 40);
    asideBtn_top->setIconSize(QSize(45, 25));
    asideBtn_repaint->setIconSize(QSize(25, 25));
    asideBtn_left->setIconSize(QSize(45, 25));
    asideBtn_right->setIconSize(QSize(45, 25));

    // 设置提示信息
    asideBtn_left->setToolTip("后退");
    asideBtn_right->setToolTip("前进");
    asideBtn_top->setToolTip("返回上一级目录");
    asideBtn_repaint->setToolTip("刷新");
    // 默认设置三个按钮是禁用的
    asideBtn_top->setEnabled(false);
    asideBtn_left->setEnabled(false);
    asideBtn_right->setEnabled(false);

    // 绑定四个按钮事件
    connect(asideBtn_repaint, &myPushButton::clicked, [=]() {
        fileTableView->repaint();
    });
    connect(asideBtn_top, &myPushButton::clicked, [=]() {
        if (absolutePath == "D:\\$RECYCLE.BIN\\S-1-5-21-1781964007-3411722057-4230829593-1001" ||
            absolutePath == "主页：" || absolutePath == "网络：") {
            asideBtn_top->setEnabled(false);
        } else {
            QFileInfo temp = QFileInfo(absolutePath);
            if (temp.fileName() == "D" || temp.fileName() == "C") {

            } else {
                absolutePath = absolutePath.chopped(temp.fileName().count() + 1);
                asideLineEdit_left->setText(absolutePath);
                fileTableView->setRootIndex(fileModel->index(absolutePath));
                rightStackedWidget->setCurrentIndex(1);
            }

        }
    });
    // 回收站是setCurrentIndex=1；
    connect(asideBtn_left, &myPushButton::clicked, [=]() {
        // 后退逻辑
        absolutePathIterator_index--; //先减一
        absolutePath = absolutePathIterator.at(absolutePathIterator_index % absolutePathIterator.count());
        if (absolutePathIterator_index % absolutePathIterator.count() == 0) {
            rightStackedWidget->setCurrentIndex(0);
            asideLineEdit_left->setText(absolutePath);
            asideBtn_left->setEnabled(false);
            asideBtn_left->setIcon(QIcon(":/res/mainScene_imgs/direction_left_gray.png"));
        } else if (absolutePath == "D:\\$RECYCLE.BIN\\S-1-5-21-1781964007-3411722057-4230829593-1001") {
            asideLineEdit_left->setText("回收站内容");
            fileTableView->setRootIndex(fileModel->index(absolutePath));
            rightStackedWidget->setCurrentIndex(1);
        } else if (absolutePath == "网络：") {
            asideLineEdit_left->setText("网络：");
            rightStackedWidget->setCurrentIndex(2);
        } else if (absolutePath == "主页：") {
            asideLineEdit_left->setText("主页：");
            rightStackedWidget->setCurrentIndex(0);
        } else {
            asideLineEdit_left->setText(absolutePath);
            fileTableView->setRootIndex(fileModel->index(absolutePath));
            rightStackedWidget->setCurrentIndex(1);
        }
        // 根据absolutePath的内容将对应的按钮设置为选中状态
        setEnableBtnFromAbsolutePath();
        // 重置状态信息
        FilesAndDirs_count();
        asideBtn_right->setEnabled(true);
        asideBtn_right->setIcon(QIcon(":/res/mainScene_imgs/direction_right_white.png"));
    });
    connect(asideBtn_right, &myPushButton::clicked, [=]() {
        // 前进逻辑
        absolutePathIterator_index++; //先减一
        absolutePath = absolutePathIterator.at(absolutePathIterator_index % absolutePathIterator.count());
        if (absolutePathIterator_index % absolutePathIterator.count() == absolutePathIterator.count() - 1) {
            if (absolutePath == "D:\\$RECYCLE.BIN\\S-1-5-21-1781964007-3411722057-4230829593-1001") {
                asideLineEdit_left->setText("回收站内容");
                fileTableView->setRootIndex(fileModel->index(absolutePath));
                rightStackedWidget->setCurrentIndex(1);
            } else if (absolutePath == "网络：") {
                asideLineEdit_left->setText("网络：");
                rightStackedWidget->setCurrentIndex(2);
            } else if (absolutePath == "主页：") {
                asideLineEdit_left->setText("主页：");
                rightStackedWidget->setCurrentIndex(0);
            } else {
                asideLineEdit_left->setText(absolutePath);
                fileTableView->setRootIndex(fileModel->index(absolutePath));
                rightStackedWidget->setCurrentIndex(1);
            }
            asideBtn_right->setEnabled(false);
            asideBtn_right->setIcon(QIcon(":/res/mainScene_imgs/direction_right_gray.png"));
        } else if (absolutePath == "D:\\$RECYCLE.BIN\\S-1-5-21-1781964007-3411722057-4230829593-1001") {
            asideLineEdit_left->setText("回收站内容");
            fileTableView->setRootIndex(fileModel->index(absolutePath));
            rightStackedWidget->setCurrentIndex(1);
        } else if (absolutePath == "网络：") {
            asideLineEdit_left->setText("网络：");
            rightStackedWidget->setCurrentIndex(2);
        } else {
            asideLineEdit_left->setText(absolutePath);
            fileTableView->setRootIndex(fileModel->index(absolutePath));
            rightStackedWidget->setCurrentIndex(1);
        }
        // 重置状态信息
        FilesAndDirs_count();
        asideBtn_left->setEnabled(true);
        asideBtn_left->setIcon(QIcon(":/res/mainScene_imgs/direction_left_white.png"));
        setEnableBtnFromAbsolutePath();
    });
    // 添加组件
    asideWidgetHLayout_left->addSpacing(10);
    asideWidgetHLayout_left->addWidget(asideBtn_left);
    asideWidgetHLayout_left->addWidget(asideBtn_right);
    asideWidgetHLayout_left->addWidget(asideBtn_repaint);
    asideWidgetHLayout_left->addWidget(asideBtn_top);
    asideWidgetHLayout_left->addStretch();

    asideWidgetHLayout_leftWidget->setLayout(asideWidgetHLayout_left);
    asideWidgetHLayout_left->setMargin(0);
    asideWidgetHLayout_right->setMargin(0);
    asideWidgetHLayout->addWidget(asideWidgetHLayout_leftWidget);
    asideWidgetHLayout->addWidget(asideWidgetHLayout_rightWidget);
    asideWidgetHLayout->addWidget(setting);
    asideWidgetHLayout->addSpacing(10);
    asideWidgetHLayout->setMargin(0);
    asideWidget->setLayout(asideWidgetHLayout);


    // 进行旁支内容中心的的两个文本框布局
    asideLineEdit_left = new myLineEdit;
    asideLineEdit_right = new myLineEdit;


    // 设置默认样式
    asideLineEdit_left->setTextMargins(5, 0, 0, 0);
    asideLineEdit_right->setTextMargins(5, 0, 0, 0);
    asideLineEdit_left->setText("主页:");
    asideLineEdit_right->setPlaceholderText("从当前位置搜索:");
    asideLineEdit_left->setStyleSheet(
            "border-top:0px solid;"
            "border-bottom:1px  solid rgb(154, 154, 154);"
            "border-left:0px  solid;"
            "border-right: 0px solid;"
            "color:rgb(255, 255, 255);"
            "font-size:18px;"
            "border-radius: 4px;"
            "background-color:rgb(50, 50, 50)"
    );
    asideLineEdit_right->setStyleSheet(
            "border-top:0px solid;"
            "border-bottom:1px  solid rgb(154, 154, 154);"
            "border-left:0px  solid;"
            "border-right: 0px solid;"
            "color:rgb(255, 255, 255);"
            "font-size: 18px;"
            "border-radius: 4px;"
            "background-color:rgb(50, 50, 50)"
    );
    asideLineEdit_left->setFixedHeight(40);
    asideLineEdit_right->setMaximumSize(400, 40);
    asideWidgetHLayout_right->addWidget(asideLineEdit_left);
    asideWidgetHLayout_right->addWidget(asideLineEdit_right);
    asideWidgetHLayout_rightWidget->setLayout(asideWidgetHLayout_right);
    // 为两个文本框绑定事件
    connect(asideLineEdit_left, &myLineEdit::returnPressed, [=]() {
        absolutePath = asideLineEdit_left->text().toUtf8();
        fileTableView->setRootIndex(fileModel->index(asideLineEdit_left->text().toUtf8().data()));
        rightStackedWidget->setCurrentIndex(1);
    });
    // 搜索一栏相关
    connect(asideLineEdit_right, &myLineEdit::returnPressed, [=]() {
        QFile searchScrollAreaBar(":/styleSheets/searchScrollAreaBar.qss");
        if (!searchScrollAreaBar.open(QIODevice::ReadOnly)) {
            qDebug() << "文件打开失败！";
        }
        QByteArray qssContent = searchScrollAreaBar.readAll();
        searchScrollAreaBar.close();
        auto *tempScrollBar = new QScrollBar;
        tempScrollBar->setStyleSheet(qssContent);

        auto *searchScrollArea = new QScrollArea;
        auto *searchScrollAreaVLayout = new QVBoxLayout;
        auto *searchScrollAreaWidgetMask = new QWidget;
        searchScrollArea->setVerticalScrollBar(tempScrollBar);
        searchFolders.clear();
        searchFiles.clear();
        if (asideLineEdit_left->text() == "主页：" || asideLineEdit_left->text() == "回收站内容" ||
            asideLineEdit_left->text() == "网络：") {
            // 在这些情况下，默认搜索D盘，但是非常费时间（D盘太大了）
            searchFilesAndFolders("D:/", asideLineEdit_right->text());
            // 先处理文件夹
            for (const auto &i: searchFolders) {
                QFileInfo info = QFileInfo(i);
                auto *pItem = new myPushButton(13, i);
                pItem->setFixedSize(rightWidget->width(), 60);
                connect(pItem, &myPushButton::clicked, [=]() {
                    asideLineEdit_left->setText(info.filePath());
                    asideLineEdit_left->returnPressed();
                });
                searchScrollAreaVLayout->addWidget(pItem);
            }
            // 再处理文件
            for (const auto &i: searchFiles) {
                QFileInfo info = QFileInfo(i);
                auto *pItem = new myPushButton(13, i);
                pItem->setFixedSize(rightWidget->width(), 60);
                connect(pItem, &myPushButton::clicked, [=]() {
                    asideLineEdit_left->setText(info.filePath().chopped(info.fileName().count()));
                    asideLineEdit_left->returnPressed();
                });
                searchScrollAreaVLayout->addWidget(pItem);
            }

        } else {
            searchFilesAndFolders(absolutePath, asideLineEdit_right->text());
            // 先处理文件夹
            for (const auto &i: searchFolders) {
                QFileInfo info = QFileInfo(i);
                auto *pItem = new myPushButton(13, i);
                pItem->setFixedSize(rightWidget->width(), 60);
                connect(pItem, &myPushButton::clicked, [=]() {
                    asideLineEdit_left->setText(info.filePath());
                    asideLineEdit_left->returnPressed();
                });
                searchScrollAreaVLayout->addWidget(pItem);
            }
            // 再处理文件
            for (const auto &i: searchFiles) {
                QFileInfo info = QFileInfo(i);
                auto *pItem = new myPushButton(13, i);
                pItem->setFixedSize(rightWidget->width(), 60);
                connect(pItem, &myPushButton::clicked, [=]() {
                    asideLineEdit_left->setText(info.filePath().chopped(info.fileName().count()));
                    asideLineEdit_left->returnPressed();
                    QDesktopServices::openUrl(QUrl::fromLocalFile(info.filePath()));
                });
                searchScrollAreaVLayout->addWidget(pItem);
            }

        }
        searchScrollAreaWidgetMask->setLayout(searchScrollAreaVLayout);
        searchScrollArea->setWidget(searchScrollAreaWidgetMask);
        searchScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        rightStackedWidget->addWidget(searchScrollArea);    // index==3
        rightStackedWidget->setCurrentIndex(stackedWidget_index++);

    });

    // 进行左边和中心界面的widget区域布局
    mainWidget = new QWidget(this);

    // 主要布局
    auto *mainLayout = new QHBoxLayout;
    // 左边布局
    auto *leftScrollArea = new QScrollArea;
    auto *leftVLayout = new QVBoxLayout;
    auto *leftMask = new QWidget;

    // 右边布局
    rightWidget = new QWidget;
    rightStackedWidget = new QStackedWidget;
    auto *fixedToolWidget = new QWidget;
    auto *rightScrollArea = new QScrollArea;
    auto *rightWidgetVLayout = new QVBoxLayout;
    auto *rightScrollAreaVLayout = new QVBoxLayout;
    auto *rightMask = new QWidget;

    // 左边布局开始
    // 为leftScrollArea添加组件设置垂直布局
    // 收藏夹的整个布局
    leftScrollAreaSelectedBtn[0] = new myPushButton(4, "收藏夹", ":/res/leftScrollArea_imgs/collection.png");
    leftScrollAreaSelectedBtn[0]->setFixedSize(352, 50);
    // 收藏夹栏的窗口
    auto *collectionWidget = new QWidget;
    collectionWidget->setFixedSize(360, 250);

    // 控制收藏夹栏的下拉窗口是否展示
    connect(leftScrollAreaSelectedBtn[0], &myPushButton::clicked, [=]() {
        if (leftLayout_flag_collection) {
            leftScrollAreaSelectedBtn[0]->setSelectedBtn(4, false);
            collectionWidget->setVisible(false);
            leftLayout_flag_collection = false;
        } else {
            leftScrollAreaSelectedBtn[0]->setSelectedBtn(4, true);
            leftLayout_flag_collection = true;
            collectionWidget->setVisible(true);
        }
    });

    // 主页一栏
    leftScrollAreaSelectedBtn[1] = new myPushButton(5, "主页", ":/res/leftScrollArea_imgs/homePage.png");
    leftScrollAreaSelectedBtn[1]->setFixedSize(350, 45);
    // 桌面一栏
    leftScrollAreaSelectedBtn[2] = new myPushButton(5, "桌面", ":/res/leftScrollArea_imgs/desktop.png");
    leftScrollAreaSelectedBtn[2]->setFixedSize(350, 45);

    // 下载一栏
    leftScrollAreaSelectedBtn[3] = new myPushButton(5, "下载", ":/res/leftScrollArea_imgs/download.png");
    leftScrollAreaSelectedBtn[3]->setFixedSize(350, 45);
    // 文档一栏
    leftScrollAreaSelectedBtn[4] = new myPushButton(5, "文档", ":/res/leftScrollArea_imgs/document.png");
    leftScrollAreaSelectedBtn[4]->setFixedSize(350, 45);
    // 回收站一栏
    leftScrollAreaSelectedBtn[5] = new myPushButton(5, "回收站", ":/res/leftScrollArea_imgs/recovery.png");
    leftScrollAreaSelectedBtn[5]->setFixedSize(350, 45);

    // 驱动器一栏 (还能优化，先做其他的功能)
    leftScrollAreaSelectedBtn[6] = new myPushButton(4, "驱动器", ":/res/leftScrollArea_imgs/computer.png");
    leftScrollAreaSelectedBtn[6]->setFixedSize(352, 50);
    leftScrollAreaSelectedBtn[6]->setSelectedBtn(4, false);
    auto *driversWidget = new QWidget;
    driversWidget->setVisible(false);
    // 控制驱动器一栏的下拉窗口是否展示
    connect(leftScrollAreaSelectedBtn[6], &myPushButton::clicked, [=]() {
        if (leftLayout_flag_driver) {
            leftScrollAreaSelectedBtn[6]->setSelectedBtn(4, true);
            driversWidget->setVisible(true);
            leftLayout_flag_driver = false;
        } else {
            leftScrollAreaSelectedBtn[6]->setSelectedBtn(4, false);
            driversWidget->setVisible(false);
            leftLayout_flag_driver = true;
        }
    });
    // 驱动器的widget布局
    auto *driverWidgetVLayout = new QVBoxLayout;
    driverWidgetVLayout->setMargin(0);

    // 获取电脑的驱动器信息
    auto driverList = QStorageInfo::mountedVolumes();
    leftScrollAreaSelectedBtn_count += driverList.count();
    driversWidget->setFixedSize(360, 50 * (driverList.count()));
    for (int i = 0; i < driverList.count(); ++i) {
        // 驱动器C盘一栏
        if (driverList.at(i).isRoot()) {
            leftScrollAreaSelectedBtn[i + 7] = new myPushButton(5, QString("本地磁盘:(%1)").arg(
                                                                        driverList.at(i).displayName()),
                                                                ":/res/leftScrollArea_imgs/driver_C.png");
            leftScrollAreaSelectedBtn[i + 7]->setFixedSize(360, 45);
            driverWidgetVLayout->addWidget(leftScrollAreaSelectedBtn[i + 7]);
        }
            // 驱动器其他一栏
        else {
            leftScrollAreaSelectedBtn[i + 7] = new myPushButton(5,
                                                                QString("本地磁盘:(%1)").arg(driverList.at(i).rootPath()),
                                                                ":/res/leftScrollArea_imgs/driver_other.png");
            leftScrollAreaSelectedBtn[i + 7]->setFixedSize(360, 45);
            driverWidgetVLayout->addWidget(leftScrollAreaSelectedBtn[i + 7]);
        }
    }

    // 云端驱动器oneDrive一栏
    leftScrollAreaSelectedBtn[driverList.count() + 6 + 1] = new myPushButton(4, "OneDrive",
                                                                             ":/res/leftScrollArea_imgs/oneDriveFolder.png");
    leftScrollAreaSelectedBtn[driverList.count() + 6 + 1]->setSelectedBtn(4, false);
    leftScrollAreaSelectedBtn[driverList.count() + 6 + 1]->setFixedSize(352, 50);
    auto *oneDriveWidget = new QWidget;
    oneDriveWidget->setFixedSize(360, 50);
    leftScrollAreaSelectedBtn[driverList.count() + 6 + 2] = new myPushButton(5, "OneDrive",
                                                                             ":/res/leftScrollArea_imgs/oneDrive.png",
                                                                             0, 0, oneDriveWidget);

    leftScrollAreaSelectedBtn[driverList.count() + 6 + 2]->setFixedSize(360, 45);

    oneDriveWidget->setVisible(false);
    // 为btn_oneDrive绑定事件
    connect(leftScrollAreaSelectedBtn[driverList.count() + 6 + 1], &myPushButton::clicked, [=]() {
        if (leftLayout_flag_oneDrive) {
            leftScrollAreaSelectedBtn[driverList.count() + 6 + 1]->setSelectedBtn(4, true);
            oneDriveWidget->setVisible(true);
            leftLayout_flag_oneDrive = false;
        } else {
            leftScrollAreaSelectedBtn[driverList.count() + 6 + 1]->setSelectedBtn(4, false);
            oneDriveWidget->setVisible(false);
            leftLayout_flag_oneDrive = true;
        }
    });

    // 网络一栏
    leftScrollAreaSelectedBtn[driverList.count() + 6 + 3] = new myPushButton(4, "网络",
                                                                             ":/res/leftScrollArea_imgs/network.png");
    leftScrollAreaSelectedBtn[driverList.count() + 6 + 3]->setSelectedBtn(4, false);
    leftScrollAreaSelectedBtn[driverList.count() + 6 + 3]->setFixedSize(352, 50);
    auto *networkWidget = new QWidget;
    networkWidget->setFixedSize(360, 50);
    leftScrollAreaSelectedBtn[driverList.count() + 6 + 4] = new myPushButton(5, "网络",
                                                                             ":/res/leftScrollArea_imgs/folder.png",
                                                                             0, 0, networkWidget);
    leftScrollAreaSelectedBtn[driverList.count() + 6 + 4]->setFixedSize(360, 45);
    //为网络一栏绑定事件
    networkWidget->setVisible(false);
    connect(leftScrollAreaSelectedBtn[driverList.count() + 6 + 3], &myPushButton::clicked, [=]() {
        if (leftLayout_flag_network) {
            leftScrollAreaSelectedBtn[driverList.count() + 6 + 3]->setSelectedBtn(4, true);
            networkWidget->setVisible(true);
            leftLayout_flag_network = false;
        } else {
            leftScrollAreaSelectedBtn[driverList.count() + 6 + 3]->setSelectedBtn(4, false);
            networkWidget->setVisible(false);
            leftLayout_flag_network = true;
        }
    });

    // 为收藏夹栏窗口添加组件+绑定事件
    // 接下来是优化按钮的绑定事件，点击后背景色变深+出现竖直的蓝色线
    auto *collectionWidgetVLayout = new QVBoxLayout;
    collectionWidgetVLayout->setMargin(0);
    // 收藏夹一个大栏的处理(刚好可以和添加组件事件一起处理，相当于节省空间)
    for (int i = 1; i <= 5; ++i) {
        collectionWidgetVLayout->addWidget(leftScrollAreaSelectedBtn[i]);
        connect(leftScrollAreaSelectedBtn[i], &myPushButton::clicked, [=]() {
            changFlagToTrueWithOtherFlagFalse(i);
            showSelectedBtn(leftScrollAreaSelectedBtn_selected_flag);
        });
    }


    // 驱动器一个大栏的处理 TODO 应该动态的从json文件中获取addCollectionFolders的值
    for (int i = addCollectionFolders + 6 + 1; i < driverList.count() + addCollectionFolders + 6 + 1; ++i) {
        connect(leftScrollAreaSelectedBtn[i], &myPushButton::clicked, [=]() {
            changFlagToTrueWithOtherFlagFalse(i);
            showSelectedBtn(leftScrollAreaSelectedBtn_selected_flag);
        });
    }

    // oneDrive一个大栏的处理(只有一个按钮)
    connect(leftScrollAreaSelectedBtn[5 + addCollectionFolders + 1 + driverList.count() + 1 + 1],
            &myPushButton::clicked, [=]() {
                changFlagToTrueWithOtherFlagFalse(5 + addCollectionFolders + 1 + driverList.count() + 1 + 1);
                showSelectedBtn(leftScrollAreaSelectedBtn_selected_flag);
            });

    // 网络一个大栏的处理（只有一个按钮）
    connect(leftScrollAreaSelectedBtn[5 + addCollectionFolders + 1 + driverList.count() + 1 + 1 + 1 + 1],
            &myPushButton::clicked, [=]() {
                changFlagToTrueWithOtherFlagFalse(5 + addCollectionFolders + 1 + driverList.count() + 1 + 1 + 1 + 1);
                showSelectedBtn(leftScrollAreaSelectedBtn_selected_flag);
            });

    // 将布局放在每个窗口上
    collectionWidget->setLayout(collectionWidgetVLayout);
    driversWidget->setLayout(driverWidgetVLayout);


    // 左侧布局添加控件
    leftVLayout->addWidget(leftScrollAreaSelectedBtn[0]);
    leftVLayout->addWidget(collectionWidget);
    leftVLayout->addWidget(leftScrollAreaSelectedBtn[6]);
    leftVLayout->addWidget(driversWidget);
    leftVLayout->addWidget(leftScrollAreaSelectedBtn[driverList.count() + 6 + 1]);
    leftVLayout->addWidget(oneDriveWidget);
    leftVLayout->addWidget(leftScrollAreaSelectedBtn[driverList.count() + 6 + 3]);
    leftVLayout->addWidget(networkWidget);
    leftVLayout->setMargin(0);

    // 在最底部添加一个弹簧，防止页面弹出时有一上一下的bug
    leftVLayout->addStretch();
    leftScrollArea->setWidgetResizable(true);
    leftScrollArea->setWidget(leftMask);
    leftMask->setLayout(leftVLayout);

    // 右边布局开始
    // 先布局固定的工具窗口
    auto *fixedToolWidgetHLayout = new QHBoxLayout;
    auto *fixedToolWidgetHLayoutInnerWidget = new QWidget;
    auto *fixedToolWidgetHLayoutInnerWidgetHLayout = new QHBoxLayout;
    auto *fixedToolWidgetHLayoutInnerWidgetHLayout_left = new QHBoxLayout;
    auto *fixedToolWidgetHLayoutInnerWidgetHLayout_right_normal = new QHBoxLayout;
    auto *fixedToolWidgetHLayoutInnerWidgetHLayout_right_homePage = new QHBoxLayout;

    // 为固定的工具窗口添加组件
    newBuild = new myPushButton(7, "新建", ":/res/fixedToolWidget_imgs/newBuild.png");
    newBuild->setFixedSize(100, 50);
    newBuild->setToolTip("新建文件或文件夹");



    // 为固定工具窗口的右边窗口的左边添加组件
    // 剪切、复制、粘贴、重命名、共享、…(表示更多，包括删除和属性)
    cut = new myPushButton(6, "剪切", ":/res/fixedToolWidget_imgs/cut.png");
    copy = new myPushButton(6, "复制", ":/res/fixedToolWidget_imgs/copy.png");
    paste = new myPushButton(6, "粘贴", ":/res/fixedToolWidget_imgs/paste.png");
    rename = new myPushButton(6, "重命名", ":/res/fixedToolWidget_imgs/rename.png");
    deleteTarget = new myPushButton(6, "删除", ":/res/fixedToolWidget_imgs/delete.png");
    itemProperty = new myPushButton(6, "属性", ":/res/fixedToolWidget_imgs/property.png");

    // 设置固定大小
    cut->setFixedSize(50, 50);
    copy->setFixedSize(50, 50);
    paste->setFixedSize(50, 50);
    rename->setFixedSize(50, 50);
    deleteTarget->setFixedSize(50, 50);
    itemProperty->setFixedSize(50, 50);

    // 添加组件
    fixedToolWidgetHLayoutInnerWidgetHLayout_left->setMargin(0);
    fixedToolWidgetHLayoutInnerWidgetHLayout_left->addWidget(cut);
    fixedToolWidgetHLayoutInnerWidgetHLayout_left->addWidget(copy);
    fixedToolWidgetHLayoutInnerWidgetHLayout_left->addWidget(paste);
    fixedToolWidgetHLayoutInnerWidgetHLayout_left->addWidget(rename);
    fixedToolWidgetHLayoutInnerWidgetHLayout_left->addWidget(deleteTarget);
    fixedToolWidgetHLayoutInnerWidgetHLayout_left->addWidget(itemProperty);


    // 为固定工具窗口的右边窗口的右边添加组件
    // 选项、排序、布局、切换预览面板、…(表示更多，包括重新打开该应用（多开）和在cmd打开等)
    // 区分一个homePage和其他页面的布局

    // homePage布局
    auto *mangeItems = new myPushButton(6, "管理小部件", ":/res/fixedToolWidget_imgs/manageItems.png");
//    auto *rightHLayout_more = new myPushButton(6, "查看更多", ":/res/fixedToolWidget_imgs/more.png");

    // 设置固定大小
    mangeItems->setFixedSize(50, 50);
//    rightHLayout_more->setFixedSize(50, 50);

    // 添加组件
    fixedToolWidgetHLayoutInnerWidgetHLayout_right_homePage->addStretch();
    fixedToolWidgetHLayoutInnerWidgetHLayout_right_homePage->addWidget(mangeItems);
//    fixedToolWidgetHLayoutInnerWidgetHLayout_right_homePage->addWidget(rightHLayout_more);

    // 为管理小部件设置菜单
    auto *manageItems_menu = new QMenu;
    auto *manageItems_menu_title = new QAction("管理小部件");
    auto *manageItems_menu_toggleFolders = new QAction("隐藏文件夹小部件");
    auto *manageItems_menu_toggleDrivers = new QAction("隐藏驱动器小部件");
    auto *manageItems_menu_togglePackages = new QAction("隐藏包小部件");
    auto *manageItems_menu_toggleRecentUsing = new QAction("隐藏最近使用小部件");
    manageItems_menu->setStyleSheet(
            "QMenu{background-color:rgb(60,60,60);color:white;border-radius:3px;padding-top:4px;}"
            "QMenu::item{border-radius:3px;height:38px;min-width:1px;margin:10px;}"
            "QMenu::item:selected{background-color:rgb(119, 179, 226);}");
    manageItems_menu->setFont(QFont("kaiTi", 12));
    manageItems_menu->setFixedSize(240, 320);
    manageItems_menu->setTitle("管理小部件");

    manageItems_menu->addAction(manageItems_menu_title);
    manageItems_menu_title->setEnabled(false);
    manageItems_menu->addSeparator();
    manageItems_menu->addAction(manageItems_menu_toggleFolders);
    manageItems_menu->addAction(manageItems_menu_toggleDrivers);
    manageItems_menu->addAction(manageItems_menu_togglePackages);
    manageItems_menu->addAction(manageItems_menu_toggleRecentUsing);
    mangeItems->setMenu(manageItems_menu);



    // 主要布局是 rightScrollAreaVLayout（最外层）
    // 展示文件夹栏
    auto *rightScrollAreaVLayout_showFolders_VLayoutWidget = new QWidget;
    auto *rightScrollAreaVLayout_showFolders_VLayout = new QVBoxLayout;
    auto *rightScrollAreaVLayout_showFolders_VLayout_topBtn = new myPushButton(8, "文件夹",
                                                                               ":/res/commonImgs/arrow-up.png");
    rightScrollAreaVLayout_showFolders_VLayout_topBtn->setFixedHeight(60);
    // 父控件是QWidget，后面好设置setVisible
    auto *rightScrollAreaVLayout_showFolders_VLayout_bottomHLayoutWidget = new QWidget;

    rightScrollAreaVLayout_showFolders_VLayoutWidget->setLayout(rightScrollAreaVLayout_showFolders_VLayout);

    // 为此按钮绑定事件,点击切换是否出现这一栏的内容
    connect(rightScrollAreaVLayout_showFolders_VLayout_topBtn, &myPushButton::clicked, [=]() {
        if (rightLayout_flag_showFolders) {
            rightScrollAreaVLayout_showFolders_VLayout_topBtn->setSelectedBtn(8, false);
            rightScrollAreaVLayout_showFolders_VLayout_bottomHLayoutWidget->setVisible(false);
            rightLayout_flag_showFolders = false;
        } else {
            rightScrollAreaVLayout_showFolders_VLayout_topBtn->setSelectedBtn(8, true);
            rightScrollAreaVLayout_showFolders_VLayout_bottomHLayoutWidget->setVisible(true);
            rightLayout_flag_showFolders = true;
        }
    });
    auto *rightScrollAreaVLayout_showFolders_VLayout_bottomHLayout = new QHBoxLayout;
    // 展示的文件夹一栏中的常用文件夹
    auto *desktopFolderBtn = new myPushButton(9, "桌面", ":/res/rightScrollArea_homePage_imgs/desktopFolder.png");
    auto *downloadFolderBtn = new myPushButton(9, "下载", ":/res/rightScrollArea_homePage_imgs/downloadFolder.png");
    auto *documentFolderBtn = new myPushButton(9, "文档", ":/res/rightScrollArea_homePage_imgs/documentFolder.png");
    auto *musicFolderBtn = new myPushButton(9, "音乐", ":/res/rightScrollArea_homePage_imgs/musicFolder.png");
    auto *pictureFolderBtn = new myPushButton(9, "图片", ":/res/rightScrollArea_homePage_imgs/pictureFolder.png");
    auto *moviesFolderBtn = new myPushButton(9, "视频", ":/res/rightScrollArea_homePage_imgs/videoFolder.png");


    // 设置固定高度
    desktopFolderBtn->setFixedHeight(110);
    downloadFolderBtn->setFixedHeight(110);
    documentFolderBtn->setFixedHeight(110);
    musicFolderBtn->setFixedHeight(110);
    pictureFolderBtn->setFixedHeight(110);
    moviesFolderBtn->setFixedHeight(110);
    // 添加到布局上
    rightScrollAreaVLayout_showFolders_VLayout_bottomHLayout->addWidget(desktopFolderBtn);
    rightScrollAreaVLayout_showFolders_VLayout_bottomHLayout->addWidget(downloadFolderBtn);
    rightScrollAreaVLayout_showFolders_VLayout_bottomHLayout->addWidget(documentFolderBtn);
    rightScrollAreaVLayout_showFolders_VLayout_bottomHLayout->addWidget(musicFolderBtn);
    rightScrollAreaVLayout_showFolders_VLayout_bottomHLayout->addWidget(pictureFolderBtn);
    rightScrollAreaVLayout_showFolders_VLayout_bottomHLayout->addWidget(moviesFolderBtn);

    // 添加到Widget上。
    rightScrollAreaVLayout_showFolders_VLayout_bottomHLayoutWidget->setLayout(
            rightScrollAreaVLayout_showFolders_VLayout_bottomHLayout);

    // 添加展示文件夹栏中的标题一栏
    rightScrollAreaVLayout_showFolders_VLayout->addWidget(rightScrollAreaVLayout_showFolders_VLayout_topBtn);
    rightScrollAreaVLayout_showFolders_VLayout->addWidget(
            rightScrollAreaVLayout_showFolders_VLayout_bottomHLayoutWidget);

    // 展示驱动器栏
    auto *rightScrollAreaVLayout_showDrivers_VLayoutWidget = new QWidget;
    auto *rightScrollAreaVLayout_showDrivers_VLayout = new QVBoxLayout;
    auto *rightScrollAreaVLayout_showDrivers_VLayout_topBtn = new myPushButton(8, "驱动器",
                                                                               ":/res/commonImgs/arrow-up.png");
    rightScrollAreaVLayout_showDrivers_VLayout_topBtn->setFixedHeight(60);
    auto *rightScrollAreaVLayout_showDrivers_VLayout_bottomHLayoutWidget = new QWidget;

    rightScrollAreaVLayout_showDrivers_VLayoutWidget->setLayout(rightScrollAreaVLayout_showDrivers_VLayout);


    // 为此按钮绑定事件,点击切换是否出现这一栏的内容
    connect(rightScrollAreaVLayout_showDrivers_VLayout_topBtn, &myPushButton::clicked, [=]() {
        if (rightLayout_flag_showDrivers) {
            rightScrollAreaVLayout_showDrivers_VLayout_topBtn->setSelectedBtn(8, false);
            rightScrollAreaVLayout_showDrivers_VLayout_bottomHLayoutWidget->setVisible(false);
            rightLayout_flag_showDrivers = false;
        } else {
            rightScrollAreaVLayout_showDrivers_VLayout_topBtn->setSelectedBtn(8, true);
            rightScrollAreaVLayout_showDrivers_VLayout_bottomHLayoutWidget->setVisible(true);
            rightLayout_flag_showDrivers = true;
        }
    });
    auto *rightScrollAreaVLayout_showDrivers_VLayout_bottomHLayout = new QHBoxLayout;
    // 展示驱动器栏的常用驱动器控件 TODO 与前面的驱动器信息有关联 在代码263行
    for (int i = 0; i < driverList.count(); ++i) {
        // 添加展示的驱动器图片
        if (driverList.at(i).isRoot()) {
            auto *showDrivers_systemDriver = new myPushButton(11,
                                                              QString("本地磁盘：(%1)").arg(driverList.at(i).displayName()),
                                                              ":/res/rightScrollArea_homePage_imgs/systemDriver.png",
                                                              driverList.at(i).bytesAvailable() / pow(2, 30),
                                                              driverList.at(i).bytesTotal() / pow(2, 30));
            showDrivers_systemDriver->setFixedSize(350, 110);
            rightScrollAreaVLayout_showDrivers_VLayout_bottomHLayout->addWidget(showDrivers_systemDriver);
            connect(showDrivers_systemDriver, &myPushButton::clicked, [=]() {
                leftScrollAreaSelectedBtn[7]->clicked(true);
                asideLineEdit_left->setText(driverList.at(i).displayName());
                fileTableView->setRootIndex(
                        fileModel->index(driverList.at(i).displayName()));
                rightStackedWidget->setCurrentIndex(1);
            });
        } else {
            auto *showDrivers_otherDriver = new myPushButton(11, QString("本地磁盘：(%1)").arg(driverList.at(i).rootPath()),
                                                             ":/res/rightScrollArea_homePage_imgs/otherDriver.png",
                                                             driverList.at(i).bytesAvailable() / pow(2, 30),
                                                             driverList.at(i).bytesTotal() / pow(2, 30));
            showDrivers_otherDriver->setFixedSize(350, 110);
            rightScrollAreaVLayout_showDrivers_VLayout_bottomHLayout->addWidget(showDrivers_otherDriver);
            connect(showDrivers_otherDriver, &myPushButton::clicked, [=]() {
                leftScrollAreaSelectedBtn[8]->clicked(true);
                asideLineEdit_left->setText(driverList.at(i).rootPath());
                fileTableView->setRootIndex(
                        fileModel->index(driverList.at(i).rootPath()));
                rightStackedWidget->setCurrentIndex(1);
            });
        }
    }
    rightScrollAreaVLayout_showDrivers_VLayout_bottomHLayout->addStretch();
    rightScrollAreaVLayout_showDrivers_VLayout_bottomHLayoutWidget->setLayout(
            rightScrollAreaVLayout_showDrivers_VLayout_bottomHLayout);

    // 添加布局
    rightScrollAreaVLayout_showDrivers_VLayout->addWidget(rightScrollAreaVLayout_showDrivers_VLayout_topBtn);
    rightScrollAreaVLayout_showDrivers_VLayout->addWidget(
            rightScrollAreaVLayout_showDrivers_VLayout_bottomHLayoutWidget);

    // 展示包部件一栏
    auto *rightScrollAreaVLayout_showPackages_VLayoutWidget = new QWidget;
    auto *rightScrollAreaVLayout_showPackages_VLayout = new QVBoxLayout;
    auto *rightScrollAreaVLayout_showPackages_VLayout_topBtn = new myPushButton(8, "包",
                                                                                ":/res/commonImgs/arrow-up.png");
    rightScrollAreaVLayout_showPackages_VLayout_topBtn->setFixedHeight(60);
    auto *rightScrollAreaVLayout_showPackages_VLayout_bottomHLayoutWidget = new QWidget;
    rightScrollAreaVLayout_showPackages_VLayoutWidget->setLayout(rightScrollAreaVLayout_showPackages_VLayout);

    // 为此按钮绑定事件,点击切换是否出现这一栏的内容
    connect(rightScrollAreaVLayout_showPackages_VLayout_topBtn, &myPushButton::clicked, [=]() {
        if (rightLayout_flag_showPackages) {
            rightScrollAreaVLayout_showPackages_VLayout_topBtn->setSelectedBtn(8, false);
            rightScrollAreaVLayout_showPackages_VLayout_bottomHLayoutWidget->setVisible(false);
            rightLayout_flag_showPackages = false;
        } else {
            rightScrollAreaVLayout_showPackages_VLayout_topBtn->setSelectedBtn(8, true);
            rightScrollAreaVLayout_showPackages_VLayout_bottomHLayoutWidget->setVisible(true);
            rightLayout_flag_showPackages = true;
        }
    });
    auto *rightScrollAreaVLayout_showPackages_VLayout_bottomHLayout = new QHBoxLayout;

    // 创建包的按钮
    packageWrapper = new QWidget;
    packageWrapper->setFixedSize(350, 320);
    packageWrapper->setStyleSheet("QWidget{background-color:rgb(50, 50, 50);border-radius:10px;}");
    createPackageBtn = new myPushButton(12, "", ":/res/rightScrollArea_homePage_imgs/addPackage.png",
                                        0, 0, packageWrapper);
    createPackageBtn->move(145, 130);
    rightScrollAreaVLayout_showPackages_VLayout_bottomHLayout->addWidget(packageWrapper);

    // 添加布局
    rightScrollAreaVLayout_showPackages_VLayout_bottomHLayoutWidget->setLayout(
            rightScrollAreaVLayout_showPackages_VLayout_bottomHLayout);
    rightScrollAreaVLayout_showPackages_VLayout->addWidget(rightScrollAreaVLayout_showPackages_VLayout_topBtn);
    rightScrollAreaVLayout_showPackages_VLayout->addWidget(
            rightScrollAreaVLayout_showPackages_VLayout_bottomHLayoutWidget);
    rightScrollAreaVLayout_showPackages_VLayout_bottomHLayout->addStretch();

    // 创建包部件完成后的布局及组件
    packageWrapperMainVLayout = new QVBoxLayout;
    packageWrapperTitleWidget = new QWidget;
    packageWrapperTitleWidgetHLayout = new QHBoxLayout;
    packageTitleLabel = new QLabel;
    packageWrapperTitleLabel_setting = new myPushButton(6, "", ":/res/fixedToolWidget_imgs/more.png");
    packageWrapperTitleLabel_menu = new QMenu;
    packageWrapperTitleLabel_menu_rename = new QAction(QPixmap(":/res/commonImgs/packageRename.png"), "重命名包");
    packageWrapperTitleLabel_menu_addFiles = new QAction(QPixmap(":/res/commonImgs/packageAddItems.png"), "添加文件");
    packageWrapperTitleLabel_menu_addFolders = new QAction(QPixmap(":/res/commonImgs/packageAddItems.png"), "添加文件夹");
    packageWrapperTitleLabel_menu_removePackage = new QAction(QPixmap(":/res/commonImgs/removePackage.png"), "移除包");

    // 存放文件或文件夹的一栏
    packageWrapperStoredWidget = new QWidget;
    packageWrapperStoredWidgetGridLayout = new QGridLayout;
    packageWrapperStoredWidget->setLayout(packageWrapperStoredWidgetGridLayout);


    // 设置菜单样式
    packageWrapperTitleLabel_menu->setFont(QFont("kaiTi", 10));
    packageWrapperTitleLabel_menu->setFont(QFont("kaiTi", 10));
    packageWrapperTitleLabel_menu->setStyleSheet(
            "QMenu{background-color:rgb(60,60,60);color:white;border-radius:3px;padding-top:4px;}"
            "QMenu::item{border-radius:3px;height:38px;min-width:1px;margin:5px 10px;}"
            "QMenu::item:selected{background-color:rgb(109, 169, 216);color:black;}");

    packageWrapperTitleLabel_menu->addAction(packageWrapperTitleLabel_menu_rename);
    packageWrapperTitleLabel_menu->addAction(packageWrapperTitleLabel_menu_addFiles);
    packageWrapperTitleLabel_menu->addAction(packageWrapperTitleLabel_menu_addFolders);
    packageWrapperTitleLabel_menu->addAction(packageWrapperTitleLabel_menu_removePackage);
    packageWrapperTitleLabel_setting->setMenu(packageWrapperTitleLabel_menu);

    packageWrapperTitleLabel_setting->setFixedSize(40, 40);
    packageTitleLabel->setFont(QFont("kaiTi", 12));

    packageWrapperTitleWidgetHLayout->setMargin(0);
    packageWrapperTitleWidgetHLayout->addWidget(packageTitleLabel);
    packageWrapperTitleWidgetHLayout->addStretch();
    packageWrapperTitleWidgetHLayout->addWidget(packageWrapperTitleLabel_setting);

    packageWrapperTitleWidget->setFixedHeight(40);
    packageWrapperTitleWidget->setLayout(packageWrapperTitleWidgetHLayout);

    // 向主布局添加组件
    packageWrapperMainVLayout->addWidget(packageWrapperTitleWidget);
    packageWrapperMainVLayout->addWidget(packageWrapperStoredWidget);
    packageWrapperMainVLayout->addStretch();

    packageWrapper->setLayout(packageWrapperMainVLayout);


    // 加载文件进行判断，有包名内容，直接加载，否则移除所有的布局里面的控件
    loadPackageFiles(); // 加载json文件
    if (packageName.isEmpty()) {
        createPackageBtn->setVisible(true);
        // 删除布局中的组件(直接设为不可见)
        packageTitleLabel->setVisible(false);
        packageWrapperTitleLabel_setting->setVisible(false);
    } else {
        packageTitleLabel->setVisible(true);
        packageTitleLabel->setText(packageName);
        packageWrapperTitleLabel_setting->setVisible(true);
        for (int i = 0; i < addPackageFileInfoList.count(); i++) {
            QFileInfo tempInfo(addPackageFileInfoList.at(i));
            if (tempInfo.isDir()) {
                packageWrapperStoredWidgetGridLayoutItems[i] = new myPushButton(14, addPackageFileInfoList.at(i));
                packageWrapperStoredWidgetGridLayout->addWidget(
                        packageWrapperStoredWidgetGridLayoutItems[i],
                        packageWrapperStoredWidgetGridLayout_row++ / 2, packageWrapperStoredWidgetGridLayout_col++ % 2);
                connect(packageWrapperStoredWidgetGridLayoutItems[i], &myPushButton::clicked, [=]() {
                    absolutePath = addPackageFileInfoList.at(i);
                    if (absolutePathIterator.count() == 0) {
                        absolutePathIterator.append(absolutePath);
                        absolutePathIterator_index++;
                    } else {
                        if (absolutePathIterator.last() != absolutePath) {
                            absolutePathIterator.append(absolutePath);
                            absolutePathIterator_index++;
                        }
                    }
                    if (absolutePathIterator_index == 0) {
                        asideBtn_right->setEnabled(true);
                        asideBtn_left->setEnabled(false);
                        asideBtn_right->setIcon(QIcon(":/res/mainScene_imgs/direction_right_white.png"));
                        asideBtn_left->setIcon(QIcon(":/res/mainScene_imgs/direction_left_gray.png"));
                    } else if (absolutePathIterator_index == absolutePathIterator.count() - 1) {
                        asideBtn_right->setEnabled(false);
                        asideBtn_left->setEnabled(true);
                        asideBtn_right->setIcon(QIcon(":/res/mainScene_imgs/direction_right_gray.png"));
                        asideBtn_left->setIcon(QIcon(":/res/mainScene_imgs/direction_left_white.png"));
                    } else {
                        asideBtn_right->setEnabled(true);
                        asideBtn_left->setEnabled(true);
                        asideBtn_left->setIcon(QIcon(":/res/mainScene_imgs/direction_left_white.png"));
                        asideBtn_right->setIcon(QIcon(":/res/mainScene_imgs/direction_right_white.png"));
                    }
                    asideLineEdit_left->setText(absolutePath);
                    fileTableView->setRootIndex(fileModel->index(absolutePath));
                    rightStackedWidget->setCurrentIndex(1);
                });
                connect(packageWrapperStoredWidgetGridLayoutItems[i], &myPushButton::openProjetLocation, [=]() {
                    packageWrapperStoredWidgetGridLayoutItems[i]->clicked(true);
                });
                connect(packageWrapperStoredWidgetGridLayoutItems[i], &myPushButton::removeProject, [=]() {
                    addPackageFileInfoList.removeAt(i);
                    packageWrapperStoredWidgetGridLayout->removeWidget(packageWrapperStoredWidgetGridLayoutItems[i]);
                    delete packageWrapperStoredWidgetGridLayoutItems[i];
                    writeIntoPackageJsonFiles("", false, i);
                    packageWrapperStoredWidgetGridLayout_row--;
                    packageWrapperStoredWidgetGridLayout_col--;
                });
            }
            if (tempInfo.isFile()) {
                packageWrapperStoredWidgetGridLayoutItems[i] = new myPushButton(14, addPackageFileInfoList.at(i));
                packageWrapperStoredWidgetGridLayout->addWidget(
                        packageWrapperStoredWidgetGridLayoutItems[i],
                        packageWrapperStoredWidgetGridLayout_row++ / 2, packageWrapperStoredWidgetGridLayout_col++ % 2);
                connect(packageWrapperStoredWidgetGridLayoutItems[i], &myPushButton::clicked, [=]() {
                    QDesktopServices::openUrl(QUrl::fromLocalFile(tempInfo.canonicalFilePath()));
                });
                connect(packageWrapperStoredWidgetGridLayoutItems[i], &myPushButton::openProjetLocation, [=]() {
                    packageWrapperStoredWidgetGridLayoutItems[i]->clicked(true);
                });
                connect(packageWrapperStoredWidgetGridLayoutItems[i], &myPushButton::removeProject, [=]() {
                    addPackageFileInfoList.removeAt(i);
                    packageWrapperStoredWidgetGridLayout->removeWidget(packageWrapperStoredWidgetGridLayoutItems[i]);
                    delete packageWrapperStoredWidgetGridLayoutItems[i];
                    writeIntoPackageJsonFiles("", false, i);
                    packageWrapperStoredWidgetGridLayout_row--;
                    packageWrapperStoredWidgetGridLayout_col--;
                });
            }
        }
        createPackageBtn->setVisible(false);
    }

    // 绑定添加包的按钮事件
    connect(createPackageBtn, &myPushButton::clicked, [=]() {
        createPackageBtnDialog = new myCreatePackageDialog;
        createPackageBtnDialog->setAttribute(Qt::WA_DeleteOnClose);
        createPackageBtnDialog->show();
        connect(createPackageBtnDialog, SIGNAL(sendData(QString, int)), this, SLOT(
                receiveData_handleCreatePackage(QString, int)));
    });

    // 重命名包事件
    connect(packageWrapperTitleLabel_menu_rename, &QAction::triggered, [=]() {
        auto *renamePackageDialog = new myCreatePackageDialog;
        renamePackageDialog->setAttribute(Qt::WA_DeleteOnClose);
        renamePackageDialog->show();
        connect(renamePackageDialog, SIGNAL(sendData(QString, int)), this, SLOT(
                receiveData_handleRenamePackage(QString, int)));
    });
    // 添加文件事件
    connect(packageWrapperTitleLabel_menu_addFiles, &QAction::triggered, [=]() {
        if (addPackageFileInfoList.count() == 10) {
            QMessageBox msg;
            msg.setWindowTitle("提示信息");
            msg.setWindowIcon(QPixmap(":/res/commonImgs/warning.png"));
            msg.setStyleSheet("QLabel{min-width:400px;min-height:100px;text-align:center;}");
            msg.setText("添加的项已达到上限！");
            msg.setFont(QFont("kaiTi", 14));
            msg.exec();
        } else {
            QString path = QFileDialog::getOpenFileName(this, "选择文件", "D:\\Windows_lib\\Desktop");
            bool flag_isRepeat = false;
            for (int i = 0; i < addPackageFileInfoList.count(); i++) {
                if (addPackageFileInfoList.at(i) == path) {
                    flag_isRepeat = true;
                }
            }
            if (!flag_isRepeat) {
                int index = addPackageFileInfoList.count();
                packageWrapperStoredWidgetGridLayoutItems[index] = new myPushButton(14, path);
                connect(packageWrapperStoredWidgetGridLayoutItems[index], &myPushButton::clicked, [=]() {
                    QDesktopServices::openUrl(QUrl::fromLocalFile(path));
                });
                connect(packageWrapperStoredWidgetGridLayoutItems[index], &myPushButton::openProjetLocation, [=]() {
                    packageWrapperStoredWidgetGridLayoutItems[index]->clicked(true);
                });
                connect(packageWrapperStoredWidgetGridLayoutItems[index], &myPushButton::removeProject, [=]() {
                    addPackageFileInfoList.removeAt(index);
                    packageWrapperStoredWidgetGridLayout->removeWidget(
                            packageWrapperStoredWidgetGridLayoutItems[index]);
                    delete packageWrapperStoredWidgetGridLayoutItems[index];
                    writeIntoPackageJsonFiles("", false, index);
                    packageWrapperStoredWidgetGridLayout_row--;
                    packageWrapperStoredWidgetGridLayout_col--;
                });
                // 添加到布局之上
                packageWrapperStoredWidgetGridLayout->addWidget(
                        packageWrapperStoredWidgetGridLayoutItems[index],
                        packageWrapperStoredWidgetGridLayout_row++ / 2, packageWrapperStoredWidgetGridLayout_col++ % 2);
                addPackageFileInfoList.append(path);
                writeIntoPackageJsonFiles(path);
            }
        }
    });
    // 添加文件夹事件
    connect(packageWrapperTitleLabel_menu_addFolders, &QAction::triggered, [=]() {
        if (addPackageFileInfoList.count() == 10) {
            // 已达上限
            QMessageBox msg;
            msg.setWindowTitle("提示信息");
            msg.setWindowIcon(QPixmap(":/res/commonImgs/warning.png"));
            msg.setStyleSheet("QLabel{min-width:400px;min-height:100px;text-align:center;}");
            msg.setText("添加的项已达到上限！");
            msg.setFont(QFont("kaiTi", 14));
            msg.exec();
        } else {
            QString dirPath = QFileDialog::getExistingDirectory(this, "选择文件夹", "D:\\Windows_lib\\Desktop",
                                                                QFileDialog::ShowDirsOnly);
            bool flag_isRepeat = false;
            for (int i = 0; i < addPackageFileInfoList.count(); i++) {
                if (addPackageFileInfoList.at(i) == dirPath) {
                    flag_isRepeat = true;
                }
            }
            if (!flag_isRepeat) {
                int index = addPackageFileInfoList.count();
                packageWrapperStoredWidgetGridLayoutItems[index] = new myPushButton(14, dirPath);
                connect(packageWrapperStoredWidgetGridLayoutItems[index], &myPushButton::clicked, [=]() {
                    absolutePath = addPackageFileInfoList.at(index);
                    if (absolutePathIterator.count() == 0) {
                        absolutePathIterator.append(absolutePath);
                        absolutePathIterator_index++;
                    } else {
                        if (absolutePathIterator.last() != absolutePath) {
                            absolutePathIterator.append(absolutePath);
                            absolutePathIterator_index++;
                        }
                    }
                    if (absolutePathIterator_index == 0) {
                        asideBtn_right->setEnabled(true);
                        asideBtn_left->setEnabled(false);
                        asideBtn_right->setIcon(QIcon(":/res/mainScene_imgs/direction_right_white.png"));
                        asideBtn_left->setIcon(QIcon(":/res/mainScene_imgs/direction_left_gray.png"));
                    } else if (absolutePathIterator_index == absolutePathIterator.count() - 1) {
                        asideBtn_right->setEnabled(false);
                        asideBtn_left->setEnabled(true);
                        asideBtn_right->setIcon(QIcon(":/res/mainScene_imgs/direction_right_gray.png"));
                        asideBtn_left->setIcon(QIcon(":/res/mainScene_imgs/direction_left_white.png"));
                    } else {
                        asideBtn_right->setEnabled(true);
                        asideBtn_left->setEnabled(true);
                        asideBtn_left->setIcon(QIcon(":/res/mainScene_imgs/direction_left_white.png"));
                        asideBtn_right->setIcon(QIcon(":/res/mainScene_imgs/direction_right_white.png"));
                    }
                    asideLineEdit_left->setText(absolutePath);
                    fileTableView->setRootIndex(fileModel->index(absolutePath));
                    rightStackedWidget->setCurrentIndex(1);
                });
                connect(packageWrapperStoredWidgetGridLayoutItems[index], &myPushButton::openProjetLocation, [=]() {
                    packageWrapperStoredWidgetGridLayoutItems[addPackageFileInfoList.count()]->clicked(true);
                });
                connect(packageWrapperStoredWidgetGridLayoutItems[index], &myPushButton::removeProject, [=]() {
                    addPackageFileInfoList.removeAt(index);
                    packageWrapperStoredWidgetGridLayout->removeWidget(
                            packageWrapperStoredWidgetGridLayoutItems[index]);
                    delete packageWrapperStoredWidgetGridLayoutItems[index];
                    writeIntoPackageJsonFiles("", false, index);
                    packageWrapperStoredWidgetGridLayout_row--;
                    packageWrapperStoredWidgetGridLayout_col--;
                });
                // 添加到布局之上
                packageWrapperStoredWidgetGridLayout->addWidget(
                        packageWrapperStoredWidgetGridLayoutItems[index],
                        packageWrapperStoredWidgetGridLayout_row++ / 2, packageWrapperStoredWidgetGridLayout_col++ % 2);
                addPackageFileInfoList.append(dirPath);
                writeIntoPackageJsonFiles(dirPath);
            }
        }
    });
    // 移除包事件
    connect(packageWrapperTitleLabel_menu_removePackage, &QAction::triggered, [=]() {
        auto *msg = new QMessageBox;
        msg->setWindowTitle("提示信息");
        msg->setWindowIcon(QPixmap(":/res/ico/fileLogo.png"));
        msg->setText("确定要移除此包吗？移除后的操作不可逆转！");
        msg->setIconPixmap(QPixmap(":/res/commonImgs/warning.png"));
        msg->setStyleSheet("QLabel{min-height:50px;}");
        msg->setAttribute(Qt::WA_DeleteOnClose);
        msg->setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        msg->setDefaultButton(QMessageBox::Cancel);
        int ret = msg->exec();
        if (ret == QMessageBox::Ok) {
            removePackage();
            createPackageBtn->setVisible(true);
        }
    });

    // 展示最近使用的文件
    auto *rightScrollAreaVLayout_showRecentUsing_VLayoutWidget = new QWidget;
    auto *rightScrollAreaVLayout_showRecentUsing_VLayout = new QVBoxLayout;
    auto *showRecentUsing_titleBtn = new myPushButton(8, "最近使用的文件", ":/res/commonImgs/arrow-up.png");
    showRecentUsing_titleBtn->setFixedHeight(60);
    defaultShowLabel = new QLabel;
    auto *showRecentUsingItemsVLayoutWidget = new QWidget;
    showRecentUsingItemsVLayout = new QVBoxLayout;
    rightScrollAreaVLayout_showRecentUsing_VLayoutWidget->setLayout(rightScrollAreaVLayout_showRecentUsing_VLayout);
    showRecentUsingItemsVLayout->addWidget(defaultShowLabel);

    // 为此按钮绑定事件,点击切换是否出现这一栏的内容
    connect(showRecentUsing_titleBtn, &myPushButton::clicked, [=]() {
        if (rightLayout_flag_showRecentUsing) {
            showRecentUsing_titleBtn->setSelectedBtn(8, false);
            showRecentUsingItemsVLayoutWidget->setVisible(false);
            rightLayout_flag_showRecentUsing = false;
        } else {
            showRecentUsing_titleBtn->setSelectedBtn(8, true);
            showRecentUsingItemsVLayoutWidget->setVisible(true);
            rightLayout_flag_showRecentUsing = true;
        }
    });

    // 展示最近使用的文件---添加组件和布局
    loadRecentUsingFiles();
    if (recentUsingFiles.count() == 0) {
        defaultShowLabel->setVisible(true);
    } else {
        defaultShowLabel->setVisible(false);;
        for (auto i: recentUsingFiles) {
            QFileInfo info = QFileInfo(i);
            auto *pItem = new myPushButton(13, i);
            pItem->setFixedHeight(42);
            connect(pItem, &myPushButton::clicked, [=]() {
                QDesktopServices::openUrl(QUrl::fromLocalFile(info.filePath()));
            });
            showRecentUsingItemsVLayout->addWidget(pItem);
        }
    }
    showRecentUsingItemsVLayoutWidget->setLayout(showRecentUsingItemsVLayout);
    defaultShowLabel->setFixedHeight(60);
    defaultShowLabel->setText("以前访问过的文件和文件夹将显示在此处");
    defaultShowLabel->setFont(QFont("kaiTi", 13, -1, true));

    // 为管理小部件菜单按钮绑定事件
    connect(manageItems_menu_toggleFolders, &QAction::triggered, [=] {
        if (manageItems_menu_toggleFolders_flag) {
            rightScrollAreaVLayout_showFolders_VLayoutWidget->setVisible(false);
            manageItems_menu_toggleFolders->setText("显示文件夹小部件");
            manageItems_menu_toggleFolders_flag = false;
        } else {
            rightScrollAreaVLayout_showFolders_VLayoutWidget->setVisible(true);
            manageItems_menu_toggleFolders->setText("隐藏文件夹小部件");
            manageItems_menu_toggleFolders_flag = true;
        }

    });
    connect(manageItems_menu_toggleDrivers, &QAction::triggered, [=] {
        if (manageItems_menu_toggleDrivers_flag) {
            rightScrollAreaVLayout_showDrivers_VLayoutWidget->setVisible(false);
            manageItems_menu_toggleDrivers->setText("显示驱动器小部件");
            manageItems_menu_toggleDrivers_flag = false;
        } else {
            rightScrollAreaVLayout_showDrivers_VLayoutWidget->setVisible(true);
            manageItems_menu_toggleDrivers->setText("隐藏驱动器小部件");
            manageItems_menu_toggleDrivers_flag = true;
        }
    });
    connect(manageItems_menu_togglePackages, &QAction::triggered, [=] {
        if (manageItems_menu_togglePackages_flag) {
            rightScrollAreaVLayout_showPackages_VLayoutWidget->setVisible(false);
            manageItems_menu_togglePackages->setText("显示包小部件");
            manageItems_menu_togglePackages_flag = false;
        } else {
            rightScrollAreaVLayout_showPackages_VLayoutWidget->setVisible(true);
            manageItems_menu_togglePackages->setText("隐藏包小部件");
            manageItems_menu_togglePackages_flag = true;
        }
    });
    connect(manageItems_menu_toggleRecentUsing, &QAction::triggered, [=] {
        if (manageItems_menu_toggleRecentUsing_flag) {
            rightScrollAreaVLayout_showRecentUsing_VLayoutWidget->setVisible(false);
            manageItems_menu_toggleRecentUsing->setText("显示最近使用小部件");
            manageItems_menu_toggleRecentUsing_flag = false;
        } else {
            rightScrollAreaVLayout_showRecentUsing_VLayoutWidget->setVisible(true);
            manageItems_menu_toggleRecentUsing->setText("隐藏最近使用小部件");
            manageItems_menu_toggleRecentUsing_flag = true;
        }
    });

    // 添加控件
    rightScrollAreaVLayout_showRecentUsing_VLayout->addWidget(showRecentUsing_titleBtn);
    rightScrollAreaVLayout_showRecentUsing_VLayout->addWidget(showRecentUsingItemsVLayoutWidget);
    // 设置对齐方式
    showRecentUsingItemsVLayout->setAlignment(defaultShowLabel, Qt::AlignHCenter);

    rightScrollAreaVLayout->setMargin(0);
    // 为堆栈窗口添加四个已知的布局
    rightScrollAreaVLayout->addWidget(rightScrollAreaVLayout_showFolders_VLayoutWidget);
    rightScrollAreaVLayout->addWidget(rightScrollAreaVLayout_showDrivers_VLayoutWidget);
    rightScrollAreaVLayout->addWidget(rightScrollAreaVLayout_showPackages_VLayoutWidget);
    rightScrollAreaVLayout->addWidget(rightScrollAreaVLayout_showRecentUsing_VLayoutWidget);
    rightScrollAreaVLayout->addStretch();

    rightMask->setLayout(rightScrollAreaVLayout);
    rightScrollArea->setWidgetResizable(true);
    rightScrollArea->setWidget(rightMask);

    // 添加组件和布局
    fixedToolWidgetHLayoutInnerWidgetHLayout->setMargin(0);
    fixedToolWidgetHLayoutInnerWidgetHLayout->addLayout(fixedToolWidgetHLayoutInnerWidgetHLayout_left);
    // 默认先显示主页内容的布局
    fixedToolWidgetHLayoutInnerWidgetHLayout->addLayout(fixedToolWidgetHLayoutInnerWidgetHLayout_right_homePage);

    fixedToolWidgetHLayout->addSpacing(5);
    fixedToolWidgetHLayout->addWidget(newBuild);
    fixedToolWidgetHLayout->addWidget(fixedToolWidgetHLayoutInnerWidget);

    // 设置样式和布局
    fixedToolWidgetHLayoutInnerWidget->setLayout(fixedToolWidgetHLayoutInnerWidgetHLayout);
    fixedToolWidgetHLayout->setMargin(0);
    fixedToolWidget->setFixedHeight(60);
    fixedToolWidget->setLayout(fixedToolWidgetHLayout);
    auto *fixedToolWidget_Shadow = new QGraphicsDropShadowEffect;
    fixedToolWidget_Shadow->setBlurRadius(8);
    fixedToolWidget_Shadow->setColor(QColor(154, 154, 154));
    fixedToolWidget_Shadow->setOffset(1, 1);
    fixedToolWidget->setGraphicsEffect(fixedToolWidget_Shadow);

    rightScrollArea->setFrameShape(QFrame::NoFrame);
    rightMask->setStyleSheet("background-color:rgb(40,40,40)");

    // rightWidget中表示底部信息情况
    auto *rightWidgetVLayout_bottom_statusWidget = new QWidget;
    auto *rightWidgetVLayout_bottom_statusWidget_HLayout = new QHBoxLayout;
    rightWidgetVLayout_bottom_statusWidget->setLayout(rightWidgetVLayout_bottom_statusWidget_HLayout);
    statusMsg_first = new QLabel;
    statusMsg_first->setText("");
    statusMsg_second = new QLabel;
    statusMsg_second->setText("");
    rightWidgetVLayout_bottom_statusWidget_HLayout->addWidget(statusMsg_first);
    rightWidgetVLayout_bottom_statusWidget_HLayout->addWidget(statusMsg_second);
    rightWidgetVLayout_bottom_statusWidget_HLayout->addStretch();
    rightWidgetVLayout_bottom_statusWidget_HLayout->setMargin(0);
    rightWidgetVLayout_bottom_statusWidget->setFixedHeight(40);

    rightWidgetVLayout->addWidget(fixedToolWidget);
    rightWidgetVLayout->addWidget(rightStackedWidget);
    rightWidgetVLayout->addWidget(rightWidgetVLayout_bottom_statusWidget);

    rightWidget->setLayout(rightWidgetVLayout);

    // 设置背景色（给父组件设置背景色即可）
    rightWidget->setStyleSheet("background-color:rgb(40,40,40);");


    // 文件系统功能开始
    fileModel = new QFileSystemModel(this);
    fileModel->setFilter(QDir::Files | QDir::Dirs | QDir::NoDot);
    fileModel->setRootPath("/");
    fileModel->setReadOnly(false);
    // 处理显示文件系统的快捷方式
    fileModel->setResolveSymlinks(false);
    auto *fileTableViewWrapper = new QWidget;
    auto *fileTableViewWrapper_VLayout = new QVBoxLayout;
    auto *fileTableViewWrapper_VLayout_topWidget = new QWidget;
    auto *fileTableViewWrapper_VLayout_topWidgetHLayout = new QHBoxLayout;

    auto *fileTableView_name = new QLabel;
    auto *fileTableView_size = new QLabel;
    auto *fileTableView_type = new QLabel;
    auto *fileTableView_lastModify = new QLabel;
    // 设置Text
    fileTableView_name->setText("名称");
    fileTableView_size->setText("大小");
    fileTableView_type->setText("类型");
    fileTableView_lastModify->setText("修改时间");
    // 设置字体
    fileTableView_name->setFont(QFont("kaiTi", 12));
    fileTableView_size->setFont(QFont("kaiTi", 12));
    fileTableView_type->setFont(QFont("kaiTi", 12));
    fileTableView_lastModify->setFont(QFont("kaiTi", 12));
    // 设置固定高度
    fileTableView_name->setFixedHeight(42);
    fileTableView_size->setFixedHeight(42);
    fileTableView_type->setFixedHeight(42);
    fileTableView_lastModify->setFixedHeight(42);
    // 将组件添加到布局上
    fileTableViewWrapper_VLayout_topWidgetHLayout->addWidget(fileTableView_name);
    fileTableViewWrapper_VLayout_topWidgetHLayout->addWidget(fileTableView_size);
    fileTableViewWrapper_VLayout_topWidgetHLayout->addWidget(fileTableView_type);
    fileTableViewWrapper_VLayout_topWidgetHLayout->addWidget(fileTableView_lastModify);

    fileTableViewWrapper_VLayout_topWidget->setLayout(fileTableViewWrapper_VLayout_topWidgetHLayout);

    fileTableView = new QTableView;

    fileTableViewWrapper_VLayout->addWidget(fileTableViewWrapper_VLayout_topWidget);
    fileTableViewWrapper_VLayout->addWidget(fileTableView);
    fileTableViewWrapper->setLayout(fileTableViewWrapper_VLayout);

    fileTableView->setModel(fileModel);
    // 设置水平和垂直的表头消失
    fileTableView->verticalHeader()->setVisible(false);
    fileTableView->horizontalHeader()->setVisible(false);

    // 设置自定义
    // 的限制
    fileTableView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    fileTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // 选中文件或文件夹时触发该事件（主要使一些按钮可以点击）
    connect(fileTableView->selectionModel(), SIGNAL(selectionChanged(
                                                            const QItemSelection &, const QItemSelection &)),
            this, SLOT(handleSelectionChanged()));
    // tableView单击后，直接启用相应的功能按钮
    connect(fileTableView, &QTableView::clicked, [=]() {
        if (currentSelectedList.count()) {
            fixedToolWidget_setFunctionalBtnEnable();
        }
    });
    // 为fileTableView绑定双击事件（双击后进入下一级文件夹）
    connect(fileTableView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(onTableViewDoubleClicked()));

    // 设置行高和列宽
    fileTableView->verticalHeader()->setDefaultSectionSize(45);
    fileTableView->setFrameShape(QFrame::NoFrame);

    fileTableView->setColumnWidth(0, 500);
    fileTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    fileTableView->setFont(QFont("kaiTi", 10));


    // 设置滚动条出现的策略
    leftScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    rightScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    fileTableView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);


    // 设置样式
    leftScrollArea->setStyleSheet("background-color:rgb(40,40,40)");
    leftScrollArea->setFrameShape(QFrame::NoFrame);
    leftMask->setStyleSheet("background-color:rgb(40,40,40)");
    auto *rightStackedWidget_Shadow = new QGraphicsDropShadowEffect;
    rightStackedWidget_Shadow->setBlurRadius(8);
    rightStackedWidget_Shadow->setColor(QColor(154, 154, 154));
    rightStackedWidget_Shadow->setOffset(0, 1);
    rightStackedWidget->setGraphicsEffect(rightStackedWidget_Shadow);

    // 完成对界面上的按钮的绑定事件
    // 默认显示主页的内容
    // 默认显示主页内容
    leftScrollAreaSelectedBtn[1]->clicked(true);
    // 点击切换到主页的按钮并将
    // 首先默认是主页设置禁用按钮
    homePageAllBtn_setEnable();
    absolutePathIterator.append("主页：");
    connect(leftScrollAreaSelectedBtn[1], &myPushButton::clicked, [=]() {
        // 设置一些禁用掉的按钮
        homePageAllBtn_setEnable();
        asideBtn_top->setEnabled(false);
        asideBtn_top->setIcon(QIcon(":/res/mainScene_imgs/direction_top_gray.png"));
        asideLineEdit_left->setText("主页：");
        statusMsg_first->setText("");
        if (absolutePathIterator.count() == 0) {
            absolutePathIterator.append("主页：");
            absolutePathIterator_index++;
        } else {
            if (absolutePathIterator.last() != "主页：") {
                absolutePathIterator.append("主页：");
                absolutePathIterator_index++;
            }
        }
        if (absolutePathIterator_index == 0) {
            asideBtn_right->setEnabled(true);
            asideBtn_left->setEnabled(false);
            asideBtn_right->setIcon(QIcon(":/res/mainScene_imgs/direction_right_white.png"));
            asideBtn_left->setIcon(QIcon(":/res/mainScene_imgs/direction_left_gray.png"));
        } else if (absolutePathIterator_index == absolutePathIterator.count() - 1) {
            asideBtn_right->setEnabled(false);
            asideBtn_left->setEnabled(true);
            asideBtn_right->setIcon(QIcon(":/res/mainScene_imgs/direction_right_gray.png"));
            asideBtn_left->setIcon(QIcon(":/res/mainScene_imgs/direction_left_white.png"));
        } else {
            asideBtn_right->setEnabled(true);
            asideBtn_left->setEnabled(true);
            asideBtn_left->setIcon(QIcon(":/res/mainScene_imgs/direction_left_white.png"));
            asideBtn_right->setIcon(QIcon(":/res/mainScene_imgs/direction_right_white.png"));
        }
        rightStackedWidget->setCurrentIndex(0);
    });
    // 点击切换到桌面的按钮
    connect(leftScrollAreaSelectedBtn[2], &myPushButton::clicked, [=]() {
        // 设置禁用的按钮
        newBuild->setEnabled(true);
        asideBtn_top->setEnabled(true);
        asideBtn_top->setIcon(QIcon(":/res/mainScene_imgs/direction_top_white.png"));
        if (copyCurrentSelectedList.count()) {
            paste->setEnabled(true);
        } else {
            paste->setEnabled(false);
        }
        absolutePath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
        if (absolutePathIterator.count() == 0) {
            absolutePathIterator.append(absolutePath);
            absolutePathIterator_index++;
        } else {
            if (absolutePathIterator.last() != absolutePath) {
                absolutePathIterator.append(absolutePath);
                absolutePathIterator_index++;
            }
        }
        if (absolutePathIterator_index == 0) {
            asideBtn_right->setEnabled(true);
            asideBtn_left->setEnabled(false);
            asideBtn_right->setIcon(QIcon(":/res/mainScene_imgs/direction_right_white.png"));
            asideBtn_left->setIcon(QIcon(":/res/mainScene_imgs/direction_left_gray.png"));
        } else if (absolutePathIterator_index == absolutePathIterator.count() - 1) {
            asideBtn_right->setEnabled(false);
            asideBtn_left->setEnabled(true);
            asideBtn_right->setIcon(QIcon(":/res/mainScene_imgs/direction_right_gray.png"));
            asideBtn_left->setIcon(QIcon(":/res/mainScene_imgs/direction_left_white.png"));
        } else {
            asideBtn_right->setEnabled(true);
            asideBtn_left->setEnabled(true);
            asideBtn_left->setIcon(QIcon(":/res/mainScene_imgs/direction_left_white.png"));
            asideBtn_right->setIcon(QIcon(":/res/mainScene_imgs/direction_right_white.png"));
        }
        FilesAndDirs_count();
        asideLineEdit_left->setText(absolutePath);
        fileTableView->setRootIndex(fileModel->index(absolutePath));
        rightStackedWidget->setCurrentIndex(1);
    });
    // 点击切换到下载的按钮
    connect(leftScrollAreaSelectedBtn[3], &myPushButton::clicked, [=] {
        // 设置禁用按钮
        newBuild->setEnabled(true);
        asideBtn_top->setEnabled(true);
        asideBtn_top->setIcon(QIcon(":/res/mainScene_imgs/direction_top_white.png"));
        if (copyCurrentSelectedList.count()) {
            paste->setEnabled(true);
        } else {
            paste->setEnabled(false);
        }
        absolutePath = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
        if (absolutePathIterator.count() == 0) {
            absolutePathIterator.append(absolutePath);
            absolutePathIterator_index++;
        } else {
            if (absolutePathIterator.last() != absolutePath) {
                absolutePathIterator.append(absolutePath);
                absolutePathIterator_index++;
            }
        }
        if (absolutePathIterator_index == 0) {
            asideBtn_right->setEnabled(true);
            asideBtn_left->setEnabled(false);
            asideBtn_right->setIcon(QIcon(":/res/mainScene_imgs/direction_right_white.png"));
            asideBtn_left->setIcon(QIcon(":/res/mainScene_imgs/direction_left_gray.png"));
        } else if (absolutePathIterator_index == absolutePathIterator.count() - 1) {
            asideBtn_right->setEnabled(false);
            asideBtn_left->setEnabled(true);
            asideBtn_right->setIcon(QIcon(":/res/mainScene_imgs/direction_right_gray.png"));
            asideBtn_left->setIcon(QIcon(":/res/mainScene_imgs/direction_left_white.png"));
        } else {
            asideBtn_right->setEnabled(true);
            asideBtn_left->setEnabled(true);
            asideBtn_right->setIcon(QIcon(":/res/mainScene_imgs/direction_right_white.png"));
            asideBtn_left->setIcon(QIcon(":/res/mainScene_imgs/direction_left_white.png"));
        }
        FilesAndDirs_count();
        asideLineEdit_left->setText(absolutePath);
        fileTableView->setRootIndex(fileModel->index(absolutePath));
        rightStackedWidget->setCurrentIndex(1);
    });
    // 点击切换到文档的按钮
    connect(leftScrollAreaSelectedBtn[4], &myPushButton::clicked, [=] {
        // 设置禁用按钮
        newBuild->setEnabled(true);
        asideBtn_top->setEnabled(true);
        asideBtn_top->setIcon(QIcon(":/res/mainScene_imgs/direction_top_white.png"));
        if (copyCurrentSelectedList.count()) {
            paste->setEnabled(true);
        } else {
            paste->setEnabled(false);
        }
        absolutePath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
        if (absolutePathIterator.count() == 0) {
            absolutePathIterator.append(absolutePath);
            absolutePathIterator_index++;
        } else {
            if (absolutePathIterator.last() != absolutePath) {
                absolutePathIterator << absolutePath;
                absolutePathIterator_index++;
            }
        }
        if (absolutePathIterator_index == 0) {
            asideBtn_right->setEnabled(true);
            asideBtn_left->setEnabled(false);
            asideBtn_right->setIcon(QIcon(":/res/mainScene_imgs/direction_right_white.png"));
            asideBtn_left->setIcon(QIcon(":/res/mainScene_imgs/direction_left_gray.png"));
        } else if (absolutePathIterator_index == absolutePathIterator.count() - 1) {
            asideBtn_right->setEnabled(false);
            asideBtn_left->setEnabled(true);
            asideBtn_right->setIcon(QIcon(":/res/mainScene_imgs/direction_right_gray.png"));
            asideBtn_left->setIcon(QIcon(":/res/mainScene_imgs/direction_left_white.png"));
        } else {
            asideBtn_right->setEnabled(true);
            asideBtn_left->setEnabled(true);
            asideBtn_right->setIcon(QIcon(":/res/mainScene_imgs/direction_right_white.png"));
            asideBtn_left->setIcon(QIcon(":/res/mainScene_imgs/direction_left_white.png"));
        }
        FilesAndDirs_count();
        asideLineEdit_left->setText(absolutePath);
        fileTableView->setRootIndex(fileModel->index(absolutePath));
        rightStackedWidget->setCurrentIndex(1);
    });
    // 点击切换到回收站的按钮
    connect(leftScrollAreaSelectedBtn[5], &myPushButton::clicked, [=] {
        // 设置禁用按钮
        fixedToolWidget_setAllBtnDisable();
        asideBtn_top->setEnabled(false);
        asideBtn_top->setIcon(QIcon(":/res/mainScene_imgs/direction_top_gray.png"));
        absolutePath = "D:\\$RECYCLE.BIN\\S-1-5-21-1781964007-3411722057-4230829593-1001";
        ShellExecute(NULL, "open", "explorer", "::{645FF040-5081-101B-9F08-00AA002F954E}", NULL, SW_SHOW);
        if (absolutePathIterator.count() == 0) {
            absolutePathIterator.append(absolutePath);
            absolutePathIterator_index++;
        } else {
            if (absolutePathIterator.last() != absolutePath) {
                absolutePathIterator.append(absolutePath);
                absolutePathIterator_index++;
            }
        }
        if (absolutePathIterator_index == 0) {
            asideBtn_right->setEnabled(true);
            asideBtn_left->setEnabled(false);
            asideBtn_right->setIcon(QIcon(":/res/mainScene_imgs/direction_right_white.png"));
            asideBtn_left->setIcon(QIcon(":/res/mainScene_imgs/direction_left_gray.png"));
        } else if (absolutePathIterator_index == absolutePathIterator.count() - 1) {
            asideBtn_right->setEnabled(false);
            asideBtn_left->setEnabled(true);
            asideBtn_right->setIcon(QIcon(":/res/mainScene_imgs/direction_right_gray.png"));
            asideBtn_left->setIcon(QIcon(":/res/mainScene_imgs/direction_left_white.png"));
        } else {
            asideBtn_right->setEnabled(true);
            asideBtn_left->setEnabled(true);
            asideBtn_right->setIcon(QIcon(":/res/mainScene_imgs/direction_right_white.png"));
            asideBtn_left->setIcon(QIcon(":/res/mainScene_imgs/direction_left_white.png"));
        }
        FilesAndDirs_count();
        asideLineEdit_left->setText("回收站内容");
        fileTableView->setRootIndex(
                fileModel->index(absolutePath));
        rightStackedWidget->setCurrentIndex(1);
    });
    // 点击驱动器切换到C盘的按钮
    connect(leftScrollAreaSelectedBtn[7], &myPushButton::clicked, [=] {
        // 设置禁用按钮
        newBuild->setEnabled(true);
        asideBtn_top->setEnabled(true);
        asideBtn_top->setIcon(QIcon(":/res/mainScene_imgs/direction_top_white.png"));
        if (copyCurrentSelectedList.count()) {
            paste->setEnabled(true);
        } else {
            paste->setEnabled(false);
        }
        absolutePath = "C:";
        if (absolutePathIterator.count() == 0) {
            absolutePathIterator.append(absolutePath);
            absolutePathIterator_index++;
        } else {
            if (absolutePathIterator.last() != absolutePath) {
                absolutePathIterator.append(absolutePath);
                absolutePathIterator_index++;
            }
        }
        if (absolutePathIterator_index == 0) {
            asideBtn_right->setEnabled(true);
            asideBtn_left->setEnabled(false);
            asideBtn_right->setIcon(QIcon(":/res/mainScene_imgs/direction_right_white.png"));
            asideBtn_left->setIcon(QIcon(":/res/mainScene_imgs/direction_left_gray.png"));
        } else if (absolutePathIterator_index == absolutePathIterator.count() - 1) {
            asideBtn_right->setEnabled(false);
            asideBtn_left->setEnabled(true);
            asideBtn_right->setIcon(QIcon(":/res/mainScene_imgs/direction_right_gray.png"));
            asideBtn_left->setIcon(QIcon(":/res/mainScene_imgs/direction_left_white.png"));
        } else {
            asideBtn_right->setEnabled(true);
            asideBtn_left->setEnabled(true);
            asideBtn_right->setIcon(QIcon(":/res/mainScene_imgs/direction_right_white.png"));
            asideBtn_left->setIcon(QIcon(":/res/mainScene_imgs/direction_left_white.png"));
        }
        FilesAndDirs_count();
        asideLineEdit_left->setText("C:");
        fileTableView->setRootIndex(
                fileModel->index("C:"));
        rightStackedWidget->setCurrentIndex(1);
    });
    // 点击驱动器切换到D盘的按钮
    connect(leftScrollAreaSelectedBtn[8], &myPushButton::clicked, [=] {
        // 设置禁用按钮
        newBuild->setEnabled(true);
        asideBtn_top->setEnabled(true);
        asideBtn_top->setIcon(QIcon(":/res/mainScene_imgs/direction_top_white.png"));
        if (copyCurrentSelectedList.count()) {
            paste->setEnabled(true);
        } else {
            paste->setEnabled(false);
        }
        absolutePath = "D:";
        if (absolutePathIterator.count() == 0) {
            absolutePathIterator.append(absolutePath);
            absolutePathIterator_index++;
        } else {
            if (absolutePathIterator.last() != absolutePath) {
                absolutePathIterator.append(absolutePath);
                absolutePathIterator_index++;
            }
        }
        if (absolutePathIterator_index == 0) {
            asideBtn_right->setEnabled(true);
            asideBtn_left->setEnabled(false);
            asideBtn_right->setIcon(QIcon(":/res/mainScene_imgs/direction_right_white.png"));
            asideBtn_left->setIcon(QIcon(":/res/mainScene_imgs/direction_left_gray.png"));
        } else if (absolutePathIterator_index == absolutePathIterator.count() - 1) {
            asideBtn_right->setEnabled(false);
            asideBtn_left->setEnabled(true);
            asideBtn_right->setIcon(QIcon(":/res/mainScene_imgs/direction_right_gray.png"));
            asideBtn_left->setIcon(QIcon(":/res/mainScene_imgs/direction_left_white.png"));
        } else {
            asideBtn_right->setEnabled(true);
            asideBtn_left->setEnabled(true);
            asideBtn_right->setIcon(QIcon(":/res/mainScene_imgs/direction_right_white.png"));
            asideBtn_left->setIcon(QIcon(":/res/mainScene_imgs/direction_left_white.png"));
        }
        FilesAndDirs_count();
        asideLineEdit_left->setText("D:");
        fileTableView->setRootIndex(
                fileModel->index("D:"));
        rightStackedWidget->setCurrentIndex(1);
    });
    // 点击切换到oneDrive的按钮
    connect(leftScrollAreaSelectedBtn[10], &myPushButton::clicked, [=] {
        // 设置禁用按钮
        newBuild->setEnabled(true);
        asideBtn_top->setEnabled(true);
        asideBtn_top->setIcon(QIcon(":/res/mainScene_imgs/direction_top_white.png"));
        absolutePath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/OneDrive";
        if (absolutePathIterator.count() == 0) {
            absolutePathIterator.append(absolutePath);
            absolutePathIterator_index++;
        } else {
            if (absolutePathIterator.last() != absolutePath) {
                absolutePathIterator.append(absolutePath);
                absolutePathIterator_index++;
            }
        }
        if (absolutePathIterator_index == 0) {
            asideBtn_right->setEnabled(true);
            asideBtn_left->setEnabled(false);
            asideBtn_right->setIcon(QIcon(":/res/mainScene_imgs/direction_right_white.png"));
            asideBtn_left->setIcon(QIcon(":/res/mainScene_imgs/direction_left_gray.png"));
        } else if (absolutePathIterator_index == absolutePathIterator.count() - 1) {
            asideBtn_right->setEnabled(false);
            asideBtn_left->setEnabled(true);
            asideBtn_right->setIcon(QIcon(":/res/mainScene_imgs/direction_right_gray.png"));
            asideBtn_left->setIcon(QIcon(":/res/mainScene_imgs/direction_left_white.png"));
        } else {
            asideBtn_right->setEnabled(true);
            asideBtn_left->setEnabled(true);
            asideBtn_right->setIcon(QIcon(":/res/mainScene_imgs/direction_right_white.png"));
            asideBtn_left->setIcon(QIcon(":/res/mainScene_imgs/direction_left_white.png"));
        }
        FilesAndDirs_count();
        asideLineEdit_left->setText(absolutePath);
        fileTableView->setRootIndex(
                fileModel->index(absolutePath));
        rightStackedWidget->setCurrentIndex(1);
    });
    // 点击切换到网络的按钮 TODO
    connect(leftScrollAreaSelectedBtn[12], &myPushButton::clicked, [=] {
        // 设置禁用的按钮
        asideBtn_top->setEnabled(false);
        asideBtn_top->setIcon(QIcon(":/res/mainScene_imgs/direction_top_gray.png"));
        if (absolutePathIterator.count() == 0) {
            absolutePathIterator.append("网络：");
            absolutePathIterator_index++;
        } else {
            if (absolutePathIterator.last() != "网络：") {
                absolutePathIterator.append("网络：");
                absolutePathIterator_index++;
            }
        }
        if (absolutePathIterator_index == 0) {
            asideBtn_right->setEnabled(true);
            asideBtn_left->setEnabled(false);
            asideBtn_right->setIcon(QIcon(":/res/mainScene_imgs/direction_right_white.png"));
            asideBtn_left->setIcon(QIcon(":/res/mainScene_imgs/direction_left_gray.png"));
        } else if (absolutePathIterator_index == absolutePathIterator.count() - 1) {
            asideBtn_right->setEnabled(false);
            asideBtn_left->setEnabled(true);
            asideBtn_right->setIcon(QIcon(":/res/mainScene_imgs/direction_right_gray.png"));
            asideBtn_left->setIcon(QIcon(":/res/mainScene_imgs/direction_left_white.png"));
        } else {
            asideBtn_right->setEnabled(true);
            asideBtn_left->setEnabled(true);
            asideBtn_right->setIcon(QIcon(":/res/mainScene_imgs/direction_right_white.png"));
            asideBtn_left->setIcon(QIcon(":/res/mainScene_imgs/direction_left_white.png"));
        }
        statusMsg_first->setText("共0项");
        fixedToolWidget_setAllBtnDisable();
        asideLineEdit_left->setText("网络：");
        rightStackedWidget->setCurrentIndex(2);
    });

    // 点击展示文件夹栏的桌面按钮
    connect(desktopFolderBtn, &myPushButton::clicked, [=]() {
        // 同样要设置侧边栏被点击的逻辑
        leftScrollAreaSelectedBtn[2]->clicked(true);
        asideLineEdit_left->setText(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation));
        fileTableView->setRootIndex(
                fileModel->index(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation)));
        rightStackedWidget->setCurrentIndex(1);
    });
    // 点击展示文件夹栏的下载按钮
    connect(downloadFolderBtn, &myPushButton::clicked, [=] {
        // 同样要设置侧边栏被点击的逻辑
        leftScrollAreaSelectedBtn[3]->clicked(true);
        asideLineEdit_left->setText(QStandardPaths::writableLocation(QStandardPaths::DownloadLocation));
        fileTableView->setRootIndex(fileModel->index(absolutePath));
        rightStackedWidget->setCurrentIndex(1);
    });
    // 点击展示文件夹栏的文档按钮
    connect(documentFolderBtn, &myPushButton::clicked, [=] {
        // 同样要设置侧边栏被点击的逻辑
        leftScrollAreaSelectedBtn[4]->clicked(true);
        asideLineEdit_left->setText(absolutePath);
        fileTableView->setRootIndex(fileModel->index(absolutePath));
        rightStackedWidget->setCurrentIndex(1);
    });
    // 点击展示文件夹栏的音乐按钮
    connect(musicFolderBtn, &myPushButton::clicked, [=] {
        // 设置禁用按钮
        newBuild->setEnabled(true);
        asideBtn_top->setEnabled(true);
        asideBtn_top->setIcon(QIcon(":/res/mainScene_imgs/direction_top_white.png"));
        if (copyCurrentSelectedList.count()) {
            paste->setEnabled(true);
        } else {
            paste->setEnabled(false);
        }
        absolutePath = QStandardPaths::writableLocation(QStandardPaths::MusicLocation);
        if (absolutePathIterator.count() == 0) {
            absolutePathIterator.append(absolutePath);
            absolutePathIterator_index++;
        } else {
            if (absolutePathIterator.last() != absolutePath) {
                absolutePathIterator.append(absolutePath);
                absolutePathIterator_index++;
            }
        }
        if (absolutePathIterator_index == 0) {
            asideBtn_right->setEnabled(true);
            asideBtn_left->setEnabled(false);
            asideBtn_right->setIcon(QIcon(":/res/mainScene_imgs/direction_right_white.png"));
            asideBtn_left->setIcon(QIcon(":/res/mainScene_imgs/direction_left_gray.png"));
        } else if (absolutePathIterator_index == absolutePathIterator.count() - 1) {
            asideBtn_right->setEnabled(false);
            asideBtn_left->setEnabled(true);
            asideBtn_right->setIcon(QIcon(":/res/mainScene_imgs/direction_right_gray.png"));
            asideBtn_left->setIcon(QIcon(":/res/mainScene_imgs/direction_left_white.png"));
        } else {
            asideBtn_right->setEnabled(true);
            asideBtn_left->setEnabled(true);
            asideBtn_right->setIcon(QIcon(":/res/mainScene_imgs/direction_right_white.png"));
            asideBtn_left->setIcon(QIcon(":/res/mainScene_imgs/direction_left_white.png"));
        }
        FilesAndDirs_count();
        asideLineEdit_left->setText(absolutePath);
        fileTableView->setRootIndex(fileModel->index(absolutePath));
        rightStackedWidget->setCurrentIndex(1);
    });
    // 点击展示文件夹栏的图片按钮
    connect(pictureFolderBtn, &myPushButton::clicked, [=] {
        // 设置禁用按钮
        newBuild->setEnabled(true);
        asideBtn_top->setEnabled(true);
        asideBtn_top->setIcon(QIcon(":/res/mainScene_imgs/direction_top_white.png"));
        if (copyCurrentSelectedList.count()) {
            paste->setEnabled(true);
        } else {
            paste->setEnabled(false);
        }
        absolutePath = QStandardPaths::writableLocation(QStandardPaths::PicturesLocation);
        if (absolutePathIterator.count() == 0) {
            absolutePathIterator.append(absolutePath);
            absolutePathIterator_index++;
        } else {
            if (absolutePathIterator.last() != absolutePath) {
                absolutePathIterator.append(absolutePath);
                absolutePathIterator_index++;
            }
        }
        if (absolutePathIterator_index == 0) {
            asideBtn_right->setEnabled(true);
            asideBtn_left->setEnabled(false);
            asideBtn_right->setIcon(QIcon(":/res/mainScene_imgs/direction_right_white.png"));
            asideBtn_left->setIcon(QIcon(":/res/mainScene_imgs/direction_left_gray.png"));
        } else if (absolutePathIterator_index == absolutePathIterator.count() - 1) {
            asideBtn_right->setEnabled(false);
            asideBtn_left->setEnabled(true);
            asideBtn_right->setIcon(QIcon(":/res/mainScene_imgs/direction_right_gray.png"));
            asideBtn_left->setIcon(QIcon(":/res/mainScene_imgs/direction_left_white.png"));
        } else {
            asideBtn_right->setEnabled(true);
            asideBtn_left->setEnabled(true);
            asideBtn_right->setIcon(QIcon(":/res/mainScene_imgs/direction_right_white.png"));
            asideBtn_left->setIcon(QIcon(":/res/mainScene_imgs/direction_left_white.png"));
        }
        FilesAndDirs_count();
        asideLineEdit_left->setText(absolutePath);
        fileTableView->setRootIndex(fileModel->index(absolutePath));
        rightStackedWidget->setCurrentIndex(1);
    });
    // 点击展示文件夹栏的视频按钮
    connect(moviesFolderBtn, &myPushButton::clicked, [=] {
        // 设置禁用按钮
        newBuild->setEnabled(true);
        asideBtn_top->setEnabled(true);
        asideBtn_top->setIcon(QIcon(":/res/mainScene_imgs/direction_top_white.png"));
        if (copyCurrentSelectedList.count()) {
            paste->setEnabled(true);
        } else {
            paste->setEnabled(false);
        }
        absolutePath = QStandardPaths::writableLocation(QStandardPaths::MoviesLocation);
        if (absolutePathIterator.count() == 0) {
            absolutePathIterator.append(absolutePath);
            absolutePathIterator_index++;
        } else {
            if (absolutePathIterator.last() != absolutePath) {
                absolutePathIterator.append(absolutePath);
                absolutePathIterator_index++;
            }
        }
        if (absolutePathIterator_index == 0) {
            asideBtn_right->setEnabled(true);
            asideBtn_left->setEnabled(false);
            asideBtn_right->setIcon(QIcon(":/res/mainScene_imgs/direction_right_white.png"));
            asideBtn_left->setIcon(QIcon(":/res/mainScene_imgs/direction_left_gray.png"));
        } else if (absolutePathIterator_index == absolutePathIterator.count() - 1) {
            asideBtn_right->setEnabled(false);
            asideBtn_left->setEnabled(true);
            asideBtn_right->setIcon(QIcon(":/res/mainScene_imgs/direction_right_gray.png"));
            asideBtn_left->setIcon(QIcon(":/res/mainScene_imgs/direction_left_white.png"));
        } else {
            asideBtn_right->setEnabled(true);
            asideBtn_left->setEnabled(true);
            asideBtn_right->setIcon(QIcon(":/res/mainScene_imgs/direction_right_white.png"));
            asideBtn_left->setIcon(QIcon(":/res/mainScene_imgs/direction_left_white.png"));
        }
        FilesAndDirs_count();
        asideLineEdit_left->setText(absolutePath);
        fileTableView->setRootIndex(fileModel->index(absolutePath));
        rightStackedWidget->setCurrentIndex(1);
    });

    // 设置滚动条样式
    // 美化滚动条
    QFile scrollBarQss_left(":/styleSheets/leftScrollBar.qss");
    QFile scrollBarQss_right(":/styleSheets/rightScrollBar.qss");
    QFile scrollBarQss_tableView(":/styleSheets/tableViewScrollBar.qss");
    if (!scrollBarQss_tableView.open(QIODevice::ReadOnly) || !scrollBarQss_left.open(QIODevice::ReadOnly) ||
        !scrollBarQss_right.open(QIODevice::ReadOnly)) {
        qDebug() << "文件打开失败！";
    }
    QByteArray qssContent_left = scrollBarQss_left.readAll();
    QByteArray qssContent_right = scrollBarQss_right.readAll();
    QByteArray qssContent_tableView = scrollBarQss_tableView.readAll();
    auto *scrollBar_left = new QScrollBar;
    auto *scrollBar_right = new QScrollBar;
    auto *scrollBar_tableView = new QScrollBar;
    scrollBar_left->setStyleSheet(qssContent_left);
    scrollBar_right->setStyleSheet(qssContent_right);
    scrollBar_tableView->setStyleSheet(qssContent_tableView);
    leftScrollArea->setVerticalScrollBar(scrollBar_left);
    rightScrollArea->setVerticalScrollBar(scrollBar_right);
    fileTableView->setVerticalScrollBar(scrollBar_tableView);
    scrollBarQss_left.close();
    scrollBarQss_right.close();
    scrollBarQss_tableView.close();

    // 栈控件不是设置Parent形式，而是addWidget形式
    rightStackedWidget->addWidget(rightScrollArea); // index==0
    rightStackedWidget->addWidget(fileTableViewWrapper);   //index==1
    auto *emptyFolderMsg = new QLabel;
    emptyFolderMsg->setText("此文件夹为空");
    emptyFolderMsg->setFont(QFont("kaiTi", 12));
    emptyFolderMsg->setFixedHeight(120);
    emptyFolderMsg->setAlignment(Qt::AlignCenter);
    rightStackedWidget->addWidget(emptyFolderMsg);   //index==2



    // 布局完成，添加组件给布局上
    mainLayout->addWidget(leftScrollArea);
    mainLayout->addWidget(rightWidget);
    mainLayout->setContentsMargins(2, 1, 2, 1);
    // 控制控件的宽度占比
    mainLayout->setStretchFactor(leftScrollArea, 1);
    mainLayout->setStretchFactor(rightWidget, 3);

    // 为主窗口设置布局
    mainWidget->setLayout(mainLayout);
    /*
     * 使用scrollArea,理清楚scrollArea的窗口大小和scrollAreaWidgetContents窗口大小的关系
     * 只要幕布控件scrollAreaWidgetContents的大小超过了QScrollArea的大小，就会自动出现滚动条；
     * 如果幕布比观察窗口还小，那就不会出现滚动条。
     * */

    // 自定义菜单合集

    // 新建一栏的菜单
    newBuild_menu = new QMenu;
    auto *newBuild_menu_action_folder = new QAction("文件夹");
    auto *newBuild_menu_action_file = new QAction("文件");
    auto *newBuild_menu_action_docx = new QAction("Microsoft Word 文档");
    auto *newBuild_menu_action_txt = new QAction("文本文档");
    auto *newBuild_menu_action_md = new QAction("Markdown 文件");
    // 添加icon图标
    newBuild_menu_action_folder->setIcon(QPixmap(":/res/newBuild_menu_imgs/folder.png"));
    newBuild_menu_action_file->setIcon(QPixmap(":/res/newBuild_menu_imgs/file.png"));
    newBuild_menu_action_docx->setIcon(QPixmap(":/res/newBuild_menu_imgs/docx.png"));
    newBuild_menu_action_txt->setIcon(QPixmap(":/res/newBuild_menu_imgs/txt.png"));
    newBuild_menu_action_md->setIcon(QPixmap(":/res/newBuild_menu_imgs/md.png"));

    /*
     * selected等同于hover
     * 设置QAction独有的方式"
     * QMenu::item{border-radius:5px;}
     * QMenu::item:selected{background-color:rgb(80,80,80);
     *
     * */
    newBuild_menu->setStyleSheet("QMenu{background-color:rgb(60,60,60);color:white;border-radius:3px;padding-top:4px;}"
                                 "QMenu::item{border-radius:3px;height:38px;min-width:1px;margin:2px 10px;}"
                                 "QMenu::item:selected{background-color:rgb(80,80,80);}");
    newBuild_menu->setFont(QFont("kaiTi", 12));
    newBuild_menu->addAction(newBuild_menu_action_folder);
    newBuild_menu->addAction(newBuild_menu_action_file);
    newBuild_menu->addSeparator();
    newBuild_menu->addAction(newBuild_menu_action_docx);
    newBuild_menu->addAction(newBuild_menu_action_txt);
    newBuild_menu->addAction(newBuild_menu_action_md);
    // 为新建按钮组件添加上自定义菜单
    newBuild->setMenu(newBuild_menu);


    // 为相应的action添加绑定事件
    // 创建文件夹
    connect(newBuild_menu_action_folder, &QAction::triggered, [=]() {
        actionMenu = new newBuild_menuDialog(absolutePath, 1);
        actionMenu->setAttribute(Qt::WA_DeleteOnClose);
        actionMenu->show();
    });
    // 创建文件
    connect(newBuild_menu_action_file, &QAction::triggered, [=]() {
        actionMenu = new newBuild_menuDialog(absolutePath, 2);
        actionMenu->setAttribute(Qt::WA_DeleteOnClose);
        actionMenu->show();
    });
    connect(newBuild_menu_action_docx, &QAction::triggered, [=]() {
        actionMenu = new newBuild_menuDialog(absolutePath, 3);
        actionMenu->setAttribute(Qt::WA_DeleteOnClose);
        actionMenu->show();
    });
    connect(newBuild_menu_action_txt, &QAction::triggered, [=]() {
        actionMenu = new newBuild_menuDialog(absolutePath, 4);
        actionMenu->setAttribute(Qt::WA_DeleteOnClose);
        actionMenu->show();
    });
    connect(newBuild_menu_action_md, &QAction::triggered, [=]() {
        actionMenu = new newBuild_menuDialog(absolutePath, 5);
        actionMenu->setAttribute(Qt::WA_DeleteOnClose);
        actionMenu->show();
    });



    // 为功能性事件绑定菜单
    connect(cut, &myPushButton::clicked, [=]() {
        isCopyOrCut = false;
        // 复制当前选中的文件和文件夹，启用粘贴按钮
        copyCurrentSelectedList = currentSelectedList;
        paste->setEnabled(true);
    });
    connect(copy, &myPushButton::clicked, [=]() {
        isCopyOrCut = true;
        // 复制当前选中的文件和文件夹，启用粘贴按钮
        copyCurrentSelectedList = currentSelectedList;
        paste->setEnabled(true);
    });
    connect(paste, &myPushButton::clicked, [=]() {
        // 执行的逻辑，剪切：先复制，在删除原文件。复制，直接复制呗
        // 粘贴所有被选中的文件到absolutePath
        auto *msg = new QMessageBox;
        msg->setWindowTitle("提示信息");
        msg->setWindowIcon(QPixmap(":/res/ico/fileLogo.png"));
        msg->setText("是否覆盖重名的文件和文件夹？");
        msg->setIconPixmap(QPixmap(":/res/commonImgs/warning.png"));
        msg->setStyleSheet("QLabel{min-height:50px;}");
        msg->setAttribute(Qt::WA_DeleteOnClose);
        msg->setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        msg->setDefaultButton(QMessageBox::Cancel);
        int ret = msg->exec();
        bool flag_isCoverFileInfoIfExist = false;
        if (ret == QMessageBox::Ok) {
            flag_isCoverFileInfoIfExist = true;
        } else {
            flag_isCoverFileInfoIfExist = false;
        }
        for (auto &i: copyCurrentSelectedList) {
            QString srcPath = fileModel->fileInfo(i).canonicalFilePath();
            QFileInfo tempInfo(srcPath);
            QString tempName = tempInfo.fileName();
            if (tempInfo.isDir()) {
                // 在目的文件夹中加上被复制文件夹的"/"和文件名，才能实现粘贴的逻辑。
                copyFolder(srcPath, absolutePath + u8"/" + tempName, flag_isCoverFileInfoIfExist);
            }
            if (tempInfo.isFile()) {
                copyFile(srcPath, absolutePath + u8"/" + tempName, flag_isCoverFileInfoIfExist);
            }
        }
        if (!isCopyOrCut) {
            // 剪切的逻辑 还要再删除选中的这些文件或文件夹
            for (auto &i: copyCurrentSelectedList) {
                QString srcPath = fileModel->fileInfo(i).filePath();
                QFileInfo tempInfo(srcPath);
                if (tempInfo.isFile()) {
                    QFile tempFile(srcPath);
                    tempFile.remove();
                }
                if (tempInfo.isDir()) {
                    QDir tempDir(srcPath);
                    tempDir.removeRecursively();
                }
            }
        }
    });
    connect(rename, &myPushButton::clicked, [=]() {
        QString tempPath = fileModel->filePath(currentSelectedList.at(0));
        auto *t = new newBuild_menuDialog(tempPath, 7);
        t->setAttribute(Qt::WA_DeleteOnClose);
        t->show();
    });
    connect(deleteTarget, &myPushButton::clicked, [=]() {
        auto *msg = new QMessageBox;
        msg->setWindowTitle("提示信息");
        msg->setWindowIcon(QPixmap(":/res/ico/fileLogo.png"));
        msg->setText("确定要删除吗？删除后的操作不可逆转！");
        msg->setIconPixmap(QPixmap(":/res/commonImgs/warning.png"));
        msg->setStyleSheet("QLabel{min-height:50px;}");
        msg->setAttribute(Qt::WA_DeleteOnClose);
        msg->setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        msg->setDefaultButton(QMessageBox::Cancel);
        int ret = msg->exec();
        if (ret == QMessageBox::Ok) {
            for (auto &i: currentSelectedList) {
                QFileInfo fileInfo(fileModel->filePath(i));
                if (fileInfo.isDir()) {
                    QDir temp(fileInfo.filePath());
                    temp.removeRecursively();
                }
                if (fileInfo.isFile()) {
                    QFile file(fileInfo.filePath());
                    file.remove();
                }
            }
        }

    });
    connect(itemProperty, &myPushButton::clicked, [=]() {
        auto *pDialog = new itemPropertyDialog(fileModel->filePath(currentSelectedList.at(0)));
        pDialog->setAttribute(Qt::WA_DeleteOnClose);
        pDialog->show();
    });
}

Widget::~Widget() {
    delete ui;
}

void Widget::packageWrapperStoredWidgetGridLayoutUpdate(int removeIndex) {
    addPackageFileInfoList.removeAt(removeIndex);
    for (int i = 0; i < addPackageFileInfoList.count(); i++) {
        packageWrapperStoredWidgetGridLayout->removeWidget(packageWrapperStoredWidgetGridLayoutItems[i]);
        delete packageWrapperStoredWidgetGridLayoutItems[i];
    }
}

void Widget::removePackage() {
    QString dirPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    QFile jsonData(dirPath + "/FileSystem/data/files.json");
    QFile packageNameFile(dirPath + "/FileSystem/data/packageName.txt");
    // 删除文件内容
    jsonData.remove();
    packageNameFile.remove();

    // 移除所有标题一栏的控件(设为不可见)
    packageTitleLabel->setVisible(false);
    packageWrapperTitleLabel_setting->setVisible(false);

    // 再根据addPackageFileInfoList移除相应的控件
    for (int i = 0; i < addPackageFileInfoList.count(); i++) {
        packageWrapperStoredWidgetGridLayout->removeWidget(packageWrapperStoredWidgetGridLayoutItems[i]);
        delete packageWrapperStoredWidgetGridLayoutItems[i];
    }

    packageWrapperStoredWidgetGridLayout_row = 0;
    packageWrapperStoredWidgetGridLayout_col = 0;
    packageName = "";
    addPackageFileInfoList.clear();
}

void Widget::loadPackageFiles() {
    QString dirPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    QFile jsonData(dirPath + "/FileSystem/data/files.json");
    QFile packageNameFile(dirPath + "/FileSystem/data/packageName.txt");

    QDir myDir(dirPath);
    if (!myDir.exists("FileSystem/data")) {
        myDir.mkdir("FileSystem/data");
    }

    // 单独处理包的名称的内容
    packageNameFile.open(QIODevice::ReadWrite);
    packageName = packageNameFile.readAll();

    // 处理布局控件的内容
    if (jsonData.open(QIODevice::ReadWrite)) {
        QByteArray allJsonContent = jsonData.readAll();
        QJsonParseError jsonError;
        QJsonDocument jsonDoc(QJsonDocument::fromJson(allJsonContent, &jsonError));
        if (jsonError.error == QJsonParseError::NoError) {
            QJsonObject jsonObj = jsonDoc.object();
            QJsonArray jsonArray = jsonObj.value("addPackageFilePath").toArray();
            for (int i = 0; i < jsonArray.count(); i++) {
                addPackageFileInfoList.append(jsonArray.at(i).toString());
            }
        }
    }
    jsonData.close();
}

void Widget::writeIntoPackageJsonFiles(QString fileName, bool isRenamePackage, int removeIndex) {
    QString dirPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    if (isRenamePackage) {
        // 单独处理
        QFile packageNameFile(dirPath + "/FileSystem/data/packageName.txt");
        if (packageNameFile.open(QIODevice::ReadWrite))
            packageNameFile.write(fileName.toUtf8());
        return;
    }
    QFile jsonData(dirPath + "/FileSystem/data/files.json");
    QJsonObject obj;
    QJsonArray array;
    QJsonDocument doc;
    QByteArray data;
    if (jsonData.open(QIODevice::ReadWrite)) {
        QByteArray allJsonContent = jsonData.readAll();
        QJsonParseError jsonError;
        QJsonDocument jsonDoc(QJsonDocument::fromJson(allJsonContent, &jsonError));
        if (jsonError.error == QJsonParseError::NoError) {
            QJsonObject jsonObj = jsonDoc.object();
            QJsonArray jsonArray = jsonObj.value("addPackageFilePath").toArray();
            array = jsonArray;
        } else {
            array.append(fileName);
            obj.insert("addPackageFilePath", array);
            doc.setObject(obj);
            data = doc.toJson();
            jsonData.write(data);
            return;
        }
    }
    jsonData.close();
    jsonData.open(QIODevice::WriteOnly);
    if (removeIndex > -1) {
        array.removeAt(removeIndex);
    } else {
        bool flag = false;
        for (auto &&i: array) {
            if (i.toString() == fileName) {
                flag = true;
                break;
            }
        }
        if (!flag) {
            array.append(fileName);
        }
    }
    obj.insert("addPackageFilePath", array);
    doc.setObject(obj);
    data = doc.toJson();
    jsonData.write(data);
    jsonData.close();
    jsonData.close();
}

void Widget::receiveData_handleRenamePackage(QString data, int isCancel) {
    if (!isCancel) {
        packageName = data;
        packageTitleLabel->setText(packageName);
        writeIntoPackageJsonFiles(packageName, true);
    }
}


void Widget::receiveData_handleCreatePackage(QString data, int isCancel) {
    // 执行一个函数 后将按钮设为不可见
    if (isCancel) {
        createPackageBtn->setVisible(true);
    } else {
        packageName = data;
        packageTitleLabel->setText(packageName);
        packageTitleLabel->setVisible(true);
        packageWrapperTitleLabel_setting->setVisible(true);
        createPackageBtn->setVisible(false);
        // 写入json文件
        writeIntoPackageJsonFiles(packageName, true);
    }

}

void Widget::loadRecentUsingFiles() {
    QString dirPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    QFile jsonData(dirPath + "/FileSystem/files.json");
    QDir myDir(dirPath);
    if (!myDir.exists("FileSystem")) {
        myDir.mkdir("FileSystem");
    }
    if (jsonData.exists()) {
        if (jsonData.open(QIODevice::ReadOnly)) {
            QByteArray allJsonContent = jsonData.readAll();
            QJsonParseError jsonError;
            QJsonDocument jsonDoc(QJsonDocument::fromJson(allJsonContent, &jsonError));
            if (jsonError.error == QJsonParseError::NoError) {
                QJsonObject jsonObj = jsonDoc.object();
                QJsonArray jsonArray = jsonObj.value("filePath").toArray();
                for (auto &&i: jsonArray) {
                    // 获得了json中的数值
                    recentUsingFiles.append(i.toString());
                }
            }
        }
    }
    jsonData.close();
}

void Widget::writeIntoRecentUsingJsonFiles(QString fileName) {
    QString dirPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    QFile jsonData(dirPath + "/FileSystem/files.json");

    QJsonObject obj;
    QJsonArray array;
    QJsonDocument doc;
    QByteArray data;
    if (jsonData.open(QIODevice::ReadWrite)) {
        QByteArray allJsonContent = jsonData.readAll();
        QJsonParseError jsonError;
        QJsonDocument jsonDoc(QJsonDocument::fromJson(allJsonContent, &jsonError));
        if (jsonError.error == QJsonParseError::NoError) {
            QJsonObject jsonObj = jsonDoc.object();
            QJsonArray jsonArray = jsonObj.value("filePath").toArray();
            array = jsonArray;
        } else {
            array.append(fileName);
            obj.insert("filePath", array);
            doc.setObject(obj);
            data = doc.toJson();
            jsonData.write(data);
            return;
        }
    }
    jsonData.close();
    jsonData.open(QIODevice::WriteOnly);
    bool flag = false;
    for (int i = 0; i < array.size(); i++) {
        if (array.at(i).toString() == fileName) {
            flag = true;
            break;
        }
    }
    if (!flag) {
        array.append(fileName);
        obj.insert("filePath", array);
        doc.setObject(obj);
        data = doc.toJson();
        jsonData.write(data);
        jsonData.close();
    } else {
        obj.insert("filePath", array);
        doc.setObject(obj);
        data = doc.toJson();
        jsonData.write(data);
        jsonData.close();
    }
}

bool Widget::searchFilesAndFolders(QString path, QString searchName) {
    // 搜索文件或文件夹
    QDir dir(path);
    if (!dir.exists()) {
        return false;
    }
    dir.setFilter(QDir::Dirs | QDir::Files);
    dir.setSorting(QDir::DirsFirst);//文件夹排在前面
    QFileInfoList list = dir.entryInfoList();
    int i = 0;
    do {
        QFileInfo fileInfo = list.at(i);
        if (fileInfo.fileName() == "." | fileInfo.fileName() == "..") {
            ++i;
            continue;
        }
        if (fileInfo.isDir()) {
            //目录处理
            if (fileInfo.fileName().contains(searchName)) {
                searchFolders.append(fileInfo.canonicalFilePath());
            }
            searchFilesAndFolders(fileInfo.filePath(), searchName);
        } else {
            //文件处理
            if (fileInfo.fileName().contains(searchName)) {
                searchFiles.append(fileInfo.canonicalFilePath());
            }
        }
        ++i;
    } while (i < list.size());
    return true;
}


void Widget::setEnableBtnFromAbsolutePath() {
    if (absolutePath == "主页：") {
        changFlagToTrueWithOtherFlagFalse(1);
        showSelectedBtn(leftScrollAreaSelectedBtn_selected_flag);
    } else if (absolutePath == QStandardPaths::writableLocation(QStandardPaths::DesktopLocation)) {
        changFlagToTrueWithOtherFlagFalse(2);
        showSelectedBtn(leftScrollAreaSelectedBtn_selected_flag);
    } else if (absolutePath == QStandardPaths::writableLocation(QStandardPaths::DownloadLocation)) {
        changFlagToTrueWithOtherFlagFalse(3);
        showSelectedBtn(leftScrollAreaSelectedBtn_selected_flag);
    } else if (absolutePath == QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)) {
        changFlagToTrueWithOtherFlagFalse(4);
        showSelectedBtn(leftScrollAreaSelectedBtn_selected_flag);
    } else if (absolutePath == "D:\\$RECYCLE.BIN\\S-1-5-21-1781964007-3411722057-4230829593-1001") {
        changFlagToTrueWithOtherFlagFalse(5);
        showSelectedBtn(leftScrollAreaSelectedBtn_selected_flag);
    } else if (absolutePath == "C:") {
        changFlagToTrueWithOtherFlagFalse(7);
        showSelectedBtn(leftScrollAreaSelectedBtn_selected_flag);
    } else if (absolutePath == "D:") {
        changFlagToTrueWithOtherFlagFalse(8);
        showSelectedBtn(leftScrollAreaSelectedBtn_selected_flag);
    } else if (absolutePath == QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/OneDrive") {
        changFlagToTrueWithOtherFlagFalse(10);
        showSelectedBtn(leftScrollAreaSelectedBtn_selected_flag);
    } else if (absolutePath == "网络") {
        changFlagToTrueWithOtherFlagFalse(12);
        showSelectedBtn(leftScrollAreaSelectedBtn_selected_flag);
    }
}

quint64 Widget::getCurrentDirSize(QString dirPath) {
    QString srcDir = fileModel->fileInfo(fileModel->index(dirPath)).canonicalFilePath();
    QDir tmpDir(srcDir);
    quint64 size = 0;

    /*获取文件列表  统计文件大小*/
            foreach(QFileInfo
                            fileInfo, tmpDir.entryInfoList(QDir::Files)) {
            size += fileInfo.size();
        }

    /*获取文件夹  并且过滤掉.和..文件夹 统计各个文件夹的文件大小 */
            foreach(QString
                            subDir, tmpDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
            size += getCurrentDirSize(srcDir + QDir::separator() + subDir); //递归进行  统计所有子目录
        }
    return size;
}

bool Widget::copyFolder(const QString &oldDir, const QString &newDir, bool coverFileIfExist) {
    QString srcDir = fileModel->fileInfo(fileModel->index(oldDir)).canonicalFilePath();
    QDir sourceDir(srcDir);
    QDir targetDir(newDir);

    if (!targetDir.exists()) { //目的文件目录不存在则创建文件目录
        if (!targetDir.mkdir(targetDir.absolutePath()))
            return false;
    }
    QFileInfoList fileInfoList = sourceDir.entryInfoList();
            foreach(QFileInfo
                            fileInfo, fileInfoList) {
            if (fileInfo.fileName() == "." || fileInfo.fileName() == "..")
                continue;

            if (fileInfo.isDir()) {    // 当为目录时，递归的进行copy
                if (!copyFolder(fileInfo.filePath(), targetDir.filePath(fileInfo.fileName()), coverFileIfExist))
                    return false;
            } else {            //当允许覆盖操作时，将旧文件进行删除操作
                if (coverFileIfExist && targetDir.exists(fileInfo.fileName())) {
                    targetDir.remove(fileInfo.fileName());
                }
                /// 进行文件copy
                if (!QFile::copy(fileInfo.filePath(), targetDir.filePath(fileInfo.fileName()))) {
                    return false;
                }
            }
        }
    return true;
}

bool Widget::copyFile(const QString &oldDir, const QString &newDir, bool coverFileIfExist) {
    if (oldDir == newDir) {
        return true;
    }

    if (!QFile::exists(oldDir)) {  //源文件不存在
        return false;
    }

    if (QFile::exists(newDir)) {   // 目的路径已存在该文件
        if (coverFileIfExist) {
            QFile::remove(newDir);
        }
    }

    if (!QFile::copy(oldDir, newDir)) {
        return false;
    }
    return true;
}

void Widget::fixedToolWidget_setFunctionalBtnDisable() {
    cut->setEnabled(false);
    copy->setEnabled(false);
    paste->setEnabled(true);
    rename->setEnabled(false);
    deleteTarget->setEnabled(false);
    itemProperty->setEnabled(false);
}


void Widget::fixedToolWidget_setFunctionalBtnEnable() {
    cut->setEnabled(true);
    copy->setEnabled(true);
    paste->setEnabled(false);
    rename->setEnabled(true);
    deleteTarget->setEnabled(true);
    itemProperty->setEnabled(true);
}


void Widget::handleSelectionChanged() {

    copy->setEnabled(true);
    currentSelectedList = fileTableView->selectionModel()->selectedIndexes();
}

void Widget::FilesAndDirs_count() {
    QDir myDir(absolutePath);
    QStringList list = myDir.entryList(QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Files);
    statusMsg_first->setText(QString("共%1项").arg(list.count()));
}


void Widget::fixedToolWidget_setAllBtnDisable() {
    newBuild->setEnabled(false);
    copy->setEnabled(false);
    cut->setEnabled(false);
    paste->setEnabled(false);
    rename->setEnabled(false);
    deleteTarget->setEnabled(false);
    itemProperty->setEnabled(false);
}


void Widget::homePageAllBtn_setEnable() {
    newBuild->setEnabled(false);
    copy->setEnabled(false);
    cut->setEnabled(false);
    paste->setEnabled(false);
    rename->setEnabled(false);
    deleteTarget->setEnabled(false);
    itemProperty->setEnabled(false);
}

DWORD Widget::ListFileInRecycleBin() {
    CHAR pszPath[MAX_PATH];     // 保存路径
    // IShellFolder接口
    IShellFolder *pisf = nullptr;
    IShellFolder *pisfRecBin = nullptr;
    // 获取“根”目录，桌面
    SHGetDesktopFolder(&pisfRecBin);

    IEnumIDList *peidl = nullptr;  // 对象遍历接口
    LPITEMIDLIST pidlBin = nullptr;
    LPITEMIDLIST idlCurrent = nullptr;

    LPMALLOC pMalloc = nullptr;
    // 分配
    SHGetMalloc(&pMalloc);
    // 回收站位置
    SHGetFolderLocation(nullptr, CSIDL_BITBUCKET, nullptr, 0, &pidlBin);
    // 绑定回收站对象
    pisfRecBin->BindToObject(pidlBin, nullptr, IID_IShellFolder, (void **) &pisf);
    // 列举回收站中的对象,得到IEnumIDList接口，包括SHCONTF_FOLDERS、
    // SHCONTF_NONFOLDERS、SHCONTF_INCLUDEHIDDEN类型的对象
    pisf->EnumObjects(nullptr, SHCONTF_FOLDERS | SHCONTF_NONFOLDERS | SHCONTF_INCLUDEHIDDEN, &peidl);

    STRRET strRet;
    ULONG uFetched;

    HANDLE hOutPut = GetStdHandle(STD_OUTPUT_HANDLE);
    printf("\nFiles In Recycle Bin:\n");
    while (true) {
        // 遍历IEnumIDList对象，idlCurrent为当前对象
        if (peidl->Next(1, &idlCurrent, &uFetched) == S_FALSE)
            break;
        // 获取回收站当前对象当前的路径，这里没有输出结果，读者可自行修改
        SHGetPathFromIDList(idlCurrent, pszPath);
        // DisplayName，删除前的路径
        pisf->GetDisplayNameOf(idlCurrent, SHGDN_NORMAL, &strRet);
        // 显示，printf可能会造成字符编码不正确。
        WriteConsoleW(hOutPut, L"\t", 1, nullptr, nullptr);

        WriteConsoleW(hOutPut, strRet.pOleStr, lstrlenW(strRet.pOleStr), nullptr, nullptr);

        WriteConsoleW(hOutPut, L"\n", 1, nullptr, nullptr);
    }
    // 释放资源
    pMalloc->Free(pidlBin);
    pMalloc->Free(strRet.pOleStr);
    pMalloc->Release();
    peidl->Release();
    pisf->Release();
    return 0;
}


void Widget::onTableViewDoubleClicked() {
    QList<QString> absPath;
    QModelIndex tmpItem = fileTableView->currentIndex();
    while (tmpItem.isValid()) {
        absPath << fileTableView->model()->itemData(tmpItem).values()[0].toString();
        tmpItem = tmpItem.parent();
    }
    // 得到文件路径（快捷方式和正常的路径）
    QString strPath;
    for (int i = absPath.size() - 1; i >= 0; --i) {
        // 替换字符串
        absPath.operator[](i).replace("本地磁盘 (D:)", "D:");
        absPath.operator[](i).replace("本地磁盘 (C:)", "C:");
        strPath += absPath.at(i);
        if (i != 0) {
            strPath += u8"/";
        }
    }
    QDir myDir(strPath);
    QStringList list = myDir.entryList(QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Files);

    QFileInfo fileInfo = fileModel->fileInfo(fileModel->index(strPath));

    absolutePath = fileInfo.canonicalFilePath();
    if (absolutePathIterator.count() == 0) {
        absolutePathIterator.append(absolutePath);
        absolutePathIterator_index++;
    } else {
        if (absolutePathIterator.last() != absolutePath) {
            absolutePathIterator.append(absolutePath);
            absolutePathIterator_index++;
        }
    }
    // 双击某个文件夹后，直接设置为禁用按钮；
    fixedToolWidget_setFunctionalBtnDisable();

//    一下两个是规范文件路径（绝对路径）
//    fileInfo.canonicalFilePath();
//    fileInfo.canonicalPath();
    if (fileInfo.isFile()) {
        if (recentUsingFiles.count() == 0) {
            defaultShowLabel->setVisible(false);
        }
        // 判重，重复则不加入
        bool isRepeat = false;
        for (int i = 0; i < recentUsingFiles.count(); i++) {
            if (recentUsingFiles.at(i) == fileInfo.canonicalFilePath()) {
                isRepeat = true;
                break;
            }
        }
        if (!isRepeat) {
            recentUsingFiles.append(fileInfo.canonicalFilePath());
            writeIntoRecentUsingJsonFiles(fileInfo.canonicalFilePath());
            auto *pItem = new myPushButton(13, fileInfo.canonicalFilePath());
            pItem->setFixedHeight(42);
            connect(pItem, &myPushButton::clicked, [=]() {
                QDesktopServices::openUrl(QUrl::fromLocalFile(fileInfo.filePath()));
            });
            showRecentUsingItemsVLayout->addWidget(pItem);
        }
        QDesktopServices::openUrl(QUrl::fromLocalFile(absolutePath));
    } else {
        // 更新显示的信息
        statusMsg_first->setText(QString("共%1项").arg(list.count()));
        asideLineEdit_left->setText(absolutePath);
        fileTableView->setRootIndex(fileModel->index(absolutePath));
    }
    if (fileTableView->currentIndex().data() == QVariant("..")) {
        fileTableView->setRootIndex(fileTableView->currentIndex().parent().parent());
    }
}

bool Widget::eventFilter(QObject *obj, QEvent *e) {
    switch (e->type()) {
        case QEvent::Resize:
            mainWidget->setGeometry(5, 84, this->width() - 10, this->height() - 75);
            asideWidget->setGeometry(5, 34, this->width() - 10, 50);
            return true;
        default:
            return QWidget::eventFilter(obj, e);
    }
}

void Widget::paintEvent(QPaintEvent *) {
//    绘制背景颜色
    QPainter painter(this);
    painter.setBrush(QColor(30, 30, 30));
    painter.drawRect(-1, -1, this->width() + 1, 36);
    QPen pen(QColor(55, 0, 0));
    painter.setPen(pen);
    painter.drawLine(QPoint(0, 35), QPoint(this->width(), 35));
    painter.setBrush(QColor(33, 33, 33));
    painter.drawRect(-1, 36, this->width() + 1, this->height() - 35);
}

bool Widget::nativeEvent(const QByteArray &eventType, void *message, long *result) {
    Q_UNUSED(eventType)

    MSG *param = static_cast<MSG *>(message);

    switch (param->message) {
        case WM_NCHITTEST: {
            int nX = GET_X_LPARAM(param->lParam) - this->geometry().x();
            int nY = GET_Y_LPARAM(param->lParam) - this->geometry().y();

            // 如果鼠标位于子控件上，则不进行处理
            if (childAt(nX, nY) != nullptr)
                return QWidget::nativeEvent(eventType, message, result);

            *result = HTCAPTION;

            // 鼠标区域位于窗体边框，进行缩放
            if ((nX > 0) && (nX < m_nBorderWidth))
                *result = HTLEFT;

            if ((nX > this->width() - m_nBorderWidth) && (nX < this->width()))
                *result = HTRIGHT;

            if ((nY > 0) && (nY < m_nBorderWidth))
                *result = HTTOP;

            if ((nY > this->height() - m_nBorderWidth) && (nY < this->height()))
                *result = HTBOTTOM;

            if ((nX > 0) && (nX < m_nBorderWidth) && (nY > 0)
                && (nY < m_nBorderWidth))
                *result = HTTOPLEFT;

            if ((nX > this->width() - m_nBorderWidth) && (nX < this->width())
                && (nY > 0) && (nY < m_nBorderWidth))
                *result = HTTOPRIGHT;

            if ((nX > 0) && (nX < m_nBorderWidth)
                && (nY > this->height() - m_nBorderWidth) && (nY < this->height()))
                *result = HTBOTTOMLEFT;

            if ((nX > this->width() - m_nBorderWidth) && (nX < this->width())
                && (nY > this->height() - m_nBorderWidth) && (nY < this->height()))
                *result = HTBOTTOMRIGHT;

            return true;
        }
    }
    return QWidget::nativeEvent(eventType, message, result);
}

void Widget::mouseMoveEvent(QMouseEvent *e) {
    return QWidget::mouseMoveEvent(e);
}

void Widget::mousePressEvent(QMouseEvent *e) {
    setFocus();
    return QWidget::mousePressEvent(e);
}

void Widget::changFlagToTrueWithOtherFlagFalse(int index) {
    leftScrollAreaSelectedBtn_selected_flag[index] = true;
    for (int i = 0; i < leftScrollAreaSelectedBtn_count; ++i) {
        if (i != index) {
            leftScrollAreaSelectedBtn_selected_flag[i] = false;
        }
    }
}

void Widget::showSelectedBtn(bool *flag_arr) {
    // 除开下标为0、6、9、11的大的标题栏
    for (int i = 0; i < leftScrollAreaSelectedBtn_count; ++i) {
        if (i == 0 || i == 6 || i == 9 || i == 11) {
            continue;
        }
        // 窗口栏中的btnType是5
        leftScrollAreaSelectedBtn[i]->setSelectedBtn(5, flag_arr[i]);
    }
}
