#include "gamemainwindow.h"
#include "gamewindow.h"
#include "gameboard.h"
#include "gamealgorithm.h"
#include <QPushButton>
#include <QLabel>
#include <QLCDNumber>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include <QDebug>
#include <QMessageBox>
GameMainWindow::GameMainWindow(QWidget *parent) : QWidget(parent)
{
    createWidgets();
    createLayouts();
    createStylesheets();
    createConnections();
}

void GameMainWindow::createWidgets()
{
    m_gameWindow = new GameWindow;
    m_player1Name = new QLabel;
    m_player2Name = new QLabel;
    m_title = new QLabel;
    m_scorePlayer1 = new QLCDNumber;
    m_scorePlayer2 = new QLCDNumber;

    m_resetButton = new QPushButton;
    m_exitButton = new QPushButton;

}

void GameMainWindow::createLayouts()
{
    QHBoxLayout *hlayout = new QHBoxLayout;
    QVBoxLayout *vlayout = new QVBoxLayout;
    vlayout->addWidget(m_title);
    vlayout->addWidget(m_player1Name);
    vlayout->addWidget(m_scorePlayer1);
    vlayout->addWidget(m_player2Name);
    vlayout->addWidget(m_scorePlayer2);
    vlayout->addWidget(m_resetButton);
    vlayout->addWidget(m_exitButton);
    hlayout->addWidget(m_gameWindow, 3);
    hlayout->addLayout(vlayout);
    setLayout(hlayout);
}

void GameMainWindow::createStylesheets()
{
    m_title->setText(tr("Draughts"));
    QFont font("Arial", 20, 900);
    m_title->setFont(font);

    m_scorePlayer1->display( QString::number(m_gameWindow->getAlgorithm()->board()->countOfBlack()) );
    m_scorePlayer2->display( QString::number(m_gameWindow->getAlgorithm()->board()->countOfWhite()) );

    m_title->setAlignment(Qt::AlignCenter);
    QFont fontLabel("Arial", 14, 900);
    m_player1Name->setText(tr("Player1"));
    m_player1Name->setAlignment(Qt::AlignCenter);
    m_player1Name->setFont(fontLabel);
    m_player2Name->setText(tr("Player2"));
    m_player2Name->setAlignment(Qt::AlignCenter);
    m_player2Name->setFont(fontLabel);

    m_resetButton->setText(tr("Reset"));
    m_resetButton->setCursor(Qt::PointingHandCursor);
    m_exitButton->setText(tr("Exit"));
    m_exitButton->setCursor(Qt::PointingHandCursor);

    setStyleSheet("QWidget{background-color: black;}"
                  "QLineEdit{color: white;}"
                  "QLabel{color: white;}"
                  "QRadioButton{color: white;}"
                  "QPushButton{background-color: #13100a; border-style: solid;  border-width: 1px; border-radius: 10px; color: white; font-size: 20px; font-family: \'Arial\'; font-weight: 500px;}"
                  "QPushButton:hover{background-color: #24211b;}");
}


void GameMainWindow::setScoreP1(int val)
{
    m_scorePlayer1->display(QString::number(val));
}

void GameMainWindow::setScoreP2(int val)
{
    m_scorePlayer2->display(QString::number(val));
}

void GameMainWindow::createConnections()
{
    connect(m_gameWindow->getAlgorithm(), SIGNAL(countOfBlackPiecesChanged(int)), this, SLOT(setScoreP1(int)));
    connect(m_gameWindow->getAlgorithm(), SIGNAL(countOfWhitePiecesChanged(int)), this, SLOT(setScoreP2(int)));
    connect(m_exitButton, &QPushButton::clicked, this, [this]{
        emit exitClicked();
    });
    connect(m_resetButton, &QPushButton::clicked, this, [this]{
        m_gameWindow->getAlgorithm()->restart();
        update();
    });

    connect(m_gameWindow->getAlgorithm(),
            &GameAlgorithm::currentPlayerChanged,
            this, [this](GameAlgorithm::Player player)
    {
        
        if(player == GameAlgorithm::Player1)
        {
            QFont fontLabel("Arial", 8, 900);
            m_player2Name->setFont(fontLabel);

            QFont activePlayerFont("Arial", 16, 1200, true);

            m_player1Name->setFont(activePlayerFont);
        }
        else
        {
            QFont fontLabel("Arial", 8, 900);
            m_player1Name->setFont(fontLabel);

            QFont activePlayerFont("Arial", 16, 1200, true);
            m_player2Name->setFont(activePlayerFont);
        }
    });

    connect(m_gameWindow->getAlgorithm(),
            &GameAlgorithm::playerWins,
            this,
            [this](GameAlgorithm::Player player){
        QString textWin = "";
        if(player == GameAlgorithm::Player1)
        {
            textWin = m_player1Name->text() + " wins!";
        }
        else if(player == GameAlgorithm::Player2)
        {
            textWin = m_player2Name->text() + " wins!";
        }
        QMessageBox::information(this, tr("Information"), textWin);
        m_gameWindow->getAlgorithm()->restart();
    });
}

void GameMainWindow::setPlayer1Name(const QString& name)
{
    m_player1Name->setText(name);
}

void GameMainWindow::setPlayer2Name(const QString& name)
{
    m_player2Name->setText(name);
}
