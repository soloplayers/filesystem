//
// Created by 93705 on 2022/5/9.
//

// You may need to build the project (run Qt uic code generator) to get "ui_login.h" resolved

#include "login.h"
#include "ui_login.h"
#include "myWidgets/myTitleBar/title_bar.h"
#include <QPixmap>
#include <QPainter>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QRegExpValidator>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlRecord>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QMessageBox>
#include <QRandomGenerator>
#include "QSqlQuery"
#include "widget.h"
#include <QStandardPaths>
#include <QFile>


login::login(QWidget *parent) :
        QWidget(parent), ui(new Ui::login) {
    ui->setupUi(this);

    // 使用随机数进行初始化
    randomNum_1 = QRandomGenerator::global()->bounded(100);
    randomNum_2 = QRandomGenerator::global()->bounded(100);

    // 连接数据库
    connectDataBase();

    auto *pTitleBar = new TitleBar(this);
    installEventFilter(pTitleBar);
    installEventFilter(this);
    setWindowTitle("文件子系统");
    // 设置无边框窗口，只填一个type类型，不然后面窗口无法进行放缩 + 设置鼠标追踪
    setMouseTracking(true);
    setWindowFlags(Qt::FramelessWindowHint);
    setWindowIcon(QPixmap(":/res/myTitleBar_imgs/fileLogo.png"));
    setMinimumSize(1280, 720);
    // 设置字体颜色
    setStyleSheet("QWidget{color:white}");
    // 设置窗口类型，与windows应用程序类型匹配
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinimizeButtonHint |
                   Qt::WindowMaximizeButtonHint);

    //进行标题栏界面布局
    auto *titleBarLayout = new QVBoxLayout;
    titleBarLayout->addWidget(pTitleBar);
    titleBarLayout->addStretch();
    titleBarLayout->setSpacing(0);
    titleBarLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(titleBarLayout);

    // 登录界面布局
    loginWrapper = new QWidget(this);
    loginWrapper->setFixedSize(740, 530);
    loginWrapper->setStyleSheet("QWidget{background-color:rgba(136, 133, 164,0.2)}"
                                "QWidget{background-color:transparent}");

    // 垂直布局
    auto *loginWrapper_VLayout = new QVBoxLayout;
    // 标题
    auto *loginWrapper_VLayout_title = new QLabel;
    loginWrapper_VLayout_title->setText("系统登录");
    loginWrapper_VLayout_title->setFont(QFont("kaiTi", 14));
    loginWrapper_VLayout_title->setAlignment(Qt::AlignCenter);
    // 用户名
    auto *userWrapper = new QWidget;
    auto *userWrapperHLayout = new QHBoxLayout;
    auto *userWrapperHLayout_leftIcon = new QLabel;
    userWrapperHLayout_rightInput = new QLineEdit;
    auto *userWrapperHLayout_tips = new QLabel;
    // 添加资源和文字信息和样式信息
    userWrapperHLayout_leftIcon->setPixmap(QPixmap(":/res/login_imgs/userIcon.png"));
    userWrapperHLayout_leftIcon->setFixedSize(34, 34);
    userWrapperHLayout_rightInput->setPlaceholderText("用户名(2-20个字符)");
    userWrapperHLayout_rightInput->setFixedSize(400, 50);
    userWrapperHLayout_rightInput->setFont(QFont("kaiTi", 12));
    userWrapperHLayout_rightInput->setStyleSheet("QLineEdit{border:none}"
                                                 "QLineEdit:hover{border-top:0px;border-bottom:1px solid rgb(237, 162, 0);border-left:0px;border-right:0px;}");
    userWrapperHLayout_tips->setText("由汉字、字母、数字组成");
    userWrapperHLayout_tips->setFixedHeight(50);
    userWrapperHLayout_tips->setFont(QFont("kaiTi", 12));
    userWrapperHLayout_tips->setStyleSheet("QLabel{background-color:rgba(150,150,150,0.2);color:rgb(255, 0, 0)}");
    userWrapperHLayout->addSpacing(20);
    userWrapperHLayout->addWidget(userWrapperHLayout_leftIcon);
    userWrapperHLayout->addWidget(userWrapperHLayout_rightInput);
    userWrapperHLayout->addWidget(userWrapperHLayout_tips);
    userWrapper->setLayout(userWrapperHLayout);
    // 限制输入的格式
    auto *val_user = new QRegExpValidator(QRegExp("[a-zA-Z0-9\u4e00-\u9fa5]{2,}$"));//正则表达式：大小写字母数字中文, 数量无上限
    userWrapperHLayout_rightInput->setValidator(val_user);
    userWrapperHLayout_rightInput->setMaxLength(20);


    // 密码
    auto *pwdWrapper = new QWidget;
    auto *pwdWrapperHLayout = new QHBoxLayout;
    auto *pwdWrapperHLayout_leftIcon = new QLabel;
    pwdWrapperHLayout_rightInput = new QLineEdit;
    auto *pwdWrapperHLayout_tips = new QLabel;
    // 添加资源和文字信息和样式
    pwdWrapperHLayout_leftIcon->setFixedSize(34, 34);
    pwdWrapperHLayout_leftIcon->setPixmap(QPixmap(":/res/login_imgs/passwordIcon.png"));
    pwdWrapperHLayout_rightInput->setEchoMode(QLineEdit::PasswordEchoOnEdit); // 输入的文本格式
    pwdWrapperHLayout_rightInput->setMaxLength(20);
    pwdWrapperHLayout_rightInput->setPlaceholderText("密码（8-20个字符）");
    pwdWrapperHLayout_rightInput->setFixedSize(400, 50);
    pwdWrapperHLayout_rightInput->setFont(QFont("kaiTi", 12));
    pwdWrapperHLayout_rightInput->setStyleSheet("QLineEdit{border:none}"
                                                "QLineEdit:hover{border-top:0px;border-bottom:1px solid rgb(237, 162, 0);border-left:0px;border-right:0px;}"
                                                "QLineEdit[echoMode='2']{lineedit-password-character:42}");
    pwdWrapperHLayout_tips->setText("由字母、数字、下划线组成");
    pwdWrapperHLayout_tips->setFont(QFont("kaiTi", 12));
    pwdWrapperHLayout_tips->setStyleSheet("QLabel{background-color:rgba(150,150,150,0.2);color:rgb(255, 0, 0)}");
    pwdWrapperHLayout->addSpacing(20);
    pwdWrapperHLayout->addWidget(pwdWrapperHLayout_leftIcon);
    pwdWrapperHLayout->addWidget(pwdWrapperHLayout_rightInput);
    pwdWrapperHLayout->addWidget(pwdWrapperHLayout_tips);
    pwdWrapper->setLayout(pwdWrapperHLayout);
    // 限制输入
    auto *val_pwd = new QRegExpValidator(QRegExp("^[A-Za-z0-9_]+${8,}"));//正则表达式：大小写字母数字中文, 数量无上限
    pwdWrapperHLayout_rightInput->setValidator(val_pwd);

    // 简单的验证
    auto *verificationWrapper = new QWidget;
    auto *verificationWrapperHLayout = new QHBoxLayout;
    auto *verificationWrapperHLayout_leftIcon = new QLabel;
    auto *verificationWrapperHLayout_rightInput = new QLineEdit;
    verificationWrapperHLayout_helper = new QPushButton;
    // 添加资源和文字信息和样式
    verificationWrapperHLayout_helper->setFixedSize(100, 50);
    verificationWrapperHLayout_leftIcon->setFixedSize(34, 34);
    verificationWrapperHLayout_leftIcon->setPixmap(QPixmap(":/res/login_imgs/verifyIcon.png"));
    verificationWrapperHLayout_rightInput->setPlaceholderText("计算结果(点击图片切换下一张)");
    verificationWrapperHLayout_rightInput->setFixedSize(400, 50);
    verificationWrapperHLayout_rightInput->setFont(QFont("kaiTi", 12));
    verificationWrapperHLayout_rightInput->setStyleSheet(
            "QLineEdit{border:none}"
            "QLineEdit:hover{border-top:0px;border-bottom:1px solid rgb(237, 162, 0);border-left:0px;border-right:0px;}"
    );
    verificationWrapperHLayout_helper->setFont(QFont("kaiTi", 12));
    verificationWrapperHLayout_helper->setStyleSheet(
            "QPushButton{color:rgb(255, 0, 0);background-color:rgba(150,150,150,0.2)}");
    verificationWrapperHLayout_helper->setText(QString("%1+%2=?").arg(randomNum_1).arg(randomNum_2));
    verificationWrapperHLayout->addSpacing(20);
    verificationWrapperHLayout->addWidget(verificationWrapperHLayout_leftIcon);
    verificationWrapperHLayout->addWidget(verificationWrapperHLayout_rightInput);
    verificationWrapperHLayout->addWidget(verificationWrapperHLayout_helper);
    verificationWrapperHLayout->addStretch();
    verificationWrapper->setLayout(verificationWrapperHLayout);
    // 限制输入
    verificationWrapperHLayout_rightInput->setValidator(new QRegExpValidator(QRegExp("^[0-9]*$")));

    //注册 登录按钮
    auto *btnWrapper = new QWidget;
    auto *btnWrapperHLayout = new QHBoxLayout;
    auto *btnWrapperHLayout_leftRegister = new QPushButton;
    auto *btnWrapperHLayout_rightLogin = new QPushButton;
    // 添加资源和文字信息和样式
    btnWrapperHLayout_leftRegister->setText("注册");
    btnWrapperHLayout_rightLogin->setText("登录");
    btnWrapperHLayout_leftRegister->setFont(QFont("kaiTi", 12));
    btnWrapperHLayout_leftRegister->setStyleSheet("QPushButton{border:1px solid rgb(102, 142, 190)}"
                                                  "QPushButton:pressed{border:1px solid rgb(50, 90, 138)}");
    btnWrapperHLayout_leftRegister->setFixedSize(200, 50);
    btnWrapperHLayout_rightLogin->setFont(QFont("kaiTi", 12));
    btnWrapperHLayout_rightLogin->setStyleSheet("QPushButton{border:1px solid rgb(102, 142, 190)}"
                                                "QPushButton:pressed{border:1px solid rgb(50, 90, 138)}");
    btnWrapperHLayout_rightLogin->setFixedSize(200, 50);
    btnWrapperHLayout->addWidget(btnWrapperHLayout_leftRegister);
    btnWrapperHLayout->addWidget(btnWrapperHLayout_rightLogin);
    btnWrapper->setLayout(btnWrapperHLayout);

    // 往垂直布局添加组件
    loginWrapper_VLayout->addWidget(loginWrapper_VLayout_title);
    loginWrapper_VLayout->addWidget(userWrapper);
    loginWrapper_VLayout->addWidget(pwdWrapper);
    loginWrapper_VLayout->addWidget(verificationWrapper);
    loginWrapper_VLayout->addWidget(btnWrapper);
    loginWrapper_VLayout->setMargin(0);
    // 给wrapper添加上布局
    loginWrapper->setLayout(loginWrapper_VLayout);

    // 为注册按钮绑定事件
    connect(btnWrapperHLayout_leftRegister, &QPushButton::clicked, [=]() {
        if (userWrapperHLayout_rightInput->text().length() < 2 || pwdWrapperHLayout_rightInput->text().length() < 8) {
            QMessageBox msg;
            msg.setWindowTitle("注册失败");
            msg.setWindowIcon(QPixmap(":/res/login_imgs/error.png"));
            msg.setStyleSheet("QLabel{min-width:300px;min-height:100px;text-align:center;}");
            msg.setText("用户名或密码长度太短！");
            msg.setFont(QFont("kaiTi", 14));
            int ret = msg.exec();
            if (ret == QMessageBox::Ok) {
                verificationWrapperHLayout_helper->clicked(true);
            }
        } else {
            if (verificationWrapperHLayout_rightInput->text().toInt() == (randomNum_1 + randomNum_2)) {
                dbOperate_register(userWrapperHLayout_rightInput->text(), pwdWrapperHLayout_rightInput->text());
            } else {
                QMessageBox msg;
                msg.setWindowTitle("验证出错");
                msg.setWindowIcon(QPixmap(":/res/login_imgs/error.png"));
                msg.setStyleSheet("QLabel{min-width:300px;min-height:100px;text-align:center;}");
                msg.setText("验证信息出错，请重新输入！");
                msg.setFont(QFont("kaiTi", 14));
                int ret = msg.exec();
                if (ret == QMessageBox::Ok) {
                    verificationWrapperHLayout_helper->clicked(true);
                }
            }
        }

    });
    // 为登录按钮绑定事件
    // 声明主要的窗口
    auto *mainWindow = new Widget;
    connect(btnWrapperHLayout_rightLogin, &QPushButton::clicked, [=]() {
        if (userWrapperHLayout_rightInput->text().length() < 2 || pwdWrapperHLayout_rightInput->text().length() < 8) {
            QMessageBox msg;
            msg.setWindowTitle("登录失败");
            msg.setWindowIcon(QPixmap(":/res/login_imgs/error.png"));
            msg.setStyleSheet("QLabel{min-width:300px;min-height:100px;text-align:center;}");
            msg.setText("用户名或密码长度太短！");
            msg.setFont(QFont("kaiTi", 14));
            int ret = msg.exec();
            if (ret == QMessageBox::Ok) {
                verificationWrapperHLayout_helper->clicked(true);
            }
        } else {
            if (verificationWrapperHLayout_rightInput->text().toInt() == randomNum_1 + randomNum_2) {
                if (dbOperate_login(userWrapperHLayout_rightInput->text(), pwdWrapperHLayout_rightInput->text())) {
                    this->hide();
                    mainWindow->show();
                }
            } else {
                QMessageBox msg;
                msg.setWindowTitle("验证出错");
                msg.setWindowIcon(QPixmap(":/res/login_imgs/error.png"));
                msg.setStyleSheet("QLabel{min-width:300px;min-height:100px;text-align:center;}");
                msg.setText("验证信息出错，请重新输入！");
                msg.setFont(QFont("kaiTi", 14));
                int ret = msg.exec();
                if (ret == QMessageBox::Ok) {
                    verificationWrapperHLayout_helper->clicked(true);
                }
            }
        }

    });
    // 绑定验证信息的QPushButton事件
    connect(verificationWrapperHLayout_helper, &QPushButton::clicked, [=]() {
        randomNum_1 = QRandomGenerator::global()->bounded(100);
        randomNum_2 = QRandomGenerator::global()->bounded(100);
        verificationWrapperHLayout_helper->setText(QString("%1+%2=?").arg(randomNum_1).arg(randomNum_2));
    });
}

