//
// Created by 93705 on 2022/4/18.
//

// You may need to build the project (run Qt uic code generator) to get "ui_FramelessWidget.h" resolved

#include "framelesswidget.h"
#include "qevent.h"

FramelessWidget::FramelessWidget(QObject *parent) :
        QObject(parent) {
    // 初始化
    padding = 8;
    moveEnable = false;
    resizeEnable = false;
    widget = nullptr;

    pressed = false;
    pressedLeft = false;
    pressedRight = false;
    pressedTop = false;
    pressedBottom = false;
    pressedLeftTop = false;
    pressedRightTop = false;
    pressedLeftBottom = false;
    pressedRightBottom = false;

    for (bool &direction: directions) {
        direction = false;
    }
    // 下标0~10 分别为 上下左右、左上、右上、左下、右下、全部都是、全都不是
    // 默认全方向都可以放缩
    allDir = true;

    //如果父类是窗体则直接设置
    if (parent->isWidgetType()) {
        setWidget((QWidget *) parent);
    }
}

bool FramelessWidget::eventFilter(QObject *watched, QEvent *event) {
    if (widget != 0 && watched == widget) {
        if (event->type() == QEvent::Resize) {
            //重新计算八个描点的区域,描点区域的作用还有就是计算鼠标坐标是否在某一个区域内
            int width = widget->width();
            int height = widget->height();

            //左侧描点区域
            rectLeft = QRect(0, padding, padding, height - padding * 2);
            //上侧描点区域
            rectTop = QRect(padding, 0, width - padding * 2, padding);
            //右侧描点区域
            rectRight = QRect(width - padding, padding, padding, height - padding * 2);
            //下侧描点区域
            rectBottom = QRect(padding, height - padding, width - padding * 2, padding);

            //左上角描点区域
            rectLeftTop = QRect(0, 0, padding, padding);
            //右上角描点区域
            rectRightTop = QRect(width - padding, 0, padding, padding);
            //左下角描点区域
            rectLeftBottom = QRect(0, height - padding, padding, padding);
            //右下角描点区域
            rectRightBottom = QRect(width - padding, height - padding, padding, padding);
        } else if (event->type() == QEvent::HoverMove) {
            //设置对应鼠标形状,这个必须放在这里而不是下面,因为可以在鼠标没有按下的时候识别
            auto *hoverEvent = (QHoverEvent *) event;
            QPoint point = hoverEvent->pos();
            if (resizeEnable) {
                // 全方向放缩（默认方式）
                if (allDir) {
                    if (rectLeft.contains(point) || rectRight.contains(point)) {
                        widget->setCursor(Qt::SizeHorCursor);
                    } else if (rectTop.contains(point) || rectBottom.contains(point)) {
                        widget->setCursor(Qt::SizeVerCursor);
                    } else if (rectLeftTop.contains(point) || rectRightBottom.contains(point)) {
                        widget->setCursor(Qt::SizeFDiagCursor);
                    } else if (rectRightTop.contains(point) || rectLeftBottom.contains(point)) {
                        widget->setCursor(Qt::SizeBDiagCursor);
                    } else {
                        widget->setCursor(Qt::ArrowCursor);
                    }
                }
                    // 指定其他方向的箭头标志
                else {
                    // 上下是同一个箭头标志
                    if ((directions[0] && rectTop.contains(point)) ||
                        (directions[1] && rectBottom.contains(point))) {
                        widget->setCursor(Qt::SizeVerCursor);
                    }
                        // 左右是同一个箭头标志
                    else if ((directions[2] && rectLeft.contains(point)) ||
                             (directions[3] && rectRight.contains(point))) {
                        widget->setCursor(Qt::SizeHorCursor);
                    }
                        // 左上和右下是同一个箭头标志
                    else if ((directions[4] && rectLeftTop.contains(point)) ||
                             (directions[7] && rectRightBottom.contains(point))) {
                        widget->setCursor(Qt::SizeFDiagCursor);
                    }
                        // 左下和右上是同一个标志
                    else if ((directions[4] && rectLeftTop.contains(point)) ||
                             (directions[7] && rectRightBottom.contains(point))) {
                        widget->setCursor(Qt::SizeBDiagCursor);
                    } else {
                        widget->setCursor(Qt::ArrowCursor);
                    }
                }
            }

            //根据当前鼠标位置,计算XY轴移动了多少
            int offsetX = point.x() - lastPos.x();
            int offsetY = point.y() - lastPos.y();

            //根据按下处的位置判断是否是移动控件还是拉伸控件
            if (moveEnable) {
                if (pressed) {
                    widget->move(widget->x() + offsetX, widget->y() + offsetY);
                }
            }

            if (resizeEnable) {
                if (allDir) {
                    if (pressedLeft) {
                        widget->setCursor(Qt::SizeHorCursor);
                        int resizeW = widget->width() - offsetX;
                        if (widget->minimumWidth() <= resizeW) {
                            widget->setGeometry(widget->x() + offsetX, rectY, resizeW, rectH);
                        }
                    } else if (pressedRight) {
                        widget->setCursor(Qt::SizeHorCursor);
                        widget->setGeometry(rectX, rectY, rectW + offsetX, rectH);
                    } else if (pressedTop) {
                        widget->setCursor(Qt::SizeVerCursor);
                        int resizeH = widget->height() - offsetY;
                        if (widget->minimumHeight() <= resizeH) {
                            widget->setGeometry(rectX, widget->y() + offsetY, rectW, resizeH);
                        }
                    } else if (pressedBottom) {
                        widget->setCursor(Qt::SizeVerCursor);
                        widget->setGeometry(rectX, rectY, rectW, rectH + offsetY);
                    } else if (pressedLeftTop) {
                        widget->setCursor(Qt::SizeFDiagCursor);
                        int resizeW = widget->width() - offsetX;
                        int resizeH = widget->height() - offsetY;
                        if (widget->minimumWidth() <= resizeW) {
                            widget->setGeometry(widget->x() + offsetX, widget->y(), resizeW, resizeH);
                        }
                        if (widget->minimumHeight() <= resizeH) {
                            widget->setGeometry(widget->x(), widget->y() + offsetY, resizeW, resizeH);
                        }
                    } else if (pressedRightTop) {
                        int resizeW = rectW + offsetX;
                        int resizeH = widget->height() - offsetY;
                        if (widget->minimumHeight() <= resizeH) {
                            widget->setGeometry(widget->x(), widget->y() + offsetY, resizeW, resizeH);
                        }
                    } else if (pressedLeftBottom) {
                        int resizeW = widget->width() - offsetX;
                        int resizeH = rectH + offsetY;
                        if (widget->minimumWidth() <= resizeW) {
                            widget->setGeometry(widget->x() + offsetX, widget->y(), resizeW, resizeH);
                        }
                        if (widget->minimumHeight() <= resizeH) {
                            widget->setGeometry(widget->x(), widget->y(), resizeW, resizeH);
                        }
                    } else if (pressedRightBottom) {
                        int resizeW = rectW + offsetX;
                        int resizeH = rectH + offsetY;
                        widget->setGeometry(widget->x(), widget->y(), resizeW, resizeH);
                    }
                }
                    // 指定方向的修改
                else {
                    if (pressedLeft && directions[2]) {
                        widget->setCursor(Qt::SizeHorCursor);
                        int resizeW = widget->width() - offsetX;
                        if (widget->minimumWidth() <= resizeW) {
                            widget->setGeometry(widget->x() + offsetX, rectY, resizeW, rectH);
                        }
                    } else if (pressedRight && directions[3]) {
                        widget->setCursor(Qt::SizeHorCursor);
                        widget->setGeometry(rectX, rectY, rectW + offsetX, rectH);
                    } else if (pressedTop && directions[0]) {
                        widget->setCursor(Qt::SizeVerCursor);
                        int resizeH = widget->height() - offsetY;
                        if (widget->minimumHeight() <= resizeH) {
                            widget->setGeometry(rectX, widget->y() + offsetY, rectW, resizeH);
                        }
                    } else if (pressedBottom && directions[1]) {
                        widget->setCursor(Qt::SizeVerCursor);
                        widget->setGeometry(rectX, rectY, rectW, rectH + offsetY);
                    } else if (pressedLeftTop && directions[4]) {
                        widget->setCursor(Qt::SizeFDiagCursor);
                        int resizeW = widget->width() - offsetX;
                        int resizeH = widget->height() - offsetY;
                        if (widget->minimumWidth() <= resizeW) {
                            widget->setGeometry(widget->x() + offsetX, widget->y(), resizeW, resizeH);
                        }
                        if (widget->minimumHeight() <= resizeH) {
                            widget->setGeometry(widget->x(), widget->y() + offsetY, resizeW, resizeH);
                        }
                    } else if (pressedRightTop && directions[5]) {
                        int resizeW = rectW + offsetX;
                        int resizeH = widget->height() - offsetY;
                        if (widget->minimumHeight() <= resizeH) {
                            widget->setGeometry(widget->x(), widget->y() + offsetY, resizeW, resizeH);
                        }
                    } else if (pressedLeftBottom && directions[6]) {
                        int resizeW = widget->width() - offsetX;
                        int resizeH = rectH + offsetY;
                        if (widget->minimumWidth() <= resizeW) {
                            widget->setGeometry(widget->x() + offsetX, widget->y(), resizeW, resizeH);
                        }
                        if (widget->minimumHeight() <= resizeH) {
                            widget->setGeometry(widget->x(), widget->y(), resizeW, resizeH);
                        }
                    } else if (pressedRightBottom && directions[7]) {
                        int resizeW = rectW + offsetX;
                        int resizeH = rectH + offsetY;
                        widget->setGeometry(widget->x(), widget->y(), resizeW, resizeH);
                    }
                }
            }
        } else if (event->type() == QEvent::MouseButtonPress) {
            //记住当前控件坐标和宽高以及鼠标按下的坐标
            auto *mouseEvent = (QMouseEvent *) event;
            rectX = widget->x();
            rectY = widget->y();
            rectW = widget->width();
            rectH = widget->height();
            lastPos = mouseEvent->pos();

            //判断按下的手柄的区域位置
            if (rectLeft.contains(lastPos)) {
                pressedLeft = true;
            } else if (rectRight.contains(lastPos)) {
                pressedRight = true;
            } else if (rectTop.contains(lastPos)) {
                pressedTop = true;
            } else if (rectBottom.contains(lastPos)) {
                pressedBottom = true;
            } else if (rectLeftTop.contains(lastPos)) {
                pressedLeftTop = true;
            } else if (rectRightTop.contains(lastPos)) {
                pressedRightTop = true;
            } else if (rectLeftBottom.contains(lastPos)) {
                pressedLeftBottom = true;
            } else if (rectRightBottom.contains(lastPos)) {
                pressedRightBottom = true;
            } else {
                pressed = true;
            }
        } else if (event->type() == QEvent::MouseButtonRelease) {
            //恢复所有
            pressed = false;
            pressedLeft = false;
            pressedRight = false;
            pressedTop = false;
            pressedBottom = false;
            pressedLeftTop = false;
            pressedRightTop = false;
            pressedLeftBottom = false;
            pressedRightBottom = false;
            widget->setCursor(Qt::ArrowCursor);
        }
    }

    return QObject::eventFilter(watched, event);
}

