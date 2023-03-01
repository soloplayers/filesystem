//
// Created by 93705 on 2022/4/18.
//

#ifndef UNTITLED_FRAMELESSWIDGET_H
#define UNTITLED_FRAMELESSWIDGET_H

#include "QObject"
#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui { class FramelessWidget; }
QT_END_NAMESPACE

#ifdef quc
#if (QT_VERSION < QT_VERSION_CHECK(5,7,0))
#include <QtDesigner/QDesignerExportWidget>
#else
#include <QtUiPlugin/QDesignerExportWidget>
#endif

class QDESIGNER_WIDGET_EXPORT FramelessWidget : public QObject
#else

class FramelessWidget : public QObject
#endif
{
Q_OBJECT
public:
    explicit FramelessWidget(QObject *parent = 0);

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

public:
    // 枚举方向值
    enum myDirections {
        Up, Down, Left, Right, LeftUp, RightUp, LeftDown, RightDown, allDirection, noneDirection
    };

private:
    int padding;                    //边距
    bool moveEnable;                //可移动
    bool resizeEnable;              //可拉伸

    //拉伸方向
    /*
     * 下标0~10 分别为 上下左右、左上、右上、左下、右下、全部都是、全都不是
     *
     * */
    bool directions[10]{};
    bool allDir;


    QWidget *widget;                //无边框窗体

    bool pressed;                   //鼠标按下
    bool pressedLeft;               //鼠标按下左侧
    bool pressedRight;              //鼠标按下右侧
    bool pressedTop;                //鼠标按下上侧
    bool pressedBottom;             //鼠标按下下侧
    bool pressedLeftTop;            //鼠标按下左上侧
    bool pressedRightTop;           //鼠标按下右上侧
    bool pressedLeftBottom;         //鼠标按下左下侧
    bool pressedRightBottom;        //鼠标按下右下侧

    int rectX{}, rectY{}, rectW{}, rectH{}; //窗体坐标+宽高

    QPoint lastPos;                 //鼠标按下处坐标

    QRect rectLeft;                 //左侧区域
    QRect rectRight;                //右侧区域
    QRect rectTop;                  //上侧区域
    QRect rectBottom;               //下侧区域
    QRect rectLeftTop;              //左上侧区域
    QRect rectRightTop;             //右上侧区域
    QRect rectLeftBottom;           //左下侧区域
    QRect rectRightBottom;          //右下侧区域

public Q_SLOTS:

    //设置边距
    void setPadding(int padding);

    //设置是否可拖动+拉伸
    void setMoveEnable(bool moveEnable);

    void setResizeEnable(bool resizeEnable);

    // 手动设置控件size大小
    void setWidgetSize(QSize size);

    void setWidgetSize(int w, int h);

    // 设置拉伸方向
    void setDirection(enum myDirections direction);

    //设置要无边框的窗体
    void setWidget(QWidget *widget);
};


#endif //UNTITLED_FRAMELESSWIDGET_H
