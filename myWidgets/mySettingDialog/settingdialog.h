//
// Created by 93705 on 2022/5/27.
//

#ifndef FILESYSTEM_SETTINGDIALOG_H
#define FILESYSTEM_SETTINGDIALOG_H

#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class settingDialog; }
QT_END_NAMESPACE

class settingDialog : public QWidget {
Q_OBJECT

public:
    explicit settingDialog(QWidget *parent = nullptr);

    ~settingDialog() override;

private:
    Ui::settingDialog *ui;
};


#endif //FILESYSTEM_SETTINGDIALOG_H
