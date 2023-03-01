//
// Created by 93705 on 2022/5/14.
//

#ifndef FILESYSTEM_ITEMPROPERTYDIALOG_H
#define FILESYSTEM_ITEMPROPERTYDIALOG_H

#include <QDialog>


QT_BEGIN_NAMESPACE
namespace Ui { class itemPropertyDialog; }
QT_END_NAMESPACE

class itemPropertyDialog : public QDialog {
Q_OBJECT

public:
    explicit itemPropertyDialog(QString absolutePath = "", QWidget *parent = nullptr);

    ~itemPropertyDialog() override;

private:
    Ui::itemPropertyDialog *ui;
    // 统计某个文件夹的大小
    quint64 getCurrentDirSize(QString dirPath);
};


#endif //FILESYSTEM_ITEMPROPERTYDIALOG_H
