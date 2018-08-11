#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QWidget>
#include "gamealgorithm.h"
#include <QPaintEvent>
#include <QMouseEvent>

class GameWindow : public QWidget
{
    Q_OBJECT

public:
    enum GameMode{PlayerVsPlayer, PlayerVsComputer};
    GameMode gameMode() const;
    void setGameMode(GameMode mode);

    GameWindow(QWidget *parent = nullptr);
    ~GameWindow();
    int rowRankWidth()
    {
        return (contentsRect().width()/algorithm->board()->getRows())/2 ;
    }
    int columnRankHeight()
    {
        return (contentsRect().height()/algorithm->board()->getColumns())/2;
    }

    int squareWidth() { return contentsRect().width() / algorithm->board()->getRows() - (2*rowRankWidth()/algorithm->board()->getRows()); }
    int squareHeight() { return contentsRect().height() / algorithm->board()->getColumns() - (2*columnRankHeight()/algorithm->board()->getColumns()); }

    void drawBoardRow(QPainter* painter, int y);
    void drawColumnRank(QPainter* painter);
    void drawCorners(QPainter* painter);
    void drawPieces(QPainter* painter);
    void drawHightlight(QPainter* painter);

    void setCurrentClickedPiece(const QPoint&);
    QPoint currentClickedPiece() const;

    GameAlgorithm* getAlgorithm() const
    {
        return algorithm;
    }
private:
    GameAlgorithm *algorithm;
    QPoint m_currentClickedPiece;
    GameMode m_gameMode;

    static QColor blackPieceColor;
    static QColor whitePieceColor;
    static QColor highlightPieceColor;
    static QColor rankColor;

    void setBlackHighlightsType(QPoint clickedPoint);
    void setWhiteHighlightsType(QPoint clickedPoint);
protected:
    void paintEvent(QPaintEvent*);
    QSize sizeHint() const;
    void mousePressEvent(QMouseEvent*);
signals:
    void pieceClicked(QPoint);

};

#endif // GAMEWINDOW_H
