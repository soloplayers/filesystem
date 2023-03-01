//
// Created by 93705 on 2022/4/10.
//

#ifndef FILESYSTEM_MYPUSHBUTTON_H
#define FILESYSTEM_MYPUSHBUTTON_H

#include <QPushButton>
#include <QHBoxLayout>
#include <QLabel>
#include <QMenu>
#include <QAction>


QT_BEGIN_NAMESPACE
namespace Ui { class myPushButton; }
QT_END_NAMESPACE

class myPushButton : public QPushButton {
Q_OBJECT

public:
    explicit myPushButton(int btnType, const QString &btn_text = "", const QString &img_path = "", double value = 0,
                          double maximum = 0, QWidget *parent = nullptr);

//    // 重写鼠标进入和离开的事件
//    void enterEvent(QEvent *) override;
//
//    void leaveEvent(QEvent *) override;
//
//    // 重写鼠标按下和释放时间
//    void mousePressEvent(QMouseEvent *) override;
//
//    void mouseReleaseEvent(QMouseEvent *) override;

    // 暴露一个可以操控的接口
    void setSelectedBtn(int btnType, bool show_Btn);

    ~myPushButton() override;

signals:

    void openProjetLocation();

    void removeProject();

public slots:

    void showMenu(const QPoint &point);

private:
    // 成员属性，维护住btnType
    /*
     * 1：代表关闭按钮
     * 2：代表最大化按钮和最小化按钮
     * 3：旁支内容的四个方向性按钮
     * 4：侧边栏的封装按钮
     * 5：侧边栏里面的按钮
     * 6：只有图片的按钮（如设置按钮），其中的btn_text是为了设置setToolTip
     * 7：左图片+右文字的按钮（水平布局）
     * 8：左文字+右图片的按钮（水平布局）
     * 9：图片在上+文字在下的按钮（垂直布局）--用于展示常用文件夹的按钮图标
     * 10: 只有图片的按钮（包部件需要）
     * 11: 专门表示右边布局的驱动器图片
     * 12: 专门表示包部件中添加的按钮（为了设置出背景色）
     * 13: 专门表示搜索出来的按钮
     * 14: 专门表示包部件中添加上的按钮
     *
     * */
    int numBtnType;
    // isSelected 维持住了一个布尔值，保存的是当前按钮是否处于selected状态，从而修改qss样式
//    bool isSelected;
    Ui::myPushButton *ui{};
    QString filePath = "";
    QString text = "";
    QHBoxLayout *btnHLayout = nullptr;
    QVBoxLayout *btnVLayout = nullptr;
    QLabel *firstPic = nullptr;
    QLabel *secondPic = nullptr;
    QLabel *firstBtnText = nullptr;
    QLabel *secondBtnText = nullptr;
    double valueSize;
    double maximumSize;

    // 为包部件里面的控件设置右键菜单
    QMenu *packageWrapperStoredWidgetGridLayoutItemMenu;
    QAction *packageWrapperStoredWidgetGridLayoutItemMenu_open;
    QAction *packageWrapperStoredWidgetGridLayoutItemMenu_remove;
};


#endif //FILESYSTEM_MYPUSHBUTTON_H
