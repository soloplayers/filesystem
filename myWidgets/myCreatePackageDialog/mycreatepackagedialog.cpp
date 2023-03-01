//
// Created by 93705 on 2022/5/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_myCreatePackageDialog.h" resolved

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include "mycreatepackagedialog.h"
#include "ui_myCreatePackageDialog.h"
#include <QDebug>


myCreatePackageDialog::myCreatePackageDialog(QWidget *parent) :
        QDialog(parent), ui(new Ui::myCreatePackageDialog) {
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
    auto *confirmSetName = new QPushButton;
    auto *cancel = new QPushButton;

    titleLabel->setText("输入包的名称");
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
        emit sendData(inputLineEdit->text(),0);
        close();
    });

    connect(cancel, &QPushButton::clicked, [=]() {
        emit sendData("", 1);
        close();
    });

}

myCreatePackageDialog::~myCreatePackageDialog() {
    delete ui;
}