login::~login() {
    delete ui;
}


void login::dbOperate_register(const QString &userName, const QString &pwd) {
    if (db.open()) {
        QSqlQuery query(db);
        query.exec("use FileSystemUsers");
        query.exec("select * from userMsg;");
        // 遍历数据库信息
        while (query.next()) {
            if (query.value("name") == userName) {
                QMessageBox msg;
                msg.setWindowTitle("注册失败");
                msg.setWindowIcon(QPixmap(":/res/login_imgs/error.png"));
                msg.setStyleSheet("QLabel{min-width:450px;min-height:150px;text-align:center;}");
                msg.setText("请更换一个用户名，该用户名已经被注册！");
                msg.setFont(QFont("kaiTi", 14));
                int ret = msg.exec();
                if (ret == QMessageBox::Ok) {
                    verificationWrapperHLayout_helper->clicked(true);
                }
                db.close();
                return;
            }
        }
        QSqlTableModel model;
        model.setTable("userMsg");
        QSqlRecord record = model.record();
        record.setValue("name", userName);
        record.setValue("pwd", pwd);
        if (model.insertRecord(-1, record)) {
            QMessageBox msg;
            msg.setWindowTitle("注册成功");
            msg.setWindowIcon(QPixmap(":/res/login_imgs/correct.png"));
            msg.setStyleSheet("QLabel{min-width:400px;min-height:150px;text-align:center;}");
            msg.setText("注册成功！请点击登录以进入系统");
            msg.setFont(QFont("kaiTi", 14));
            int ret = msg.exec();
            if (ret == QMessageBox::Ok) {
                verificationWrapperHLayout_helper->clicked(true);
            }
            QFile tempFile(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation) + u8"/" +
                           "FileSystem_账号密码.txt");
            // 直接覆盖重写账号密码
            tempFile.open(QIODevice::ReadWrite | QIODevice::Text);
            QString content = "用户名：" + userWrapperHLayout_rightInput->text() + "\n密码：" +
                              pwdWrapperHLayout_rightInput->text();
            QByteArray str = content.toUtf8();
            tempFile.write(str);
            QMessageBox msg_c;
            msg_c.setWindowTitle("提示信息");
            msg_c.setWindowIcon(QPixmap(":/res/commonImgs/warning.png"));
            msg_c.setStyleSheet("QLabel{min-width:400px;min-height:100px;text-align:center;}");
            msg_c.setText("用户名和密码已经存入到您的桌面！");
            msg_c.setFont(QFont("kaiTi", 14));
            msg_c.exec();
        }
        db.close();
        return;
    }
}

