#include "gamealgorithm.h"
#include <QMessageBox>
#include <QDebug>
GameAlgorithm::GameAlgorithm(GameBoard *board, QObject *parent) :  QObject(parent), m_board(board)
{
    initHightlightType();
    m_currentPlayer = Player2;
    setModeType(HightlightMode);
    setHightlightPointPiece(QPoint(-1, -1 ));
    setGameMode(PlayerVsPlayer);
    setCurrentPlayer(Player1);
}

GameBoard* GameAlgorithm::board() const
{
    return m_board;
}

void GameAlgorithm::setBoard(GameBoard* board)
{
    if(m_board == board)
        return;
    m_board = board;
}

void GameAlgorithm::initHightlightType()
{
    for(int r = 0; r < 8; ++r)
    {
        for(int c = 0; c < 8; ++c)
        {
            setHighlightType(r,c, NoHightlight);
        }
    }
}

void GameAlgorithm::setHighlightType(int x, int y, HightlightType highlightType)
{
    if(m_hightlightBoard[x][y] == highlightType)
    {
        return;
    }
    m_hightlightBoard[x][y] = highlightType;
}


GameAlgorithm::HightlightType GameAlgorithm::highlightType(int x, int y) const
{
    return m_hightlightBoard[x][y];
}

void GameAlgorithm::setModeType(ModeType type)
{
    m_type = type;
}

GameAlgorithm::ModeType GameAlgorithm::getModeType() const
{
    return m_type;
}

void GameAlgorithm::setHightlightPointPiece(const QPoint& p)
{
    m_hightlightPointPiece = p;
}

QPoint GameAlgorithm::hightlightPointPiece() const
{
    return  m_hightlightPointPiece;
}

void GameAlgorithm::restart()
{
    initHightlightType();
    setModeType(HightlightMode);
    setHightlightPointPiece(QPoint(-1, -1 ));
    setGameMode(PlayerVsPlayer);
    setCurrentPlayer(Player1);
    board()->initBoard();
}

void GameAlgorithm::setHighlightsType(QPoint clickedPoint)
{
    initHightlightType();
    //setWhiteHighlightsType(clickedPoint);
    //setBlackHighlightsType(clickedPoint);
    if( gameMode() == PlayerVsPlayer)
    {



     ///////////////////////////////////////////////Black Piece
         if( currentPlayer() == Player1 )
         {
             setBlackHighlightsType(clickedPoint);
         }
     ///////////////////////////////////////////////White Piece
         else
         {
             setWhiteHighlightsType(clickedPoint);
         }

    }
}

void GameAlgorithm::setMovesType(QPoint clickedPoint)
{
    if( highlightType( clickedPoint.x(), clickedPoint.y() ) == GameAlgorithm::Highlight )
    {
        board()->movePiece( hightlightPointPiece().x() ,
                                      hightlightPointPiece().y() ,
                                      clickedPoint.x() ,
                                      clickedPoint.y() );
    }
    else if( highlightType( clickedPoint.x(), clickedPoint.y() ) == GameAlgorithm::PossibleDestroyEnemy )
    {
        board()->movePiece( hightlightPointPiece().x() ,
                                      hightlightPointPiece().y() ,
                                      clickedPoint.x() ,
                                      clickedPoint.y() );

        for(int row = 0; row < board()->getRows(); ++row)
        {
            for(int column = 0; column < board()->getColumns(); ++column)
            {
                if( highlightType(row, column) == GameAlgorithm::Enemy )
                {
                    board()->setBoardData(row, column, GameBoard::Empty);

                }
            }
        }

        emit countOfBlackPiecesChanged( board()->countOfBlack() );
        emit countOfWhitePiecesChanged( board()->countOfWhite() );

    }
    else if(
            board()->boardData( clickedPoint.x() , clickedPoint.y() ) == GameBoard::WhitePiece ||
            board()->boardData( clickedPoint.x() , clickedPoint.y() ) == GameBoard::BlackPiece
            )
    {
        initHightlightType();
        setModeType(GameAlgorithm::HightlightMode);
        setHighlightsType(clickedPoint);
        return;

    }
    else if(board()->boardData( clickedPoint.x() , clickedPoint.y() ) == GameBoard::Empty)
    {
        return;
    }



    //reset

    checkWinCondition();

    Player currentP = currentPlayer();
    setCurrentPlayer( currentP == Player1 ? Player2 : Player1 );

    setModeType(GameAlgorithm::HightlightMode);
    initHightlightType();

}

