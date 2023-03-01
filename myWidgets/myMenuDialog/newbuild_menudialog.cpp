//
// Created by 93705 on 2022/5/12.
//

// You may need to build the project (run Qt uic code generator) to get "ui_newBuild_menuDialog.h" resolved

#include "newbuild_menudialog.h"
#include "ui_newBuild_menuDialog.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QDebug>
#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <QString>


newBuild_menuDialog::newBuild_menuDialog(const QString &absPath, short actionType, QWidget *parent) :
        QDialog(parent), ui(new Ui::newBuild_menuDialog) {
    ui->setupUi(this);

    setStyleSheet("QDialog{color:white;background-color:rgb(1, 21, 22)}");
    setFixedSize(450, 320);
    setWindowFlag(Qt::FramelessWindowHint);

    // 声明组件
    auto *mainVLayout = new QVBoxLayout;
    auto *bottomHLayoutWrapper = new QWidget;
    auto *bottomHLayout = new QHBoxLayout;
    auto *titleLabel = new QLabel;
    auto *inputLineEdit = new QLineEdit;
    auto *confirmSetName = new QPushButton(this);
    auto *cancel = new QPushButton(this);

    // 设置文本信息
    if (actionType == 7) {
        titleLabel->setText("重命名目标");
    } else {
        titleLabel->setText("键入名称");
    }
    titleLabel->setFont(QFont("heiTi", 20, 1));
    inputLineEdit->setPlaceholderText("输入名称");
    inputLineEdit->setFont(QFont("kaiTi", 14));
    confirmSetName->setText("确认");
    confirmSetName->setFont(QFont("kaiTi", 16));
    cancel->setText("取消");
    cancel->setFont(QFont("kaiTi", 16));

    // 设置样式

    confirmSetName->setFixedSize(180, 40);
    inputLineEdit->setFixedSize(400, 40);
    cancel->setFixedSize(180, 40);
    titleLabel->setStyleSheet("QLabel{color:white;padding-left:20px;}");
    bottomHLayoutWrapper->setStyleSheet("QWidget{background-color:rgb(33,33,33);}");
    inputLineEdit->setStyleSheet(
            "QLineEdit{margin-left:32px;color:white;border-radius:5px;background-color:rgb(35,35,35);border-left:none;border-top:none;border-right:none;border-bottom:1px solid gray;}"
            "QLineEdit:hover{border-radius:5px;border-left:none;border-top:none;border-right:none;border-bottom:2px solid rgb(117, 182, 233)}");
    cancel->setStyleSheet("QPushButton{color:white;border-radius:5px;background-color:rgb(45,45,45);}"
                          "QPushButton:hover{background-color:rgb(55,55,55);}");
    confirmSetName->setStyleSheet(
            "QPushButton{border-radius:5px;background-color:rgb(67,67,67);color:rgb(160,160,160);}");
    // 添加组件到布局中
    bottomHLayout->addWidget(confirmSetName);
    bottomHLayout->addWidget(cancel);
    bottomHLayoutWrapper->setLayout(bottomHLayout);

    mainVLayout->addWidget(titleLabel);
    mainVLayout->addWidget(inputLineEdit);
    mainVLayout->addSpacing(50);
    mainVLayout->addWidget(bottomHLayoutWrapper);
    mainVLayout->setMargin(0);

    setLayout(mainVLayout);
    // 初始时确认按钮不可用
    confirmSetName->setEnabled(false);
    // 绑定事件
    connect(inputLineEdit, &QLineEdit::textEdited, [=]() {
        if (inputLineEdit->text().length() >= 1) {
            confirmSetName->setEnabled(true);
            confirmSetName->setStyleSheet(
                    "QPushButton{border-radius:5px;background-color:rgb(109, 169, 216);color:black;}"
                    "QPushButton:hover{background-color:rgb(119, 179, 226);}");
        } else {
            confirmSetName->setEnabled(false);
            confirmSetName->setStyleSheet(
                    "QPushButton{color:white;border-radius:5px;background-color:rgb(67,67,67);color:rgb(160,160,160);}");
        }
    });
    connect(confirmSetName, &QPushButton::clicked, [=]() {
        QDir currentFolderPath(absPath);
        QDir newFolder(absPath + u8"/" + inputLineEdit->text());
        QFile newFile;
        newFile.setFileName(absPath + u8"/" + inputLineEdit->text());
        QFileInfo fileInfo(absPath);
        QStringList list = currentFolderPath.entryList(QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Files);
        int itemsCount = list.count();
        switch (actionType) {
            case 1:
                if (!newFolder.exists()) {
                    newFolder.mkdir(absPath + u8"/" + inputLineEdit->text());
                } else {
                    newFolder.mkdir(absPath + u8"/" + inputLineEdit->text() + QString("_%1").arg(itemsCount + 1));
                }
                break;
            case 2:
                if (!newFile.exists()) {
                    newFile.open(QIODevice::ReadWrite);
                } else {
                    newFile.setFileName(absPath + u8"/" + inputLineEdit->text() + QString("_%1").arg(itemsCount));
                    newFile.open(QIODevice::ReadWrite);
                }
                break;
            case 3:
                newFile.setFileName(absPath + u8"/" + inputLineEdit->text() + ".docx");
                if (!newFile.exists()) {
                    newFile.open(QIODevice::ReadWrite);
                } else {
                    newFile.setFileName(
                            absPath + u8"/" + inputLineEdit->text() + QString("_%1").arg(itemsCount) + ".docx");
                    newFile.open(QIODevice::ReadWrite);
                }
                break;
            case 4:
                newFile.setFileName(absPath + u8"/" + inputLineEdit->text() + ".txt");
                if (!newFile.exists()) {
                    newFile.open(QIODevice::ReadWrite);
                } else {
                    newFile.setFileName(
                            absPath + u8"/" + inputLineEdit->text() + QString("_%1").arg(itemsCount) + ".txt");
                    newFile.open(QIODevice::ReadWrite);
                }
                break;
            case 5:
                newFile.setFileName(absPath + u8"/" + inputLineEdit->text() + ".md");
                if (!newFile.exists()) {
                    newFile.open(QIODevice::ReadWrite);
                } else {
                    newFile.setFileName(
                            absPath + u8"/" + inputLineEdit->text() + QString("_%1").arg(itemsCount) + ".md");
                    newFile.open(QIODevice::ReadWrite);
                }
                break;
            case 6:
                break;
            case 7: {
                QStringList list = absPath.split("/");
                QString temp = absPath.chopped(list.at(list.count() - 1).length() + 1);
                if (fileInfo.isDir()) {
                    currentFolderPath.rename(absPath, temp + u8"/" + inputLineEdit->text());
                }
                if (fileInfo.isFile()) {
                    currentFolderPath.rename(absPath, temp + u8"/" + inputLineEdit->text() + "." + fileInfo.suffix());
                }
                break;
            }
            default:
                break;
        }
        close();
    });
    connect(cancel, &QPushButton::clicked, [=]() {
        close();
    });

}

newBuild_menuDialog::~newBuild_menuDialog() {
    delete ui;
}

