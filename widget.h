#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "myWidgets/myPushButton/mypushbutton.h"
#include "myWidgets/myLineEdit/mylineedit.h"
#include "myWidgets/myMenuDialog/newbuild_menudialog.h"
#include "myWidgets/myCreatePackageDialog/mycreatepackagedialog.h"
#include <QFileSystemModel>
#include <QTableView>
#include <QOpenGLBuffer>
#include <QStackedWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget {
Q_OBJECT

private slots:

    void receiveData_handleCreatePackage(QString data, int isCancel);

    void receiveData_handleRenamePackage(QString data, int isCancel);

public slots:

    void onTableViewDoubleClicked();

    void handleSelectionChanged();

public:
    explicit Widget(QWidget *parent = nullptr);


    // 事件过滤器
    bool eventFilter(QObject *, QEvent *) override;

    // 鼠标拖拽自定义缩放
    bool nativeEvent(const QByteArray &eventType, void *message, long *result) override;

    // 重写绘图事件，设置背景颜色
    void paintEvent(QPaintEvent *) override;

    // 测试鼠标按下事件
    void mousePressEvent(QMouseEvent *) override;

    void mouseMoveEvent(QMouseEvent *) override;

    virtual ~Widget() override;

private:
    QWidget *mainWidget;
    QWidget *asideWidget;
    QWidget *rightWidget;
    QFileSystemModel *fileModel;
    QTableView *fileTableView;
    QStackedWidget *rightStackedWidget;
    int stackedWidget_index;
    // 维护住 按钮，被选中后，背景色高亮+蓝色竖直线（假设添加总数上限为1000）
    // 起始是默认有11 + driverList.count()个可被选中的按钮,在左栏依次从上到下编号
    /*
     * 0: 收藏夹的大栏
     *      1: 收藏夹下的窗口中---主页
     *      2: 收藏夹下的窗口中---桌面
     *      3: 收藏夹下的窗口中---下载
     *      4: 收藏夹下的窗口中---文档
     *      5: 收藏夹下的窗口中---回收站
     * 6: 驱动器的大栏 (磁盘有driverList.count()个，我的电脑是2个)
     *      7：驱动器下的窗口中---C盘
     *      8：驱动器下的窗口中---D盘
     * 9：oneDrive的大栏
     *      10：oneDrive下的窗口中---oneDrive
     * 11: 网络的大栏
     *      12：网络下的窗口中---网络
     * 13~~: 索引均为添加的文件夹数或者驱动器的磁盘数
     * 总数计算方法 = 默认的固定按钮：11 + 驱动器磁盘个数driverList.count() + 添加的收藏文件夹数
     * */
    myPushButton *leftScrollAreaSelectedBtn[1000]{};
    // 标记其中的按钮是否被选中
    bool leftScrollAreaSelectedBtn_selected_flag[1000]{};
    // 标记左边的大栏中的总数，一共有多少个按钮,
    int leftScrollAreaSelectedBtn_count;
    short addCollectionFolders;

    // 封装成一个函数，增加代码可读性，将被点击的按钮的flag值置为true，其他的均设为false;
    void changFlagToTrueWithOtherFlagFalse(int index);

    // 实现指定的索引下标被选中，从而设计一连串的连锁反应
    void showSelectedBtn(bool flag_arr[]);

    // 设置在主页进行禁用或可用的按钮
    void homePageAllBtn_setEnable();

    // 设置禁用右边布局的所有按钮
    void fixedToolWidget_setAllBtnDisable();

    // 设置功能按钮的禁用和启用
    void fixedToolWidget_setFunctionalBtnDisable();

    void fixedToolWidget_setFunctionalBtnEnable();

    // 根据absolutePath内容将相应的按钮设置为选中状态
    void setEnableBtnFromAbsolutePath();

    // 复制一个文件夹所有内容到另一个文件夹中
    bool copyFolder(const QString &oldDir, const QString &newDir, bool coverFileIfExist);

    // 复制文件
    bool copyFile(const QString &oldDir, const QString &newDir, bool coverFileIfExist);

    // 统计目录下的文件夹和文件项个数
    void FilesAndDirs_count();

    // 搜索文件或文件夹一栏
    bool searchFilesAndFolders(QString path, QString name);

    // 保留搜索下来的值
    QList<QString> searchFiles;
    QList<QString> searchFolders;

    // 统计某个文件夹的大小
    quint64 getCurrentDirSize(QString dirPath);

    quint64 tempSize;

    DWORD ListFileInRecycleBin();


    Ui::Widget *ui;
    int m_nBorderWidth; //m_nBorder表示鼠标位于边框缩放范围的宽度
    bool leftLayout_flag_collection;
    bool leftLayout_flag_driver;
    bool leftLayout_flag_oneDrive;
    bool leftLayout_flag_network;
    bool rightLayout_flag_showFolders;
    bool rightLayout_flag_showDrivers;
    bool rightLayout_flag_showPackages;
    bool rightLayout_flag_showRecentUsing;

    bool manageItems_menu_toggleFolders_flag;
    bool manageItems_menu_toggleDrivers_flag;
    bool manageItems_menu_togglePackages_flag;
    bool manageItems_menu_toggleRecentUsing_flag;

    // 维护主贯穿全应用的文件（夹）的绝对路径
    QString absolutePath;

    // 旁支内容中心的两个文本框布局
    myLineEdit *asideLineEdit_left;
    myLineEdit *asideLineEdit_right;

    // 底部状态栏信息
    QLabel *statusMsg_first;
    QLabel *statusMsg_second;

    // 为固定工具窗口的右边窗口的左边添加组件
    // 剪切、复制、粘贴、重命名、共享、…(表示更多，包括删除和属性)
    myPushButton *newBuild;
    myPushButton *cut;
    myPushButton *copy;
    myPushButton *paste;
    myPushButton *rename;
    myPushButton *deleteTarget;
    myPushButton *itemProperty;

    // 存储每次absolutePath的值，模拟栈容器,以便于设置前进和后退的功能
    QList<QString> absolutePathIterator;
    int absolutePathIterator_index;

    // 自定义菜单的制作
    QMenu *newBuild_menu;
    newBuild_menuDialog *actionMenu;

    // 包部件的创建
    myCreatePackageDialog *createPackageBtnDialog;
    QString packageName;
    myPushButton *createPackageBtn;
    // 可用fileInfo判断是目录还是文件
    QList<QString> addPackageFileInfoList;
    // 布局控件
    QVBoxLayout *packageWrapperMainVLayout;
    QWidget *packageWrapper;
    QWidget *packageWrapperTitleWidget;
    QHBoxLayout *packageWrapperTitleWidgetHLayout;
    QLabel *packageTitleLabel;
    myPushButton *packageWrapperTitleLabel_setting;
    QMenu *packageWrapperTitleLabel_menu;
    QAction *packageWrapperTitleLabel_menu_rename;
    QAction *packageWrapperTitleLabel_menu_addFiles;
    QAction *packageWrapperTitleLabel_menu_addFolders;
    QAction *packageWrapperTitleLabel_menu_removePackage;
    QWidget *packageWrapperStoredWidget;
    QGridLayout *packageWrapperStoredWidgetGridLayout;
    myPushButton *packageWrapperStoredWidgetGridLayoutItems[11];
    int packageWrapperStoredWidgetGridLayout_row;
    int packageWrapperStoredWidgetGridLayout_col;

    // 移除某个常用部件后，进行布局的更新（防止空位）
    void packageWrapperStoredWidgetGridLayoutUpdate(int removeIndex);

    void removePackage();

    void loadPackageFiles();

    void writeIntoPackageJsonFiles(QString fileName, bool isRenamePackage = false,int removeIndex=-1);


    // 表示当前选中的文件和文件夹
    QModelIndexList currentSelectedList;
    QModelIndexList copyCurrentSelectedList;

    // 表示最后一次点击的复制还是剪切的动作
    // 复制copy的值为true,而剪切cut的值为false
    bool isCopyOrCut;

    // 保留最近使用的文件
    QList<QString> recentUsingFiles;
    QVBoxLayout *showRecentUsingItemsVLayout;
    QLabel *defaultShowLabel;

    // 加载json文件
    void loadRecentUsingFiles();

    // 写入json文件
    void writeIntoRecentUsingJsonFiles(QString fileName);

};

#endif // WIDGET_H
