//
// Created by 93705 on 2022/5/14.
//

// You may need to build the project (run Qt uic code generator) to get "ui_itemPropertyDialog.h" resolved

#include <QFileInfo>
#include <QDir>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include "itempropertydialog.h"
#include "ui_itemPropertyDialog.h"
#include <QPixmap>
#include <QDateTime>
#include <QLabel>
#include <QFileIconProvider>
#include <QtMath>


itemPropertyDialog::itemPropertyDialog(QString absolutePath, QWidget *parent) :
        QDialog(parent), ui(new Ui::itemPropertyDialog) {
    ui->setupUi(this);
    QFileInfo tempInfo = QFileInfo(absolutePath);
    QFileIconProvider provider;
    QIcon icon = provider.icon(tempInfo);
    QPixmap IconToPix = icon.pixmap(QSize(48, 48));

    setFixedSize(650, 600);
    setWindowTitle(tempInfo.fileName() + " 属性");
    setWindowIcon(icon);
    setFont(QFont("kaiTi", 12));


    // 设置布局及其组件
    auto *mainVLayout = new QVBoxLayout;

    auto *firstWidget = new QWidget;
    auto *secondWidget = new QWidget;
    auto *thirdWidget = new QWidget;
    auto *fourthWidget = new QWidget;
    auto *fifthWidget = new QWidget;
    auto *sixthWidget = new QWidget;
    auto *seventhWidget = new QWidget;
    auto *eighthWidget = new QWidget;

    auto *firstWidgetHLayout = new QHBoxLayout;
    auto *secondWidgetHLayout = new QHBoxLayout;
    auto *thirdWidgetHLayout = new QHBoxLayout;
    auto *fourthWidgetHLayout = new QHBoxLayout;
    auto *fifthWidgetHLayout = new QHBoxLayout;
    auto *sixthWidgetHLayout = new QHBoxLayout;
    auto *seventhWidgetHLayout = new QHBoxLayout;

    auto *firstWidgetHLayout_leftLabel = new QLabel;
    auto *firstWidgetHLayout_rightLabel = new QLabel;
    auto *secondWidgetHLayout_leftLabel = new QLabel;
    auto *secondWidgetHLayout_rightLabel = new QLabel;
    auto *thirdWidgetHLayout_leftLabel = new QLabel;
    auto *thirdWidgetHLayout_rightLabel = new QLabel;
    auto *fourthWidgetHLayout_leftLabel = new QLabel;
    auto *fourthWidgetHLayout_rightLabel = new QLabel;
    auto *fifthWidgetHLayout_leftLabel = new QLabel;
    auto *fifthWidgetHLayout_rightLabel = new QLabel;
    auto *sixthWidgetHLayout_leftLabel = new QLabel;
    auto *sixthWidgetHLayout_rightLabel = new QLabel;
    auto *seventhWidgetHLayout_leftLabel = new QLabel;
    auto *seventhWidgetHLayout_rightLabel = new QLabel;

    // 设置样式及信息
    firstWidgetHLayout_leftLabel->setPixmap(IconToPix);
    firstWidgetHLayout_leftLabel->setFixedSize(100, 56);
    firstWidgetHLayout_rightLabel->setText(tempInfo.fileName());
    firstWidgetHLayout_rightLabel->setFont(QFont("kaiTi", 12));
    firstWidgetHLayout_rightLabel->setStyleSheet("QLabel{border:1px solid black;}");
    firstWidgetHLayout_rightLabel->setFixedHeight(35);
    // 设置QLabel内容复制
    firstWidgetHLayout_leftLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    firstWidgetHLayout_rightLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);

    secondWidgetHLayout_leftLabel->setText("类型：");
    secondWidgetHLayout_leftLabel->setFont(QFont("kaiTi", 12, QFont::Bold));
    if (tempInfo.isDir()) {
        secondWidgetHLayout_rightLabel->setText("文件夹");
    } else if (tempInfo.isFile()) {
        secondWidgetHLayout_rightLabel->setText("文件");
    }
    secondWidgetHLayout_leftLabel->setFixedSize(100, 25);
    secondWidgetHLayout_rightLabel->setFixedHeight(35);
    // 设置QLabel内容复制
    secondWidgetHLayout_leftLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    secondWidgetHLayout_rightLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);

    thirdWidgetHLayout_leftLabel->setText("位置：");
    thirdWidgetHLayout_leftLabel->setFont(QFont("kaiTi", 12, QFont::Bold));

    thirdWidgetHLayout_rightLabel->setText(tempInfo.filePath().chopped(tempInfo.fileName().length()));
    thirdWidgetHLayout_leftLabel->setFixedSize(100, 25);
    thirdWidgetHLayout_rightLabel->setFixedHeight(35);
    // 设置QLabel内容复制
    thirdWidgetHLayout_leftLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    thirdWidgetHLayout_rightLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);