void GameAlgorithm::setModes(QPoint clickedPoint)
{
    if(getModeType() == GameAlgorithm::HightlightMode)
    {
        setHighlightsType(clickedPoint);
    }
    else
    {
        setMovesType(clickedPoint);
    }
}

GameAlgorithm::GameMode GameAlgorithm::gameMode() const{
    return m_gameMode;
}


void GameAlgorithm::setGameMode(GameMode mode)
{
    if(m_gameMode == mode)
        return;

    m_gameMode = mode;
}

void GameAlgorithm::setBlackHighlightsType(QPoint clickedPoint)
{
    if(board()->boardData(clickedPoint.x(), clickedPoint.y() ) == GameBoard::WhitePiece || board()->boardData(clickedPoint.x(), clickedPoint.y() ) == GameBoard::Empty)
    {

        return;
    }

    if(board()->boardData(clickedPoint.x() , clickedPoint.y() ) == GameBoard::BlackPiece)
    {

        int x = clickedPoint.x();
        int y = clickedPoint.y();
        setHightlightPointPiece(clickedPoint);
//Empty left piece
        if( (x-1) >= 0 && (y - 1) >= 0 && board()->boardData(x-1, y - 1) == GameBoard::Empty )
        {
            setHighlightType(x-1, y -1, GameAlgorithm::Highlight);
            setModeType(GameAlgorithm::MoveMode);
        }
//Enemy left piece
        if( (x-1) >= 0 && (y - 1) >= 0 && board()->boardData(x-1, y - 1) == GameBoard::WhitePiece )
        {
            if( (x-2) >= 0 && (y - 2) >= 0 && board()->boardData(x-2, y - 2) == GameBoard::Empty )
            {
                setHighlightType( x-1, y-1, GameAlgorithm::Enemy);
                setHighlightType(x - 2, y - 2, GameAlgorithm::PossibleDestroyEnemy);


//2 X Enemy left piece
                if( (x-3) >= 0 && (y-3) >= 0 && board()->boardData(x-3, y - 3) == GameBoard::WhitePiece)
                {
                    if( (x-4) >= 0 && (y-4) >= 0 && board()->boardData(x-4, y-4) == GameBoard::Empty)
                    {
                        setHighlightType(x-3, y-3, GameAlgorithm::Enemy);
                        setHighlightType(x-4, y-4, GameAlgorithm::PossibleDestroyEnemy);
//3 X Enemy left piece
                        if( (x-5) >= 0 && (y-5) >= 0 && board()->boardData(x-5, y-5) == GameBoard::WhitePiece)
                        {
                            if( (x-6) >= 0 && (y-6) >= 0 && board()->boardData(x-6, y-6) == GameBoard::Empty)
                            {
                                setHighlightType(x-5, y-5, GameAlgorithm::Enemy);
                                setHighlightType(x-6, y-6, GameAlgorithm::PossibleDestroyEnemy);
//4 X Enemy left piece
                                if( (x-7) >= 0 && (y-7) >= 0 && board()->boardData(x-7, y-7) == GameBoard::WhitePiece)
                                {
                                    if( (x-8) >= 0 && (y-8) >= 0 && board()->boardData(x-8, y-8) == GameBoard::Empty)
                                    {
                                        setHighlightType(x-7, y-7, GameAlgorithm::Enemy);
                                        setHighlightType(x-8, y-8, GameAlgorithm::PossibleDestroyEnemy);
                                    }
                                }
                            }
                        }
                    }
                }




                setModeType(GameAlgorithm::MoveMode);
            }
        }
//Empty right piece
        if( (x+1) >= 0 && (y - 1) >= 0 && board()->boardData(x + 1, y - 1) == GameBoard::Empty )
        {
            setHighlightType(x+1, y -1, GameAlgorithm::Highlight);
            setModeType(GameAlgorithm::MoveMode);
        }
//Enemy right piece
        if( (x+1) >= 0 && (y - 1) >= 0 && board()->boardData(x + 1, y - 1) == GameBoard::WhitePiece)
        {
            if( ( x+2 ) >= 0 && ( y-2 ) >= 0 && board()->boardData( x+2, y-2 ) == GameBoard::Empty )
            {
                setHighlightType( x+1, y-1, GameAlgorithm::Enemy);
                setHighlightType( x+2, y-2, GameAlgorithm::PossibleDestroyEnemy);
                setModeType(GameAlgorithm::MoveMode);
            }
        }
    }
}

