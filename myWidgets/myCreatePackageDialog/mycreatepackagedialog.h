//
// Created by 93705 on 2022/5/25.
//

#ifndef FILESYSTEM_MYCREATEPACKAGEDIALOG_H
#define FILESYSTEM_MYCREATEPACKAGEDIALOG_H

#include <QDialog>


QT_BEGIN_NAMESPACE
namespace Ui { class myCreatePackageDialog; }
QT_END_NAMESPACE

class myCreatePackageDialog : public QDialog {
Q_OBJECT

signals:

    void sendData(QString,int);

public:
    explicit myCreatePackageDialog(QWidget *parent = nullptr);

    ~myCreatePackageDialog() override;

private:
    Ui::myCreatePackageDialog *ui{};
};


#endif //FILESYSTEM_MYCREATEPACKAGEDIALOG_H
