#ifndef GAMEMAINWINDOW_H
#define GAMEMAINWINDOW_H

#include <QWidget>
class GameWindow;
class QLabel;
class QLCDNumber;
class QPushButton;

class GameMainWindow : public QWidget
{
    Q_OBJECT
public:
    explicit GameMainWindow(QWidget *parent = nullptr);
    void setPlayer1Name(const QString& name);
    void setPlayer2Name(const QString& name);
    GameWindow* window(){
        return m_gameWindow;
    }
signals:
    void exitClicked();
private slots:
    void setScoreP1(int val);
    void setScoreP2(int val);
private:
    GameWindow *m_gameWindow;
    QLabel *m_title;
    QLabel *m_player1Name;
    QLabel *m_player2Name;
    QLCDNumber *m_scorePlayer1;
    QLCDNumber *m_scorePlayer2;
    QWidget *m_infoWidget;
    QPushButton* m_resetButton;
    QPushButton* m_exitButton;

    void createWidgets();
    void createLayouts();
    void createStylesheets();
    void createConnections();
};

#endif // GAMEMAINWINDOW_H
