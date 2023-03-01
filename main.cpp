#include "widget.h"
#include "myWidgets/myLogin/login.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
//    Widget w;
    login w;
    w.show();
    return QApplication::exec();
}
