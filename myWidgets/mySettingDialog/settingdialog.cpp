//
// Created by 93705 on 2022/5/27.
//

// You may need to build the project (run Qt uic code generator) to get "ui_settingDialog.h" resolved

#include "settingdialog.h"
#include "ui_settingDialog.h"
#include "myWidgets/myTitleBar/title_bar.h"
#include <QIcon>
#include <QVBoxLayout>
#include <QLabel>


settingDialog::settingDialog(QWidget *parent) :
        QWidget(parent), ui(new Ui::settingDialog) {
    ui->setupUi(this);

    setStyleSheet("background-color:rgb(50,50,50);color:white");
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowIcon(QIcon(":/res/myTitleBar_imgs/fileLogo.png"));
    setWindowTitle("FileSubSystem");
    setFixedSize(600, 400);

    auto *mainVLayout = new QVBoxLayout;

    auto *msg_version = new QLabel;
    auto *msg_creator = new QLabel;
    msg_version->setText("File SubSystem-version：1.0");
    msg_version->setFont(QFont("kaiTi", 16));

    msg_creator->setText("created by 计算机181 何雷 18416117");
    msg_creator->setFont(QFont("kaiTi", 16));

    mainVLayout->addSpacing(100);
    mainVLayout->addWidget(msg_version);
    mainVLayout->addSpacing(100);
    mainVLayout->addWidget(msg_creator);
    mainVLayout->addStretch();
    mainVLayout->setAlignment(Qt::AlignCenter);
    setLayout(mainVLayout);
}

settingDialog::~settingDialog() {
    delete ui;
}