void FramelessWidget::setPadding(int padding) {
    this->padding = padding;
}

void FramelessWidget::setMoveEnable(bool moveEnable) {
    this->moveEnable = moveEnable;
}

void FramelessWidget::setResizeEnable(bool resizeEnable) {
    this->resizeEnable = resizeEnable;
}

void FramelessWidget::setWidget(QWidget *widget) {
    if (this->widget == 0) {
        this->widget = widget;
        //设置鼠标追踪为真
        this->widget->setMouseTracking(true);
        //绑定事件过滤器
        this->widget->installEventFilter(this);
        //设置悬停为真,必须设置这个,不然当父窗体里边还有子窗体全部遮挡了识别不到MouseMove,需要识别HoverMove
        this->widget->setAttribute(Qt::WA_Hover, true);
    }
}

void FramelessWidget::setDirection(myDirections direction) {
    bool flag = false;   // 一个标志信号，是否改变了direction数组中的值
    switch (direction) {
        case 0:
            this->directions[0] = true;
            flag = true;
            break;
        case 1:
            this->directions[1] = true;
            flag = true;
            break;
        case 2:
            this->directions[2] = true;
            flag = true;
            break;
        case 3:
            this->directions[3] = true;
            flag = true;
            break;
        case 4:
            this->directions[4] = true;
            flag = true;
            break;
        case 5:
            this->directions[5] = true;
            flag = true;
            break;
        case 6:
            this->directions[6] = true;
            flag = true;
            break;
        case 7:
            this->directions[7] = true;
            flag = true;
            break;
        case 8:
            allDir = true;
            break;
        default:
            allDir = false;
            flag = true;
            break;
    }
    if (flag) {
        allDir = false;
    }
}

void FramelessWidget::setWidgetSize(QSize size) {

}

void FramelessWidget::setWidgetSize(int w, int h) {

}

