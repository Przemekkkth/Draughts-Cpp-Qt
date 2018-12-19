#include "gameboard.h"
#include <QDebug>

GameBoard::GameBoard(int rows, int columns, QObject *parent) : m_rows(rows), m_columns(columns), QObject(parent)
{
    initBoard();
}

void GameBoard::setRows(int newRows)
{
    if(m_rows == newRows)
        return;
    m_rows = newRows;
}

void GameBoard::setColumns(int newColumns)
{
    if(m_columns == newColumns)
        return;
    m_columns = newColumns;
}

int GameBoard::getRows() const
{
    return  m_rows;
}

int GameBoard::getColumns() const
{
    return  m_columns;
}

void GameBoard::setBoardData(int x, int y, BoardPiece p)
{
    if(p == m_boardData[x][y])
    {
        return;
    }

    m_boardData[x][y] = p;

}

GameBoard::BoardPiece GameBoard::boardData(int x, int y)
{
    return  m_boardData[x][y];
}

void GameBoard::initBoard()
{
    for(int r = 0; r < m_rows; ++r)
        for(int c = 0; c < m_columns; ++c)
            setBoardData(r,c, Empty);

    setBoardData(1, 0, WhitePiece);
    setBoardData(3, 0, WhitePiece);
    setBoardData(5, 0, WhitePiece);
    setBoardData(7, 0, WhitePiece);
    setBoardData(0, 1, WhitePiece);
    setBoardData(2, 1, WhitePiece);
    setBoardData(4, 1, WhitePiece);
    setBoardData(6, 1, WhitePiece);

    setBoardData(1, 6, BlackPiece);
    setBoardData(3, 6, BlackPiece);
    setBoardData(5, 6, BlackPiece);
    setBoardData(7, 6, BlackPiece);
    setBoardData(0, 7, BlackPiece);
    setBoardData(2, 7, BlackPiece);
    setBoardData(4, 7, BlackPiece);
    setBoardData(6, 7, BlackPiece);
}

bool GameBoard::movePiece(int fromX, int fromY, int toX, int toY)
{
    if(boardData(fromX, fromY) == Empty){
        return false;
    }

    setBoardData(toX, toY, boardData(fromX, fromY));
    setBoardData(fromX, fromY, Empty);
    return true;
}

int GameBoard::countOfWhite()
{
    int countOfWhite = 0;
    for(int r = 0; r < m_rows; ++r)
        for(int c = 0; c < m_columns; ++c)
            if( boardData(r, c) == WhitePiece || boardData(r, c) == WhiteQueen )
                countOfWhite++;
    return countOfWhite;
}

int GameBoard::countOfBlack()
{
    int countOfBlack = 0;
    for(int r = 0; r < m_rows; ++r)
        for(int c = 0; c < m_columns; ++c)
            if( boardData(r, c) == BlackPiece || boardData(r, c) == BlackQueen)
                countOfBlack++;
    return countOfBlack;
}

void GameBoard::debugBoard()
{
    for(int r = 0; r < m_rows; ++r)
    {
        QString line = "";
        for(int c = 0; c < m_columns; ++c)
        {

            if(boardData(r, c) == BlackPiece)
            {
                line += "(" + QString::number(r) + ", " + QString::number(c) + " ) " + "BP";
            }
            else if(boardData(r, c) == WhitePiece)
            {
                line += "(" + QString::number(r) + ", " + QString::number(c) + " ) " + "WP";
            }
            else if( boardData(r, c) == BlackQueen)
            {
                line += "(" + QString::number(r) + ", " + QString::number(c) + " ) " + "BQ";
            }
            else if( boardData(r, c) == WhiteQueen)
            {
                line += "(" + QString::number(r) + ", " + QString::number(c) + " ) " + "WQ";
            }
            else
            {
                line += "(" + QString::number(r) + ", " + QString::number(c) + " ) " + "EE";
            }
        }
        qDebug() << line;
    }
}