//    --------------------------------
    fourthWidgetHLayout_leftLabel->setText("大小：");
    fourthWidgetHLayout_leftLabel->setFont(QFont("kaiTi", 12, QFont::Bold));

    quint64 totalSize, tempSize;
    QString units[6] = {"B", "KB", "MB", "GB", "TB"};
    int units_index = 0;
    if (tempInfo.isDir()) {
        totalSize = getCurrentDirSize(absolutePath);
    } else if (tempInfo.isFile()) {
        totalSize = tempInfo.size();
    }
    tempSize = totalSize;
    for (int i = 0; i < 7; i++) {
        if (0 < tempSize / pow(2, 10 * i) && tempSize / pow(2, 10 * i) < 1024) {
            units_index = i;
            break;
        }
    }
    fourthWidgetHLayout_rightLabel->setText(
            QString::number((int) (tempSize / pow(2, 10 * units_index))) + " " + units[units_index] +
            QString("(%1字节)").arg(totalSize));
    fourthWidgetHLayout_leftLabel->setFixedSize(100, 25);
    fourthWidgetHLayout_rightLabel->setFixedHeight(35);
    // 设置QLabel内容复制
    fourthWidgetHLayout_leftLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    fourthWidgetHLayout_rightLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);

    fifthWidgetHLayout_leftLabel->setText("创建时间：");
    fifthWidgetHLayout_leftLabel->setFont(QFont("kaiTi", 12, QFont::Bold));
    fifthWidgetHLayout_rightLabel->setText(tempInfo.birthTime().toString("yyyy-MM-dd hh:mm:ss"));
    fifthWidgetHLayout_leftLabel->setFixedSize(100, 25);
    fifthWidgetHLayout_rightLabel->setFixedHeight(35);
    // 设置QLabel内容复制
    fifthWidgetHLayout_leftLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    fifthWidgetHLayout_rightLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);

    sixthWidgetHLayout_leftLabel->setText("修改时间：");
    sixthWidgetHLayout_leftLabel->setFont(QFont("kaiTi", 12, QFont::Bold));
    sixthWidgetHLayout_rightLabel->setText(tempInfo.lastModified().toString("yyyy-MM-dd hh:mm:ss"));
    sixthWidgetHLayout_leftLabel->setFixedSize(100, 25);
    sixthWidgetHLayout_rightLabel->setFixedHeight(35);
    // 设置QLabel内容复制
    sixthWidgetHLayout_leftLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    sixthWidgetHLayout_rightLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);

    seventhWidgetHLayout_leftLabel->setText("访问时间：");
    seventhWidgetHLayout_leftLabel->setFont(QFont("kaiTi", 12, QFont::Bold));
    seventhWidgetHLayout_rightLabel->setText(tempInfo.lastRead().toString("yyyy-MM-dd hh:mm:ss"));
    seventhWidgetHLayout_leftLabel->setFixedSize(100, 25);
    seventhWidgetHLayout_rightLabel->setFixedHeight(35);
    // 设置QLabel内容复制
    seventhWidgetHLayout_leftLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    seventhWidgetHLayout_rightLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);

//    -----------------------------------

    // 设置布局以及添加组件到布局中
    firstWidgetHLayout->addWidget(firstWidgetHLayout_leftLabel);
    firstWidgetHLayout->addWidget(firstWidgetHLayout_rightLabel);
    secondWidgetHLayout->addWidget(secondWidgetHLayout_leftLabel);
    secondWidgetHLayout->addWidget(secondWidgetHLayout_rightLabel);
    thirdWidgetHLayout->addWidget(thirdWidgetHLayout_leftLabel);
    thirdWidgetHLayout->addWidget(thirdWidgetHLayout_rightLabel);
    fourthWidgetHLayout->addWidget(fourthWidgetHLayout_leftLabel);
    fourthWidgetHLayout->addWidget(fourthWidgetHLayout_rightLabel);
    fifthWidgetHLayout->addWidget(fifthWidgetHLayout_leftLabel);
    fifthWidgetHLayout->addWidget(fifthWidgetHLayout_rightLabel);
    sixthWidgetHLayout->addWidget(sixthWidgetHLayout_leftLabel);
    sixthWidgetHLayout->addWidget(sixthWidgetHLayout_rightLabel);
    seventhWidgetHLayout->addWidget(seventhWidgetHLayout_leftLabel);
    seventhWidgetHLayout->addWidget(seventhWidgetHLayout_rightLabel);

    firstWidget->setLayout(firstWidgetHLayout);
    secondWidget->setLayout(secondWidgetHLayout);
    thirdWidget->setLayout(thirdWidgetHLayout);
    fourthWidget->setLayout(fourthWidgetHLayout);
    fifthWidget->setLayout(fifthWidgetHLayout);
    sixthWidget->setLayout(sixthWidgetHLayout);
    seventhWidget->setLayout(seventhWidgetHLayout);

    mainVLayout->addWidget(firstWidget);
    mainVLayout->addWidget(secondWidget);
    mainVLayout->addWidget(thirdWidget);
    mainVLayout->addWidget(fourthWidget);
    mainVLayout->addWidget(fifthWidget);
    mainVLayout->addWidget(sixthWidget);
    mainVLayout->addWidget(seventhWidget);
    mainVLayout->addStretch();

    // 将整个布局添加到Dialog中
    setLayout(mainVLayout);


}

itemPropertyDialog::~itemPropertyDialog() {
    delete ui;
}

quint64 itemPropertyDialog::getCurrentDirSize(QString dirPath) {
    QString srcDir = QDir(dirPath).canonicalPath();
    QDir tmpDir(srcDir);
    quint64 size = 0;
    /*获取文件列表  统计文件大小*/
            foreach(QFileInfo fileInfo, tmpDir.entryInfoList(QDir::Files)) {
            size += fileInfo.size();
        }
    /*获取文件夹  并且过滤掉.和..文件夹 统计各个文件夹的文件大小 */
            foreach(QString subDir, tmpDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
            size += getCurrentDirSize(srcDir + QDir::separator() + subDir); //递归进行  统计所有子目录
        }
    return size;
}

