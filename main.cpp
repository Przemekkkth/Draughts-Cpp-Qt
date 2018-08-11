#include "gamewindow.h"
#include <QApplication>
#include <QIcon>
#include "gamemenu.h"
#include "gamemainwindow.h"
#include "gameinterface.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GameInterface w;
    w.setWindowTitle(QObject::tr("Draughts"));
    w.setWindowIcon(QIcon(":/img/windowIcon.png"));
    w.show();

    return a.exec();
}
