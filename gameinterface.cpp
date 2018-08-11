#include "gameinterface.h"
#include "gamemenu.h"
#include "gamemainwindow.h"
#include "gamewindow.h"
#include "gamealgorithm.h"
#include <QStackedWidget>
#include <QHBoxLayout>
#include <QDebug>

GameInterface::GameInterface(QWidget *parent) : QWidget(parent)
{
    createWidgets();
    createLayouts();
    createStylesheets();
    init();
    createConnections();

    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
}

void GameInterface::createWidgets()
{
    m_gameMenu = new GameMenu;
    m_gameMainWindow = new GameMainWindow;
    m_mainContainer = new QStackedWidget(this);
}

void GameInterface::init()
{
    m_mainContainer->addWidget(m_gameMenu);
    m_mainContainer->addWidget(m_gameMainWindow);
}

void GameInterface::createLayouts()
{
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(m_mainContainer);
    setLayout(layout);
}

void GameInterface::createStylesheets()
{
    setStyleSheet("QWidget{background-color: black;}"
                  "QStackedWidget{background-color: black;}");
    resize(m_gameMainWindow->size());
}

QSize GameInterface::sizeHint() const
{
    return m_gameMainWindow->size();
}

void GameInterface::createConnections()
{
    connect(m_gameMenu, &GameMenu::nextClicked, this, [this]{
        m_mainContainer->setCurrentIndex(1);
        m_gameMainWindow->setPlayer1Name( m_gameMenu->player1VSPlayerName() );
        m_gameMainWindow->setPlayer2Name( m_gameMenu->player2VSPlayerName() );
    });
    connect(m_gameMainWindow, &GameMainWindow::exitClicked, this, [this]{
        m_mainContainer->setCurrentIndex(0 );
    });

}
