//
// Created by 93705 on 2022/4/10.
//

// You may need to build the project (run Qt uic code generator) to get "ui_myPushButton.h" resolved

#include <QTimer>
#include "mypushbutton.h"
#include "ui_myPushButton.h"
#include "QDebug"
#include <QEvent>
#include <QMouseEvent>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QProgressBar>
#include <QPixmap>
#include <QFileIconProvider>
#include <QDateTime>
#include <QMenu>
#include <QAction>

myPushButton::myPushButton(int btnType, const QString &btn_text, const QString &img_path, double value, double maximum,
                           QWidget *parent)
        : QPushButton(parent), ui(new Ui::myPushButton) {
    ui->setupUi(this);
    numBtnType = btnType;
    filePath = img_path;
    text = btn_text;
    //isSelected = false;
    valueSize = value;
    maximumSize = maximum;

    setMouseTracking(true);

    switch (numBtnType) {
        case 1:
            setStyleSheet("QPushButton{background-color:transparent;color:black;}"
                          "QPushButton:hover{background-color:red};color:black;");
            break;
        case 2:
            setStyleSheet("QPushButton{background-color:transparent;color:black;}"
                          "QPushButton:hover{background-color:rgb(57,57,57);color:black;}");
            break;
        case 3:
            if (text != "") {
                setText(text);
            }
            setStyleSheet("background-color:transparent;");
            break;
        case 4:
            btnHLayout = new QHBoxLayout;
            firstPic = new QLabel;
            firstBtnText = new QLabel;
            secondPic = new QLabel;
            firstPic->setPixmap(QPixmap(filePath));
            firstPic->setFixedSize(32, 32);

            // 指定固定的向上箭头
            secondPic->setPixmap(QPixmap(":res/commonImgs/arrow-up.png"));
            secondPic->setFixedSize(32, 32);
            firstBtnText->setFixedHeight(32);
            if (text != "") {
                firstBtnText->setText(text);
                firstBtnText->setFont(QFont("kaiTi", 12));
            }
            btnHLayout->addWidget(firstPic);
            btnHLayout->addSpacing(5);
            btnHLayout->addWidget(firstBtnText);
            firstBtnText->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
            btnHLayout->addWidget(secondPic);
            btnHLayout->setAlignment(Qt::AlignVCenter);
            firstPic->setStyleSheet("background-color:transparent;");
            firstBtnText->setStyleSheet("background-color:transparent;");
            secondPic->setStyleSheet("background-color:transparent;");
            setStyleSheet("QPushButton{background-color:transparent;color:white;}"
                          "QPushButton:hover{background-color:rgb(57,57,57);border-radius:10px}"
                          "QPushButton:pressed{background-color:rgb(47,47,47);border-radius:10px}");
            setLayout(btnHLayout);
            break;
        case 5:
            btnHLayout = new QHBoxLayout;
            firstPic = new QLabel;
            firstBtnText = new QLabel;
            secondPic = new QLabel;
            // 指定固定的竖直图案为空
            firstPic->setPixmap(QPixmap(""));
            firstPic->setFixedSize(32, 32);
            secondPic->setPixmap(QPixmap(filePath));
            secondPic->setFixedSize(32, 32);
            firstBtnText->setFixedHeight(32);
            if (text != "") {
                firstBtnText->setText(text);
                firstBtnText->setFont(QFont("kaiTi", 12));
            }
            btnHLayout->addWidget(firstPic);
            btnHLayout->addSpacing(10);
            btnHLayout->addWidget(secondPic);
            btnHLayout->addWidget(firstBtnText);
            firstBtnText->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
            btnHLayout->setAlignment(Qt::AlignVCenter);
            setLayout(btnHLayout);
            setStyleSheet("QPushButton{background-color:transparent;}"
                          "QPushButton:hover{background-color:rgb(57,57,57);border-radius:10px}"
                          "QPushButton:pressed{background-color:rgb(47,47,47);border-radius:10px}");
            firstPic->setStyleSheet("background-color:transparent;");
            firstBtnText->setStyleSheet("background-color:transparent;");
            secondPic->setStyleSheet("background-color:transparent;");
            break;
        case 6:
            setToolTip(text);
            setStyleSheet("QPushButton{background-color:transparent;color:black;}"
                          "QPushButton:hover{background-color:rgb(57,57,57);border-radius:10px;color:black;}"
                          "QPushButton:pressed{background-color:rgb(47,47,47);border-radius:10px;color:black;}"
                          "QPushButton::menu-indicator{image:none;}");
            setIcon(QIcon(filePath));
            break;
        case 7:
            btnHLayout = new QHBoxLayout;
            firstPic = new QLabel;
            firstBtnText = new QLabel;
            firstPic->setPixmap(QPixmap(filePath));
            firstPic->setFixedSize(32, 32);
            if (text != "") {
                firstBtnText->setText(text);
                firstBtnText->setFont(QFont("kaiTi", 12));
            }
            firstPic->setStyleSheet("QLabel{background-color:transparent}");
            firstBtnText->setStyleSheet("QLabel{background-color:transparent}");
            setStyleSheet("QPushButton{background-color:transparent;color:white;}"
                          "QPushButton:hover{background-color:rgb(57,57,57);border-radius:10px}"
                          "QPushButton:pressed{background-color:rgb(47,47,47);border-radius:10px}"
                          "QPushButton::menu-indicator{image:none;}");
            btnHLayout->setAlignment(Qt::AlignVCenter);
            btnHLayout->addWidget(firstPic);
            btnHLayout->addWidget(firstBtnText);
            btnHLayout->setContentsMargins(5, 0, 5, 0);
            setLayout(btnHLayout);
            break;
        case 8:
            btnHLayout = new QHBoxLayout;
            firstBtnText = new QLabel;
            secondPic = new QLabel;
            secondPic->setPixmap(QPixmap(filePath));
            secondPic->setFixedSize(32, 32);
            if (text != "") {
                firstBtnText->setText(text);
                firstBtnText->setFont(QFont("kaiTi", 12));
            }
            firstBtnText->setStyleSheet("QLabel{background-color:transparent}");
            secondPic->setStyleSheet("QLabel{background-color:transparent}");
            setStyleSheet("QPushButton{background-color:transparent;color:white;}"
                          "QPushButton:hover{background-color:rgb(57,57,57);border-radius:10px}"
                          "QPushButton:pressed{background-color:rgb(47,47,47);border-radius:10px}");
            btnHLayout->setAlignment(Qt::AlignCenter);
            btnHLayout->addSpacing(20);
            btnHLayout->addWidget(firstBtnText);
            btnHLayout->addStretch();
            btnHLayout->addWidget(secondPic);
            btnHLayout->setContentsMargins(5, 0, 5, 0);
            setLayout(btnHLayout);
            break;
        case 9:
            btnVLayout = new QVBoxLayout;
            firstPic = new QLabel;
            firstBtnText = new QLabel;
            firstPic->setPixmap(QPixmap(filePath));
            firstPic->setFixedSize(50, 50);
            if (text != "") {
                firstBtnText->setText(text);
                firstBtnText->setFont(QFont("kaiTi", 12));
            }
            firstPic->setStyleSheet("background-color:transparent;");
            firstBtnText->setStyleSheet("background-color:transparent;");
            setStyleSheet("QPushButton{background-color:rgb(52, 52, 52);color:white;border-radius:10px;}"
                          "QPushButton:hover{background-color:rgb(60, 60, 60)}"
                          "QPushButton:pressed{background-color:rgb(47, 47, 47)}");
            btnVLayout->setAlignment(Qt::AlignCenter);
            btnVLayout->addStretch();
            btnVLayout->addWidget(firstPic);
            btnVLayout->addWidget(firstBtnText);
            btnVLayout->addStretch();
            btnVLayout->setContentsMargins(5, 0, 5, 0);
            firstPic->setAttribute(Qt::WA_TransparentForMouseEvents);
            firstBtnText->setAttribute(Qt::WA_TransparentForMouseEvents);
            setLayout(btnVLayout);
            break;
        case 10:
            setStyleSheet("QPushButton{background-color:rgb(52, 52, 52);color:white;border-radius:10px;}"
                          "QPushButton:hover{background-color:rgb(57, 57, 57)}"
                          "QPushButton:pressed{background-color:rgb(47, 47, 47)}");
            setIcon(QIcon(filePath));
            setIconSize(QSize(48, 48));
            setFixedSize(60, 60);
            break;
        case 11: {
            btnHLayout = new QHBoxLayout;
            auto *process = new QProgressBar;
            auto *btnHLayout_rightWidget = new QWidget;
            btnVLayout = new QVBoxLayout;
            firstPic = new QLabel;
            firstBtnText = new QLabel;
            secondBtnText = new QLabel;
            firstPic->setPixmap(QPixmap(filePath));
            if (text != "") {
                firstBtnText->setText(text);
                firstBtnText->setFont(QFont("kaiTi", 12));
            }
            secondBtnText->setText(QString("%1GB可用,共%2GB").arg(valueSize).arg(maximumSize));
            secondBtnText->setFont(QFont("黑体", 10));
            process->setFixedHeight(15);
            QFile qssFile(":/styleSheets/progressBar.qss");
            qssFile.open(QIODevice::ReadOnly);
            QByteArray qssFileContent = qssFile.readAll();
            process->setOrientation(Qt::Horizontal);
            process->setMaximum(maximumSize);
            process->setMinimum(0);
            process->setValue(maximumSize - valueSize);
            process->setStyleSheet(qssFileContent);
            // 添加布局
            btnVLayout->addWidget(firstBtnText);
            btnVLayout->addWidget(process);
            btnVLayout->addWidget(secondBtnText);
            btnHLayout_rightWidget->setLayout(btnVLayout);
            firstPic->setStyleSheet("QLabel{background-color:transparent}");
            firstBtnText->setStyleSheet("QLabel{background-color:transparent}");
            secondBtnText->setStyleSheet("QLabel{background-color:transparent}");
            btnHLayout_rightWidget->setStyleSheet("QWidget{background-color:transparent}");
            setStyleSheet("QPushButton{background-color:rgb(52, 52, 52);color:white;border-radius:10px;}"
                          "QPushButton:hover{background-color:rgb(57, 57, 57)}"
                          "QPushButton:pressed{background-color:rgb(47, 47, 47)}");
            btnHLayout->addWidget(firstPic);
            btnHLayout->addWidget(btnHLayout_rightWidget);
            btnHLayout->setStretchFactor(firstPic, 1);
            btnHLayout->setStretchFactor(btnHLayout_rightWidget, 3);
            setLayout(btnHLayout);
            break;
        }
        case 12:
            setStyleSheet("QPushButton{background-color:rgb(64, 64, 64);color:white;border-radius:30px;}"
                          "QPushButton:hover{background-color:rgb(70, 70, 70);border-radius:30px;}"
                          "QPushButton:pressed{background-color:rgb(60, 60, 60);border-radius:30px;}");
            setIcon(QIcon(filePath));
            setIconSize(QSize(48, 48));
            setFixedSize(60, 60);
            break;
        case 13: {
            QFileInfo info = QFileInfo(text);
            QFileIconProvider provider;
            QIcon icon = provider.icon(info);
            QPixmap IconToPix = icon.pixmap(QSize(48, 48));
            btnHLayout = new QHBoxLayout;
            firstPic = new QLabel;
            firstBtnText = new QLabel;
            secondBtnText = new QLabel;
            firstPic->setPixmap(IconToPix);
            firstPic->setFixedSize(48, 48);
            if (text != "") {
                firstBtnText->setText(info.fileName());
                firstBtnText->setFont(QFont("kaiTi", 12));
            }
            secondBtnText->setText("最后访问的时间：" + info.lastModified().toString("yyyy-MM-dd hh:mm:ss"));
            secondBtnText->setFont(QFont("kaiTi", 12));
            firstPic->setStyleSheet("QLabel{background-color:transparent}");
            firstBtnText->setStyleSheet("QLabel{background-color:transparent}");
            secondBtnText->setStyleSheet("QLabel{background-color:transparent}");
            setStyleSheet("QPushButton{background-color:transparent;color:white;}"
                          "QPushButton:hover{background-color:rgb(57,57,57);border-radius:10px}"
                          "QPushButton:pressed{background-color:rgb(47,47,47);border-radius:10px}"
                          "QPushButton::menu-indicator{image:none;}");
            btnHLayout->setAlignment(Qt::AlignCenter);
            btnHLayout->addWidget(firstPic);
            btnHLayout->addWidget(firstBtnText);
            btnHLayout->addStretch();
            btnHLayout->addWidget(secondBtnText);
            btnHLayout->addSpacing(50);
            btnHLayout->setContentsMargins(5, 0, 5, 0);
            setLayout(btnHLayout);
            break;
        }
        case 14:
            // 存放的文件或文件夹按钮的菜单
            packageWrapperStoredWidgetGridLayoutItemMenu = new QMenu;
            packageWrapperStoredWidgetGridLayoutItemMenu_open = new QAction(QPixmap(":/res/commonImgs/openProject.png"),
                                                                            "打开项目位置");
            packageWrapperStoredWidgetGridLayoutItemMenu_remove = new QAction(
                    QPixmap(":/res/commonImgs/removePackage.png"),
                    "从包中移除");
            packageWrapperStoredWidgetGridLayoutItemMenu->setStyleSheet(
                    "QMenu{background-color:rgb(60,60,60);color:white;border-radius:3px;padding-top:4px;}"
                    "QMenu::item{border-radius:3px;height:38px;min-width:1px;margin:5px 10px;}"
                    "QMenu::item:selected{background-color:rgb(65,65,65);}");
            packageWrapperStoredWidgetGridLayoutItemMenu->addAction(packageWrapperStoredWidgetGridLayoutItemMenu_open);
            packageWrapperStoredWidgetGridLayoutItemMenu->addAction(
                    packageWrapperStoredWidgetGridLayoutItemMenu_remove);

            // 绑定触发信号事件
            connect(packageWrapperStoredWidgetGridLayoutItemMenu_open, &QAction::triggered, [=]() {
                emit openProjetLocation();
            });
            connect(packageWrapperStoredWidgetGridLayoutItemMenu_remove, &QAction::triggered, [=]() {
                emit removeProject();
            });

            QFileIconProvider provider;
            QFileInfo tempInfo(text);
            QIcon icon = provider.icon(tempInfo);
            QPixmap IconToPix = icon.pixmap(QSize(20, 20));
            btnHLayout = new QHBoxLayout;
            firstPic = new QLabel;
            firstBtnText = new QLabel;
            firstPic->setPixmap(IconToPix);
            firstPic->setFixedSize(24, 24);
            firstBtnText->setText(tempInfo.fileName());
            firstBtnText->setFont(QFont("kaiTi", 10));
            firstPic->setStyleSheet("QLabel{background-color:transparent}");
            firstBtnText->setStyleSheet("QLabel{background-color:transparent;}");
            setStyleSheet("QPushButton{background-color:transparent;color:white;}"
                          "QPushButton:hover{background-color:rgb(57,57,57);border-radius:10px}"
                          "QPushButton:pressed{background-color:rgb(47,47,47);border-radius:10px}"
                          "QPushButton::menu-indicator{image:none;}");
            btnHLayout->setAlignment(Qt::AlignVCenter);
            btnHLayout->addWidget(firstPic);
            btnHLayout->addWidget(firstBtnText);
            btnHLayout->setContentsMargins(5, 0, 5, 0);
            setLayout(btnHLayout);
            setFixedHeight(40);
            setToolTip(tempInfo.fileName());
            setContextMenuPolicy(Qt::CustomContextMenu);
            connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showMenu(const QPoint&)));
            break;
    }
}


myPushButton::~myPushButton() {
    delete ui;
}

void myPushButton::setSelectedBtn(int btnType, bool show_Btn) {
    if (show_Btn) {
        // 因为btnType4和8的实现逻辑是一样的，故可直接一起处理
        if (btnType == 4 || btnType == 8) {
            secondPic->setPixmap(QPixmap(":/res/commonImgs/arrow-up.png"));
        }
        if (btnType == 5) {
            firstPic->setPixmap(QPixmap(":res/leftScrollArea_imgs/verticalLine.png"));
            //isSelected = true;
        }
    } else {
        if (btnType == 4 || btnType == 8) {
            secondPic->setPixmap(QPixmap(":/res/commonImgs/arrow-down.png"));
        }
        if (btnType == 5) {
            firstPic->setPixmap(QPixmap(""));
            //isSelected = false;
        }
    }
}

void myPushButton::showMenu(const QPoint &point) {
    packageWrapperStoredWidgetGridLayoutItemMenu->exec(mapToGlobal(point));
}


