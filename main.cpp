#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("Расширение рабочего пространства САПР");
    
    MainWindow window;
    window.show();
    
    return app.exec();
}
