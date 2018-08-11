#ifndef GAMEMENU_H
#define GAMEMENU_H

#include <QWidget>
class QLabel;
class QRadioButton;
class QPushButton;
class QLineEdit;
class GameMenu : public QWidget
{
    Q_OBJECT
public:
    explicit GameMenu(QWidget *parent = nullptr);
    QString player1VSPlayerName();
    QString player2VSPlayerName();
    QString playerVsCompName();

signals:
    void nextClicked();
public slots:

private:
    QLabel* m_title;
    QRadioButton* m_PVPmode;//m_mode
    QRadioButton* m_PVCmode;//m_model
    QPushButton* m_nextButton;

    QLineEdit *m_player1VsPlEdit;
    QLineEdit *m_player2VsPlEdit;
    QLineEdit *m_player1VsCompEdit;

    QLabel *m_player1VsPlLabel;
    QLabel *m_player2VsPlLabel;
    QLabel *m_player1VsCompLabel;

    void createWidgets();
    void createLayouts();
    void createStyleSheets();
    void createConnections();
private slots:
    void hidePvPEdits();
    void hidePvCEdits();

    void showPvPEdits();
    void showPvCEdits();

    void checkNextButtonEnabled();
};

#endif // GAMEMENU_H
