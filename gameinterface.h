#ifndef GAMEINTERFACE_H
#define GAMEINTERFACE_H

#include <QWidget>
class GameMenu;
class GameMainWindow;
class QStackedWidget;
class GameInterface : public QWidget
{
    Q_OBJECT
public:
    explicit GameInterface(QWidget *parent = nullptr);

signals:

public slots:

private:
    GameMenu *m_gameMenu;
    GameMainWindow *m_gameMainWindow;
    QStackedWidget *m_mainContainer;
    void createWidgets();
    void createLayouts();
    void createStylesheets();
    void createConnections();
    void init();
protected:
    QSize sizeHint() const;
};

#endif // GAMEINTERFACE_H
