#ifndef TITLE_BAR
#define TITLE_BAR

#include <QWidget>
#include "MyWidgets/myPushButton/mypushbutton.h"

class QLabel;

class QPushButton;

class TitleBar : public QWidget {
Q_OBJECT

public:
    explicit TitleBar(QWidget *parent = 0);

    ~TitleBar() override;

protected:

    // 双击标题栏进行界面的最大化/还原
    void mouseDoubleClickEvent(QMouseEvent *event) override;

    // 进行鼠标界面的拖动
    void mousePressEvent(QMouseEvent *event) override;

    // 设置界面标题与图标
    bool eventFilter(QObject *obj, QEvent *event) override;

private slots:

    // 进行最小化、最大化/还原、关闭操作
    void onClicked();

private:

    // 最大化/还原
    void updateMaximize();


private:
    QLabel *m_pIconLabel;
    QLabel *m_pTitleLabel;
    myPushButton *m_pMinimizeButton;
    myPushButton *m_pMaximizeButton;
    myPushButton *m_pCloseButton;
};

#endif // TITLE_BAR