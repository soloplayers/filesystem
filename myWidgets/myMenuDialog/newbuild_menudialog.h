//
// Created by 93705 on 2022/5/12.
//

#ifndef FILESYSTEM_NEWBUILD_MENUDIALOG_H
#define FILESYSTEM_NEWBUILD_MENUDIALOG_H

#include <QDialog>


QT_BEGIN_NAMESPACE
namespace Ui { class newBuild_menuDialog; }
QT_END_NAMESPACE

class newBuild_menuDialog : public QDialog {
Q_OBJECT
signals:

    void sendData(QString data);

public:
    /*
     * actionType 1--5是新建菜单栏
     * 1：创建文件夹
     * 2：创建文件
     * 3：创建Microsoft Word 文档
     * 4：创建txt文本文档
     * 5：创建md文件
     * --------------------------
     * 6：设置菜单对话框！
     * -------------------------
     * 7：文件操作之重命名对话框！
     *
     * */
    explicit newBuild_menuDialog(const QString &absPath = "", short actionType = 0, QWidget *parent = nullptr);

    ~newBuild_menuDialog() override;


private:
    Ui::newBuild_menuDialog *ui;

};


#endif //FILESYSTEM_NEWBUILD_MENUDIALOG_H
