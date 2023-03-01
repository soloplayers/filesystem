//
// Created by 93705 on 2022/5/9.
//

#ifndef FILESYSTEM_LOGIN_H
#define FILESYSTEM_LOGIN_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QLineEdit>
#include <QPushButton>


QT_BEGIN_NAMESPACE
namespace Ui { class login; }
QT_END_NAMESPACE

class login : public QWidget {
Q_OBJECT

public:
    explicit login(QWidget *parent = nullptr);

    // 重写绘图事件制作背景图
    void paintEvent(QPaintEvent *) override;

    // 重写事件过滤器
    bool eventFilter(QObject *, QEvent *) override;

    ~login() override;

private:
    Ui::login *ui;

    QWidget *loginWrapper;
    QSqlDatabase db;
    int randomNum_1;
    int randomNum_2;
    QLineEdit *userWrapperHLayout_rightInput;
    QLineEdit *pwdWrapperHLayout_rightInput;
    QPushButton *verificationWrapperHLayout_helper;

    // 连接数据库
    bool connectDataBase();

    // 连接数据库进行登录验证
    bool dbOperate_login(const QString &userName, const QString &pwd);

    // 连接数据库进行注册
    void dbOperate_register(const QString &userName, const QString &pwd);
};


#endif //FILESYSTEM_LOGIN_H
