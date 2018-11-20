#include "gamemenu.h"
#include <QLabel>
#include <QRadioButton>
#include <QPushButton>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QDebug>
#include <QLineEdit>
#include <QMessageBox>

GameMenu::GameMenu(QWidget *parent) : QWidget(parent)
{
    createWidgets();
    createLayouts();
    createStyleSheets();
    createConnections();
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    emit pvpClicked();

}

void GameMenu::createWidgets()
{
    m_title = new QLabel;
    m_PVPmode = new QRadioButton;
    m_PVCmode = new QRadioButton;
    m_nextButton = new QPushButton;
    m_nextButton->setCursor(QCursor(Qt::PointingHandCursor));


    m_player1VsPlEdit = new QLineEdit;
    m_player2VsPlEdit =new QLineEdit;
    m_player1VsCompEdit = new QLineEdit;

    m_player1VsPlLabel = new QLabel;
    m_player2VsPlLabel = new QLabel;
    m_player1VsCompLabel = new QLabel;
}

void GameMenu::createLayouts()
{
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(m_title,3);

    layout->addSpacing(55);
    layout->addWidget(m_PVPmode, 3);
    layout->addSpacing(15);

    QHBoxLayout *pVsPLinesEdit = new QHBoxLayout;
    pVsPLinesEdit->addWidget(m_player1VsPlLabel,1);
    pVsPLinesEdit->addWidget(m_player1VsPlEdit,1);
    pVsPLinesEdit->addWidget(m_player2VsPlLabel, 1);
    pVsPLinesEdit->addWidget(m_player2VsPlEdit, 1);

    layout->addLayout(pVsPLinesEdit, 1);


    layout->addSpacing(50);
    layout->addWidget(m_PVCmode, 1);
    QHBoxLayout *pVsCpLinesEdit = new QHBoxLayout;
    pVsCpLinesEdit->addWidget(m_player1VsCompLabel, 1);
    pVsCpLinesEdit->addWidget(m_player1VsCompEdit, 1);

    layout->addLayout(pVsCpLinesEdit, 1);

    layout->addSpacing(50);
    layout->addWidget(m_nextButton, 1);
    setLayout(layout);
}

void GameMenu::createStyleSheets()
{
    setStyleSheet("QWidget{background-color: black;}"
                  "QLineEdit{color: white;}"
                  "QLabel{color: white;}"
                  "QRadioButton{color: white;}"
                  "QPushButton{background-color: #13100a; border-style: solid;  border-width: 1px; border-radius: 10px; color: white; font-size: 20px; font-family: \'Arial\'; font-weight: 500px;}"
                  "QPushButton:hover{background-color: #24211b;}");
    QFont font("Arial", 30, 500, true);
    m_title->setText(tr("Draughts"));
    m_title->setFont(font);
    m_title->setAlignment(Qt::AlignCenter );
   // m_title->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QFont fontMode("Arial", 20, 500, true);
    m_PVPmode->setText("Player vs Player");
    m_PVPmode->setFont(fontMode);
    m_PVCmode->setText("Player vs Computer");
    m_PVCmode->setFont(fontMode);

    m_nextButton->setText(tr("Next"));
    m_nextButton->setMinimumHeight(100);

    QFont fontEdit("Arial", 15, 300);
    m_player1VsPlLabel->setText(tr("Player 1"));
    m_player1VsPlLabel->setFont(fontEdit);
    m_player2VsPlLabel->setText(tr("Player 2"));
    m_player2VsPlLabel->setFont(fontEdit);
    m_player1VsPlEdit->setFont(fontEdit);
    m_player1VsPlEdit->setMaxLength(15);
    m_player2VsPlEdit->setFont(fontEdit);
    m_player2VsPlEdit->setMaxLength(15);

    m_player1VsCompLabel->setText(tr("Player 1"));
    m_player1VsCompLabel->setFont(fontEdit);
    m_player1VsCompEdit->setFont(fontEdit);
    m_player1VsCompEdit->setMaxLength(15);

    setMinimumSize(QSize(500, 500));
    //setMaximumSize(QSize(500, 500));

    m_nextButton->setEnabled(false);
}


void GameMenu::createConnections()
{

    connect(m_nextButton, &QPushButton::clicked, this, &GameMenu::nextClicked);
    connect(m_nextButton, &QPushButton::clicked, this, [this](){
                if(m_PVPmode->isChecked())
                {
                    emit pvpClicked();
                }
                else
                {
                    emit pvcClicked();
                }
            });
    connect(m_PVPmode, &QRadioButton::clicked, this, [this]()
    {
        m_nextButton->setEnabled(false);
        hidePvCEdits();
        showPvPEdits();
    });

    connect(m_PVCmode, &QRadioButton::clicked, this, [this]()
    {
        m_nextButton->setEnabled(false);
        hidePvPEdits();
        showPvCEdits();
    });

    connect(m_player1VsPlEdit, &QLineEdit::textChanged, this, &GameMenu::checkNextButtonEnabled);
    connect(m_player2VsPlEdit, &QLineEdit::textChanged, this, &GameMenu::checkNextButtonEnabled);
    connect(m_player1VsCompEdit, &QLineEdit::textChanged, this, &GameMenu::checkNextButtonEnabled);

    m_PVPmode->click();

}

void GameMenu::hidePvPEdits()
{
    m_player1VsPlEdit->hide();
    m_player2VsPlEdit->hide();
    m_player1VsCompEdit->hide();

    m_player1VsPlLabel->hide();
    m_player2VsPlLabel->hide();
}

void GameMenu::hidePvCEdits()
{
    m_player1VsCompLabel->hide();
    m_player1VsCompEdit->hide();
}

void GameMenu::showPvPEdits()
{
    m_player1VsPlEdit->show();
    m_player1VsPlEdit->clear();
    m_player2VsPlEdit->show();
    m_player2VsPlEdit->clear();


    m_player1VsPlLabel->show();
    m_player2VsPlLabel->show();
}

void GameMenu::showPvCEdits()
{
    m_player1VsCompLabel->show();
    m_player1VsCompEdit->show();
    m_player1VsCompEdit->clear();
}

void GameMenu::checkNextButtonEnabled()
{
    if( m_PVPmode->isChecked() )
    {
        m_nextButton->setEnabled(!m_player1VsPlEdit->text().isEmpty() && !m_player2VsPlEdit->text().isEmpty() );
    }

    if( m_PVCmode->isChecked() )
    {
        m_nextButton->setEnabled(!m_player1VsCompEdit->text().isEmpty());
    }
}

QString GameMenu::player1VSPlayerName()
{
    return m_player1VsPlEdit->text();
}

QString GameMenu::player2VSPlayerName()
{
    return m_player2VsPlEdit->text();
}

QString GameMenu::playerVsCompName()
{
    return m_player1VsCompEdit->text();
}

