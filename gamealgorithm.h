#ifndef GAMEALGORITHM_H
#define GAMEALGORITHM_H

#include "gameboard.h"

#include <QObject>
#include <QPointF>


class GameAlgorithm : public QObject
{
    Q_OBJECT
public:
    explicit GameAlgorithm(GameBoard *board, QObject *parent);

    GameBoard* board() const;
    void setBoard(GameBoard* board);

    enum HightlightType {NoHightlight, Highlight, PossibleDestroyEnemy, Enemy};
    void setHighlightType(int x, int y, HightlightType h);
    HightlightType highlightType(int x, int y) const ;

    enum ModeType{HightlightMode, MoveMode};
    void setModeType(ModeType t);
    ModeType getModeType() const;


    void initHightlightType();
    void restart();

    enum Player{Player1, Player2, Computer};
    void setCurrentPlayer(Player p);
    Player currentPlayer() const;


    void checkWinCondition();



    void setHightlightPointPiece(const QPoint&);
    QPoint hightlightPointPiece() const;

    enum GameMode{PlayerVsPlayer, PlayerVsComputer};
    GameMode gameMode() const;
    void setGameMode(GameMode mode);
private:
    void setBlackHighlightsType(QPoint clickedPoint);
    void setWhiteHighlightsType(QPoint clickedPoint);

    GameBoard* m_board;
    QPoint m_hightlightPointPiece;

    GameMode m_gameMode;
    Player m_currentPlayer;    
    HightlightType m_hightlightBoard[8][8];
    ModeType m_type;
private slots:
    void setHighlightsType(QPoint clickedPoint);
    void setMovesType(QPoint clickedPoint);
    void setModes(QPoint clickedPoint);
signals:
    void countOfBlackPiecesChanged(int);
    void countOfWhitePiecesChanged(int);
    void currentPlayerChanged(Player player);
    void playerWins(Player player);
};

#endif // GAMEALGORITHM_H
