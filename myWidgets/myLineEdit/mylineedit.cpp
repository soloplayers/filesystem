//
// Created by 93705 on 2022/4/16.
//

// You may need to build the project (run Qt uic code generator) to get "ui_myLineEdit.h" resolved

#include "mylineedit.h"
#include "ui_myLineEdit.h"


myLineEdit::myLineEdit(QWidget *parent) :
        QLineEdit(parent), ui(new Ui::myLineEdit) {
    ui->setupUi(this);
    flag_doubleClicked = true;
}

myLineEdit::~myLineEdit() {
    delete ui;
}

void myLineEdit::mousePressEvent(QMouseEvent *e) {
    if (flag_doubleClicked) {
        emit myLineEdit::mouseDoubleClickEvent(e);
        flag_doubleClicked = false;
    }
    setTextMargins(5, 0, 0, 0);
    setStyleSheet(
            "border-top:0px solid;"
            "border-bottom:3px  solid rgb(84, 132, 169);"
            "border-left:0px  solid;"
            "border-right: 0px solid;"
            "color:white;"
            "font-size:18px;"
            "border-radius: 3px;"
            "background-color:rgb(45, 45, 45)"
    );
    return QLineEdit::mousePressEvent(e);
}

void myLineEdit::focusOutEvent(QFocusEvent *e) {
    flag_doubleClicked = true;
    setStyleSheet(
            "border-top:0px solid;"
            "border-bottom:1px  solid rgb(154, 154, 154);"
            "border-left:0px  solid;"
            "border-right: 0px solid;"
            "color:white;"
            "font-size:18px;"
            "border-radius: 3px;"
            "background-color:rgb(45, 45, 45)"
    );
    return QLineEdit::focusOutEvent(e);
}

