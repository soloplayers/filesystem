//
// Created by 93705 on 2022/4/16.
//

#ifndef FILESYSTEM_MYLINEEDIT_H
#define FILESYSTEM_MYLINEEDIT_H

#include <QLineEdit>


QT_BEGIN_NAMESPACE
namespace Ui { class myLineEdit; }
QT_END_NAMESPACE

class myLineEdit : public QLineEdit {
Q_OBJECT

public:
    explicit myLineEdit(QWidget *parent = nullptr);

    // 重写鼠标按下事件（单击）
    void mousePressEvent(QMouseEvent *) override;

    // LineEdit获得焦点和失去焦点的响应事件
    void focusOutEvent(QFocusEvent *) override;

    ~myLineEdit() override;

signals:


private:
    bool flag_doubleClicked;
    Ui::myLineEdit *ui;
};


#endif //FILESYSTEM_MYLINEEDIT_H