bool login::dbOperate_login(const QString &userName, const QString &pwd) {
    if (db.open()) {
        QSqlQuery query(db);
        query.exec("use FileSystemUsers");
        query.exec("select * from userMsg;");
        // 遍历数据库信息
        while (query.next()) {
            if (query.value("name") == userName && query.value("pwd") == pwd) {
                QMessageBox msg;
                msg.setWindowTitle("登录成功");
                msg.setWindowIcon(QPixmap(":/res/login_imgs/correct.png"));
                msg.setStyleSheet("QLabel{min-width:300px;min-height:100px;text-align:center;}");
                msg.setText("欢迎使用文件子系统！");
                msg.setFont(QFont("kaiTi", 14));
                msg.exec();
                db.close();
                return true;
            }
        }
        // 查询失败，数据库没有该信息
        QMessageBox msg;
        msg.setWindowTitle("登录失败");
        msg.setWindowIcon(QPixmap(":/res/login_imgs/error.png"));
        msg.setStyleSheet("QLabel{min-width:450px;min-height:150px;text-align:center;}");
        msg.setText("用户名不存在或密码不正确，请重新输入！");
        msg.setFont(QFont("kaiTi", 14));
        int ret = msg.exec();
        if (ret == QMessageBox::Ok) {
            verificationWrapperHLayout_helper->clicked(true);
        }
        db.close();
        return false;
    }
    return false;
}

void login::paintEvent(QPaintEvent *e) {
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/res/login_imgs/login_background_img.png");
    painter.drawPixmap(0, 0, this->width(), this->height(), pix);
}

bool login::eventFilter(QObject *obj, QEvent *e) {
    switch (e->type()) {
        case QEvent::Resize:
            loginWrapper->move((this->width() - loginWrapper->width()) / 2,
                               (this->height() - loginWrapper->height()) / 2);
            return true;
        default:
            return QWidget::eventFilter(obj, e);
    }
}

bool login::connectDataBase() {
    db = QSqlDatabase::addDatabase("QODBC");
    db.setHostName("127.0.0.1");
    db.setPort(3306);
    db.setDatabaseName("Qt_to_mysql");
    db.setUserName("root");
    db.setPassword("HL19990624.");
    if (db.open()) {
        return true;
    }
    return false;
}
