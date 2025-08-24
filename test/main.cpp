#include <QtCore/QCoreApplication>

#include "TestWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    TestWindow w;
    w.show();

    return app.exec();
}
