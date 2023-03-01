#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QEvent>
#include <QMouseEvent>
#include <QApplication>
#include "title_bar.h"
#include <QIcon>
#include "MyWidgets/myPushButton/mypushbutton.h"
#include "QDebug"

#ifdef Q_OS_WIN
#pragma comment(lib, "user32.lib")

#include <qt_windows.h>
#include <QPainter>
#include <QTimer>

#endif

TitleBar::TitleBar(QWidget *parent)
        : QWidget(parent) {
    setFixedHeight(35);
    // 可添加border
    setStyleSheet("background-color:transparent");

    m_pIconLabel = new QLabel(this);
    m_pTitleLabel = new QLabel(this);
    m_pMinimizeButton = new myPushButton(2, "","",0,0,this);
    m_pMaximizeButton = new myPushButton(2, "","",0,0,this);
    m_pCloseButton = new myPushButton(1, "","",0,0,this);


    m_pTitleLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_pTitleLabel->setStyleSheet("font-size:20px;");

    m_pMinimizeButton->setFixedSize(55, 35);
    m_pMaximizeButton->setFixedSize(55, 35);
    m_pCloseButton->setFixedSize(55, 35);
    m_pIconLabel->setFixedSize(20, 20);
    m_pIconLabel->setScaledContents(true);

    m_pTitleLabel->setObjectName("titleLabel");
    m_pMinimizeButton->setObjectName("minimizeButton");
    m_pMaximizeButton->setObjectName("maximizeButton");
    m_pCloseButton->setObjectName("closeButton");

    m_pCloseButton->setIcon(QIcon(":/res/myTitleBar_imgs/close.png"));
    m_pMaximizeButton->setIcon(QIcon(":/res/myTitleBar_imgs/maximum.png"));
    m_pMinimizeButton->setIcon(QIcon(":/res/myTitleBar_imgs/minimum.png"));
    m_pCloseButton->setIconSize(QSize(20, 20));
    m_pMaximizeButton->setIconSize(QSize(20, 20));
    m_pMinimizeButton->setIconSize(QSize(20, 20));


    m_pCloseButton->setToolTip("关闭");
    m_pMaximizeButton->setToolTip("最大化");
    m_pMinimizeButton->setToolTip("最小化");


    auto *pLayout = new QHBoxLayout(this);
    pLayout->addWidget(m_pIconLabel);
    pLayout->addSpacing(5);
    pLayout->addWidget(m_pTitleLabel);
    pLayout->addWidget(m_pMinimizeButton);
    pLayout->addWidget(m_pMaximizeButton);
    pLayout->addWidget(m_pCloseButton);
    pLayout->setSpacing(0);
    pLayout->setContentsMargins(5, 0, 5, 0);
    setLayout(pLayout);


    connect(m_pMinimizeButton, SIGNAL(clicked(bool)), this, SLOT(onClicked()));
    connect(m_pMaximizeButton, SIGNAL(clicked(bool)), this, SLOT(onClicked()));
    connect(m_pCloseButton, SIGNAL(clicked(bool)), this, SLOT(onClicked()));
}

TitleBar::~TitleBar()
= default;

void TitleBar::mouseDoubleClickEvent(QMouseEvent *event) {
    Q_UNUSED(event);
    emit m_pMaximizeButton->clicked();
}

void TitleBar::mousePressEvent(QMouseEvent *event) {
#ifdef Q_OS_WIN
    if (ReleaseCapture()) {
        QWidget *pWindow = this->window();
        if (pWindow->isTopLevel()) {
            SendMessage(HWND(pWindow->winId()), WM_SYSCOMMAND, SC_MOVE + HTCAPTION, 0);
        }
    }
    event->ignore();
#else
#endif
}

bool TitleBar::eventFilter(QObject *obj, QEvent *event) {
    switch (event->type()) {
        case QEvent::WindowTitleChange: {
            QWidget *pWidget = qobject_cast<QWidget *>(obj);
            if (pWidget) {
                m_pTitleLabel->setText(pWidget->windowTitle());
                return true;
            }
        }
        case QEvent::WindowIconChange: {
            QWidget *pWidget = qobject_cast<QWidget *>(obj);
            if (pWidget) {
                QIcon icon = pWidget->windowIcon();
                m_pIconLabel->setPixmap(icon.pixmap(m_pIconLabel->size()));
                return true;
            }
        }
        case QEvent::WindowStateChange:
        case QEvent::Resize:
            updateMaximize();
            return true;
        default:
            return QWidget::eventFilter(obj, event);

    }
}

void TitleBar::onClicked() {
    auto *pButton = qobject_cast<QPushButton *>(sender());
    QWidget *pWindow = this->window();
    if (pWindow->isTopLevel()) {
        if (pButton == m_pMinimizeButton) {
            pWindow->showMinimized();
        } else if (pButton == m_pMaximizeButton) {
            pWindow->isMaximized() ? pWindow->showNormal() : pWindow->showMaximized();
        } else if (pButton == m_pCloseButton) {
            pWindow->close();
        }
    }
}

void TitleBar::updateMaximize() {
    QWidget *pWindow = this->window();
    if (pWindow->isTopLevel()) {
        bool bMaximize = pWindow->isMaximized();
        if (bMaximize) {
            m_pMaximizeButton->setProperty("maximizeProperty", "restore");
            m_pMaximizeButton->setIcon(QIcon(":/res/myTitleBar_imgs/restore.png"));
            m_pMaximizeButton->setToolTip("还原");
        } else {
            m_pMaximizeButton->setProperty("maximizeProperty", "maximize");
            m_pMaximizeButton->setIcon(QIcon(":/res/myTitleBar_imgs/maximum.png"));
            m_pMaximizeButton->setToolTip("最大化");
        }
        m_pMaximizeButton->setStyle(QApplication::style());
    }
}