void GameAlgorithm::setWhiteHighlightsType(QPoint clickedPoint)
{
    if(board()->boardData(clickedPoint.x(), clickedPoint.y() ) == GameBoard::BlackPiece || board()->boardData(clickedPoint.x(), clickedPoint.y() ) == GameBoard::Empty)
    {

        return;
    }

    ///////////////////////////////////////////////White Piece
        if( board()->boardData(clickedPoint.x() , clickedPoint.y() ) == GameBoard::WhitePiece )
        {
            int x = clickedPoint.x();
            int y = clickedPoint.y();
            setHightlightPointPiece(clickedPoint);
    //Empty left piece
            if( (x - 1) >= 0 && (y + 1) < board()->getRows() && board()->boardData(x-1, y + 1) == GameBoard::Empty)
            {

                setHighlightType(x-1, y + 1, GameAlgorithm::Highlight);
                setModeType(GameAlgorithm::MoveMode);
            }
    //Enemy left piece
            if( (x - 1) >= 0 && (y + 1) < board()->getRows() && board()->boardData(x-1, y + 1) == GameBoard::BlackPiece)
            {
                if( (x-2) >= 0 && (y + 2)  < board()->getRows() && board()->boardData(x-2, y + 2) == GameBoard::Empty )
                {
                    setHighlightType(x - 1, y + 1, GameAlgorithm::Enemy);
                    setHighlightType(x - 2, y + 2, GameAlgorithm::PossibleDestroyEnemy);
                    setModeType(GameAlgorithm::MoveMode);
                }
            }
    //Empty right piece
            if( (x + 1 ) >= 0 && (y + 1) < board()->getRows() && board()->boardData(x + 1, y + 1) == GameBoard::Empty )
            {
                 setHighlightType(x+1, y + 1, GameAlgorithm::Highlight);
                 setModeType(GameAlgorithm::MoveMode);
            }
    //Enemy right piece
            if( (x + 1 ) >= 0 && (y + 1) < board()->getRows() && board()->boardData(x + 1, y + 1) == GameBoard::BlackPiece )
            {
                if( (x+2) >= 0 && (y + 2)  < board()->getRows() && board()->boardData(x+2, y + 2) == GameBoard::Empty )
                {
                    setHighlightType(x + 1, y + 1, GameAlgorithm::Enemy);
                    setHighlightType(x + 2, y + 2, GameAlgorithm::PossibleDestroyEnemy);
                    setModeType(GameAlgorithm::MoveMode);
                }
            }
        }


}

void GameAlgorithm::setCurrentPlayer(Player p)
{
    if(m_currentPlayer == p)
        return;
    m_currentPlayer = p;

    emit currentPlayerChanged(p);
}

GameAlgorithm::Player GameAlgorithm::currentPlayer() const
{
    return  m_currentPlayer;
}

void GameAlgorithm::checkWinCondition()
{
    if( !board()->countOfWhite())
    {
        emit playerWins( Player1 );
    }
    else if( !board()->countOfBlack() )
    {
        emit playerWins( Player2 );
    }
}
