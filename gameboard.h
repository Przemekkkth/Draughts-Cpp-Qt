#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <QObject>

class GameBoard : public QObject
{
    Q_OBJECT
public:
    explicit GameBoard(int rows, int columns, QObject *parents);

    void setRows(int newRows);
    void setColumns(int newColumns);

    int getRows() const;
    int getColumns() const;
    int countOfWhite();
    int countOfBlack();
    enum BoardPiece{WhitePiece, BlackPiece, WhiteQueen, BlackQueen, Empty};
    void initBoard();
    void setBoardData(int x, int y, BoardPiece p);
    BoardPiece boardData(int x, int y);
    bool movePiece(int fromX, int fromY, int toX, int toY);

    void debugBoard();
signals:

public slots:

private:
    int m_rows;
    int m_columns;
    BoardPiece m_boardData[8][8];

};

#endif // GAMEOBJECT_H
