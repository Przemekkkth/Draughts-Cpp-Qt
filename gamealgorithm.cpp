#include "gamealgorithm.h"
#include <QMessageBox>
#include <QDebug>

GameAlgorithm::GameAlgorithm(GameBoard *board, QObject *parent) :  QObject(parent), m_board(board)
{
    initHightlightType();
    m_currentPlayer = Player2;
    setModeType(HightlightMode);
    setHightlightPointPiece(QPoint(-1, -1 ));
    //setGameMode(PlayerVsPlayer);
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
    else if( gameMode() == PlayerVsComputer)
    {
        if( currentPlayer() == Player1)
        {
            setBlackHighlightsType(clickedPoint);
        }
        else
        {
            QPoint p = generateRandomWhiteHighlightPiece();
            setComputerWhiteHighlightsType(p);
            setMovesType(generateRandomWhiteMovePiece(p));
        }
    }
}

void GameAlgorithm::setMovesType(QPoint clickedPoint)
{
    if( board()->boardData(clickedPoint.x(), clickedPoint.y()) == GameBoard::BlackPiece ||
        board()->boardData(clickedPoint.x(), clickedPoint.y()) == GameBoard::WhitePiece ||
        board()->boardData(clickedPoint.x(), clickedPoint.y()) == GameBoard::BlackQueen ||
        board()->boardData(clickedPoint.x(), clickedPoint.y()) == GameBoard::WhiteQueen
        )
    {
        setHighlightsType(QPoint(-1, -1));
        setHighlightsType(clickedPoint);
        setModeType(GameAlgorithm::HightlightMode);
        return;
    }

    if( highlightType( clickedPoint.x(), clickedPoint.y() ) == GameAlgorithm::Highlight )
    {

        board()->movePiece( hightlightPointPiece().x() ,
                                      hightlightPointPiece().y() ,
                                      clickedPoint.x() ,
                                      clickedPoint.y() );

       checkReplacementPieceToQueen();

    }
    else if( highlightType( clickedPoint.x(), clickedPoint.y() ) == GameAlgorithm::PossibleDestroyEnemy )
    {
        board()->movePiece( hightlightPointPiece().x() ,
                                      hightlightPointPiece().y() ,
                                      clickedPoint.x() ,
                                      clickedPoint.y() );

    QPoint oldClicked(hightlightPointPiece().x(), hightlightPointPiece().y());
    QPoint newClicked(clickedPoint.x(), clickedPoint.y());
    if(currentPlayer() == Player1){
        removeEnemiesForPlayer1(oldClicked, newClicked);
    }
    else if( currentPlayer() == Player2)
    {
        removeEnemiesForPlayer2(oldClicked, newClicked);
    }


    checkReplacementPieceToQueen();
// end new version
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
    if(gameMode() == PlayerVsComputer)
    {
        emit boardChanged(QPoint(0, 0));
    }
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


    if(board()->boardData(clickedPoint.x(), clickedPoint.y() ) == GameBoard::WhitePiece || board()->boardData(clickedPoint.x(), clickedPoint.y() ) == GameBoard::Empty || board()->boardData(clickedPoint.x(), clickedPoint.y()) == GameBoard::WhiteQueen )
    {
        return;
    }

    if(board()->boardData(clickedPoint.x() , clickedPoint.y() ) == GameBoard::BlackPiece)
    {

        int x = clickedPoint.x();
        int y = clickedPoint.y();
        setHightlightPointPiece(clickedPoint);
//Empty position on left
        if( (x-1) >= 0 && (y - 1) >= 0 && board()->boardData(x-1, y - 1) == GameBoard::Empty )
        {
            setHighlightType(x-1, y -1, GameAlgorithm::Highlight);
            setModeType(GameAlgorithm::MoveMode);
        }
//Enemies on left side of piece
        checkTopLeftEnemiesPos(x, y, GameBoard::WhitePiece);
//Empty position on right
        if( (x+1) >= 0 && (y - 1) >= 0 && board()->boardData(x + 1, y - 1) == GameBoard::Empty )
        {
            setHighlightType(x+1, y -1, GameAlgorithm::Highlight);
            setModeType(GameAlgorithm::MoveMode);
        }
//Enemies on riht side of piece
        checkTopRightEnemiesPos(x, y, GameBoard::WhitePiece);
    }

    if( board()->boardData(clickedPoint.x(), clickedPoint.y()) == GameBoard::BlackQueen)
    {
        int x = clickedPoint.x();
        int y = clickedPoint.y();
        setHightlightPointPiece(clickedPoint);
//Empty
        //top-left
        if( (x-1) >= 0 && (y - 1) >= 0 && board()->boardData(x-1, y - 1) == GameBoard::Empty )
        {
            setHighlightType(x-1, y -1, GameAlgorithm::Highlight);
            setModeType(GameAlgorithm::MoveMode);
        }
        //top-right
        if( (x+1) < board()->getColumns() && (y - 1) >= 0 && board()->boardData(x+1, y - 1) == GameBoard::Empty )
        {
            setHighlightType(x+1, y -1, GameAlgorithm::Highlight);
            setModeType(GameAlgorithm::MoveMode);
        }
        //bottom-left
        if( (x-1) >= 0 && (y + 1) < board()->getRows() && board()->boardData(x-1, y + 1) == GameBoard::Empty )
        {
            setHighlightType(x-1, y +1, GameAlgorithm::Highlight);
            setModeType(GameAlgorithm::MoveMode);
        }
        //bottom-right
        if( (x+1) < board()->getColumns() && (y + 1) <  board()->getRows() && board()->boardData(x+1, y + 1) == GameBoard::Empty )
        {
            setHighlightType(x+1, y+1, GameAlgorithm::Highlight);
            setModeType(GameAlgorithm::MoveMode);
        }
//Enemies
        checkBottomLeftEnemiesPos(x, y, GameBoard::WhitePiece);
        checkBottomRightEnemiesPos(x, y, GameBoard::WhitePiece);
        checkTopLeftEnemiesPos(x, y, GameBoard::WhitePiece);
        checkTopRightEnemiesPos(x, y, GameBoard::WhitePiece);


    }
}

void GameAlgorithm::setWhiteHighlightsType(QPoint clickedPoint)
{
    if(board()->boardData(clickedPoint.x(), clickedPoint.y() ) == GameBoard::BlackPiece || board()->boardData(clickedPoint.x(), clickedPoint.y() ) == GameBoard::Empty || board()->boardData(clickedPoint.x(), clickedPoint.y()) == GameBoard::BlackQueen)
    {
        return;
    }
    ///////////////////////////////////////////////White Piece
        if( board()->boardData(clickedPoint.x() , clickedPoint.y() ) == GameBoard::WhitePiece )
        {
            int x = clickedPoint.x();
            int y = clickedPoint.y();
            setHightlightPointPiece(clickedPoint);
//Empty position on left
            if( (x - 1) >= 0 && (y + 1) < board()->getRows() && board()->boardData(x-1, y + 1) == GameBoard::Empty)
            {

                setHighlightType(x-1, y + 1, GameAlgorithm::Highlight);
                setModeType(GameAlgorithm::MoveMode);
            }
//Enemies on left side of piece
            checkBottomLeftEnemiesPos(x, y, GameBoard::BlackPiece);
//Empty position on right
            if( (x + 1 ) >= 0 && (y + 1) < board()->getRows() && board()->boardData(x + 1, y + 1) == GameBoard::Empty )
            {
                 setHighlightType(x+1, y + 1, GameAlgorithm::Highlight);
                 setModeType(GameAlgorithm::MoveMode);
            }
//Enemies on right side of piece
            checkBottomRightEnemiesPos(x, y, GameBoard::BlackPiece);

        }

        if( board()->boardData(clickedPoint.x(), clickedPoint.y()) == GameBoard::WhiteQueen)
        {
            int x = clickedPoint.x();
            int y = clickedPoint.y();
            setHightlightPointPiece(clickedPoint);
    //Empty
            //top-left
            if( (x-1) >= 0 && (y - 1) >= 0 && board()->boardData(x-1, y - 1) == GameBoard::Empty )
            {
                setHighlightType(x-1, y -1, GameAlgorithm::Highlight);
                setModeType(GameAlgorithm::MoveMode);
            }
            //top-right
            if( (x+1) < board()->getColumns() && (y - 1) >= 0 && board()->boardData(x+1, y - 1) == GameBoard::Empty )
            {
                setHighlightType(x+1, y -1, GameAlgorithm::Highlight);
                setModeType(GameAlgorithm::MoveMode);
            }
            //bottom-left
            if( (x-1) >= 0 && (y + 1) < board()->getRows() && board()->boardData(x-1, y + 1) == GameBoard::Empty )
            {
                setHighlightType(x-1, y +1, GameAlgorithm::Highlight);
                setModeType(GameAlgorithm::MoveMode);
            }
            //bottom-right
            if( (x+1) < board()->getColumns() && (y + 1) <  board()->getRows() && board()->boardData(x+1, y + 1) == GameBoard::Empty )
            {
                setHighlightType(x+1, y+1, GameAlgorithm::Highlight);
                setModeType(GameAlgorithm::MoveMode);
            }
    //Enemies
            checkBottomLeftEnemiesPos(x, y, GameBoard::BlackPiece);
            checkBottomRightEnemiesPos(x, y, GameBoard::BlackPiece);
            checkTopLeftEnemiesPos(x, y, GameBoard::BlackPiece);
            checkTopRightEnemiesPos(x, y, GameBoard::BlackPiece);


        }
}

void GameAlgorithm::setComputerWhiteHighlightsType(QPoint clickedPoint)
{
    if(board()->boardData(clickedPoint.x(), clickedPoint.y() ) == GameBoard::BlackPiece || board()->boardData(clickedPoint.x(), clickedPoint.y() ) == GameBoard::Empty || board()->boardData(clickedPoint.x(), clickedPoint.y()) == GameBoard::BlackQueen)
    {
        return;
    }
    ///////////////////////////////////////////////White Piece
        if( board()->boardData(clickedPoint.x() , clickedPoint.y() ) == GameBoard::WhitePiece )
        {
            int x = clickedPoint.x();
            int y = clickedPoint.y();
            setHightlightPointPiece(clickedPoint);
//Empty position on left
            if( (x - 1) >= 0 && (y + 1) < board()->getRows() && board()->boardData(x-1, y + 1) == GameBoard::Empty)
            {

                setHighlightType(x-1, y + 1, GameAlgorithm::Highlight);
                setModeType(GameAlgorithm::MoveMode);
            }
//Enemies on left side of piece
            checkBottomLeftEnemiesPos(x, y, GameBoard::BlackPiece);
//Empty position on right
            if( (x + 1 ) >= 0 && (y + 1) < board()->getRows() && board()->boardData(x + 1, y + 1) == GameBoard::Empty )
            {
                 setHighlightType(x+1, y + 1, GameAlgorithm::Highlight);
                 setModeType(GameAlgorithm::MoveMode);
            }
//Enemies on right side of piece
            checkBottomRightEnemiesPos(x, y, GameBoard::BlackPiece);

        }

        if( board()->boardData(clickedPoint.x(), clickedPoint.y()) == GameBoard::WhiteQueen)
        {
            int x = clickedPoint.x();
            int y = clickedPoint.y();
            setHightlightPointPiece(clickedPoint);
    //Empty
            //top-left
            if( (x-1) >= 0 && (y - 1) >= 0 && board()->boardData(x-1, y - 1) == GameBoard::Empty )
            {
                setHighlightType(x-1, y -1, GameAlgorithm::Highlight);
                setModeType(GameAlgorithm::MoveMode);
            }
            //top-right
            if( (x+1) < board()->getColumns() && (y - 1) >= 0 && board()->boardData(x+1, y - 1) == GameBoard::Empty )
            {
                setHighlightType(x+1, y -1, GameAlgorithm::Highlight);
                setModeType(GameAlgorithm::MoveMode);
            }
            //bottom-left
            if( (x-1) >= 0 && (y + 1) < board()->getRows() && board()->boardData(x-1, y + 1) == GameBoard::Empty )
            {
                setHighlightType(x-1, y +1, GameAlgorithm::Highlight);
                setModeType(GameAlgorithm::MoveMode);
            }
            //bottom-right
            if( (x+1) < board()->getColumns() && (y + 1) <  board()->getRows() && board()->boardData(x+1, y + 1) == GameBoard::Empty )
            {
                setHighlightType(x+1, y+1, GameAlgorithm::Highlight);
                setModeType(GameAlgorithm::MoveMode);
            }
    //Enemies
            checkBottomLeftEnemiesPos(x, y, GameBoard::BlackPiece);
            checkBottomRightEnemiesPos(x, y, GameBoard::BlackPiece);
            checkTopLeftEnemiesPos(x, y, GameBoard::BlackPiece);
            checkTopRightEnemiesPos(x, y, GameBoard::BlackPiece);


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

void GameAlgorithm::checkTopLeftEnemiesPos(int xPos, int yPos, GameBoard::BoardPiece piece)
{
    GameBoard::BoardPiece queen = piece;
    if(piece == GameBoard::BlackPiece)
    {
        queen = GameBoard::BlackQueen;
    }
    else if(piece == GameBoard::WhitePiece)
    {
        queen = GameBoard::WhiteQueen;
    }


    for(int i = 1; i <= board()->getRows(); i += 2)
    {
        if( ( (xPos - i) >= 0 && (yPos - i) >= 0 && board()->boardData(xPos - i, yPos - i) == piece)
                ||
            ((xPos - i) >= 0 && (yPos - i) >= 0 && board()->boardData(xPos - i, yPos - i) == queen)
                )
        {
            if( (xPos - i - 1) >= 0 && (yPos - i - 1) >= 0 && board()->boardData(xPos - i - 1, yPos - i - 1) == GameBoard::Empty)
            {
                setHighlightType(xPos - i, yPos - i, GameAlgorithm::Enemy);
                setHighlightType(xPos - i - 1, yPos - i - 1, GameAlgorithm::PossibleDestroyEnemy);
                setModeType(GameAlgorithm::MoveMode);
                checkTopRightEnemiesPos(xPos - i - 1, yPos - i - 1, piece);
            }
            else{
                return;
            }
        }
        else
        {
            return;
        }
    }
}

void GameAlgorithm::checkTopRightEnemiesPos(int xPos, int yPos, GameBoard::BoardPiece piece)
{
    GameBoard::BoardPiece queen = piece;
    if(piece == GameBoard::BlackPiece)
    {
        queen = GameBoard::BlackQueen;
    }
    else if(piece == GameBoard::WhitePiece)
    {
        queen = GameBoard::WhiteQueen;
    }

    for(int i = 1; i <= board()->getRows() ; i+= 2)
    {
        if( ((xPos + i) < board()->getColumns() && (yPos - i) >= 0 && board()->boardData(xPos + i, yPos - i) == piece)
                ||
             ((xPos + i) < board()->getColumns() && (yPos - i) >= 0 && board()->boardData(xPos + i, yPos - i) == queen)
           )
        {
            if( (xPos + i + 1) < board()->getColumns() && (yPos - i - 1) >= 0 && board()->boardData(xPos + i + 1, yPos - i -1) == GameBoard::Empty)
            {
                setHighlightType(xPos + i, yPos - i, GameAlgorithm::Enemy);
                setHighlightType(xPos + i + 1, yPos - i - 1, GameAlgorithm::PossibleDestroyEnemy);
                setModeType(GameAlgorithm::MoveMode);
                checkTopLeftEnemiesPos(xPos + i + 1, yPos - i - 1, piece);
            }
            else{
                return;
            }
        }
        else
        {
            return;
        }
    }
}

void GameAlgorithm::checkBottomLeftEnemiesPos(int xPos, int yPos, GameBoard::BoardPiece piece)
{
    GameBoard::BoardPiece queen = piece;
    if(piece == GameBoard::BlackPiece)
    {
        queen = GameBoard::BlackQueen;
    }
    else if(piece == GameBoard::WhitePiece)
    {
        queen = GameBoard::WhiteQueen;
    }

    for(int i = 1; i <= board()->getRows(); i += 2)
    {
        if( ( (xPos - i) >= 0 && (yPos + i) < board()->getColumns() && board()->boardData(xPos -  i, yPos + i) == piece)
            ||
            ( (xPos - i) >= 0 && (yPos + i) < board()->getColumns() && board()->boardData(xPos -  i, yPos + i) == queen)
          )
        {
            if( (xPos - i - 1) >= 0 && (yPos + i + 1) < board()->getColumns() && board()->boardData(xPos - i - 1, yPos + i + 1) == GameBoard::Empty)
            {
                setHighlightType(xPos - i, yPos + i, GameAlgorithm::Enemy);
                setHighlightType(xPos - i - 1, yPos + i + 1, GameAlgorithm::PossibleDestroyEnemy);
                setModeType(GameAlgorithm::MoveMode);
                checkBottomRightEnemiesPos(xPos - i - 1, yPos + i + 1, piece);
            }
            else{
                return;
            }
        }
        else{
            return;
        }
    }
}

void GameAlgorithm::checkBottomRightEnemiesPos(int xPos, int yPos, GameBoard::BoardPiece piece)
{
    GameBoard::BoardPiece queen = piece;
    if(piece == GameBoard::BlackPiece)
    {
        queen = GameBoard::BlackQueen;
    }
    else if(piece == GameBoard::WhitePiece)
    {
        queen = GameBoard::WhiteQueen;
    }

    for(int i = 1; i <= board()->getRows(); i += 2)
    {
        if( ((xPos + i) < board()->getColumns() && (yPos + i) < board()->getColumns() && board()->boardData(xPos +  i, yPos + i) == piece)
            ||
            ((xPos + i) < board()->getColumns() && (yPos + i) < board()->getColumns() && board()->boardData(xPos +  i, yPos + i) == queen)
                )
        {
            if( (xPos + i + 1) >= 0 && (yPos + i + 1) < board()->getColumns() && board()->boardData(xPos + i + 1, yPos + i + 1) == GameBoard::Empty)
            {
                setHighlightType(xPos + i, yPos + i, GameAlgorithm::Enemy);
                setHighlightType(xPos + i + 1, yPos + i + 1, GameAlgorithm::PossibleDestroyEnemy);
                setModeType(GameAlgorithm::MoveMode);
                checkBottomLeftEnemiesPos(xPos + i + 1, yPos + i + 1, piece);
            }
            else{
                return;
            }
        }
        else{
            return;
        }
    }
}

void GameAlgorithm::removeEnemiesForPlayer1(QPoint oldClicked, QPoint newClicked)
{
    for(int i = 2 ; oldClicked != newClicked; )
    {
    if(newClicked.y() <= oldClicked.y()){
        if(newClicked.x() < oldClicked.x())
        {
//left up
            if(highlightType(oldClicked.x() - i, oldClicked.y() - i) == GameAlgorithm::PossibleDestroyEnemy){
                if(highlightType(oldClicked.x() - i + 1, oldClicked.y() - i + 1) == GameAlgorithm::Enemy)
                {
                    board()->setBoardData(oldClicked.x() - i + 1, oldClicked.y() - i + 1, GameBoard::Empty);
                    oldClicked = QPoint(oldClicked.x() - i , oldClicked.y() - i );

                }
            }

        }
        else if(newClicked.x() > oldClicked.x() )
        {
//right up
            if(highlightType(oldClicked.x() + i, oldClicked.y() - i) == GameAlgorithm::PossibleDestroyEnemy)
            {
                if(highlightType(oldClicked.x() + i - 1, oldClicked.y() - i + 1) == GameAlgorithm::Enemy)
                {
                    board()->setBoardData(oldClicked.x() + i - 1, oldClicked.y() - i + 1, GameBoard::Empty);
                    oldClicked = QPoint(oldClicked.x() + i, oldClicked.y() - i);
                }
            }
        }
        else{
//left up
                            if(highlightType(oldClicked.x() - i, oldClicked.y() - i) == GameAlgorithm::PossibleDestroyEnemy){
                                if(highlightType(oldClicked.x() - i + 1, oldClicked.y() - i + 1) == GameAlgorithm::Enemy)
                                {
                                    board()->setBoardData(oldClicked.x() - i + 1, oldClicked.y() - i + 1, GameBoard::Empty);
                                    oldClicked = QPoint(oldClicked.x() - i , oldClicked.y() - i );

                                }
                            }
//potencially right up
                            else if( highlightType(oldClicked.x() + i, oldClicked.y() - i ) == GameAlgorithm::PossibleDestroyEnemy )
                            {
                                if( highlightType(oldClicked.x() + i - 1, oldClicked.y() - i + 1) == GameAlgorithm::Enemy){
                                    board()->setBoardData(oldClicked.x() + i - 1, oldClicked.y() - i + 1, GameBoard::Empty);
                                    oldClicked = QPoint(oldClicked.x() + i, oldClicked.y() - i);
                                }
                            }
        }

     }
    else{
        if(newClicked.x() < oldClicked.x())
        {
            if(highlightType(oldClicked.x() - i, oldClicked.y() + i) == GameAlgorithm::PossibleDestroyEnemy){
                if(highlightType(oldClicked.x() - i + 1, oldClicked.y() + i - 1) == GameAlgorithm::Enemy)
                {
                    board()->setBoardData(oldClicked.x() - i + 1, oldClicked.y() + i - 1, GameBoard::Empty);
                    oldClicked = QPoint(oldClicked.x() - i , oldClicked.y() + i );
                }
            }
        }
        else if(newClicked.x() > oldClicked.x() )
        {
            if(highlightType(oldClicked.x() + i, oldClicked.y() + i) == GameAlgorithm::PossibleDestroyEnemy)
            {
                if(highlightType(oldClicked.x() + i - 1, oldClicked.y() + i - 1) == GameAlgorithm::Enemy)
                {
                    board()->setBoardData(oldClicked.x() + i - 1, oldClicked.y() + i - 1, GameBoard::Empty);
                    oldClicked = QPoint(oldClicked.x() + i, oldClicked.y() + i);
                }
            }
        }


        else{
            if(highlightType(oldClicked.x() - i, oldClicked.y() + i) == GameAlgorithm::PossibleDestroyEnemy){
                if(highlightType(oldClicked.x() - i + 1, oldClicked.y() + i - 1) == GameAlgorithm::Enemy)
                {
                    board()->setBoardData(oldClicked.x() - i + 1, oldClicked.y() + i - 1, GameBoard::Empty);
                    oldClicked = QPoint(oldClicked.x() - i , oldClicked.y() + i );
                }
                else if(highlightType(oldClicked.x() + i, oldClicked.y() + i) == GameAlgorithm::PossibleDestroyEnemy)
                {
                    if(highlightType(oldClicked.x() + i - 1, oldClicked.y() + i - 1) == GameAlgorithm::Enemy)
                    {
                        board()->setBoardData(oldClicked.x() + i - 1, oldClicked.y() + i - 1, GameBoard::Empty);
                        oldClicked = QPoint(oldClicked.x() + i, oldClicked.y() + i);
                    }
                }
            }
        }
    }

    }

}

void GameAlgorithm::removeEnemiesForPlayer2(QPoint oldClicked, QPoint newClicked)
{
    for(int i = 2 ; oldClicked != newClicked; )
    {
         if(newClicked.y() >= oldClicked.y()){
            if(newClicked.x() < oldClicked.x())
        {
            if(highlightType(oldClicked.x() - i, oldClicked.y() + i) == GameAlgorithm::PossibleDestroyEnemy){
                if(highlightType(oldClicked.x() - i + 1, oldClicked.y() + i - 1) == GameAlgorithm::Enemy)
                {
                    board()->setBoardData(oldClicked.x() - i + 1, oldClicked.y() + i - 1, GameBoard::Empty);
                    oldClicked = QPoint(oldClicked.x() - i , oldClicked.y() + i );
                }
            }
        }
            else if(newClicked.x() > oldClicked.x() )
        {
            if(highlightType(oldClicked.x() + i, oldClicked.y() + i) == GameAlgorithm::PossibleDestroyEnemy)
            {
                if(highlightType(oldClicked.x() + i - 1, oldClicked.y() + i - 1) == GameAlgorithm::Enemy)
                {
                    board()->setBoardData(oldClicked.x() + i - 1, oldClicked.y() + i - 1, GameBoard::Empty);
                    oldClicked = QPoint(oldClicked.x() + i, oldClicked.y() + i);
                }
            }
            }
            else{
            if(highlightType(oldClicked.x() - i, oldClicked.y() + i) == GameAlgorithm::PossibleDestroyEnemy){
                if(highlightType(oldClicked.x() - i + 1, oldClicked.y() + i - 1) == GameAlgorithm::Enemy)
                {
                    board()->setBoardData(oldClicked.x() - i + 1, oldClicked.y() + i - 1, GameBoard::Empty);
                    oldClicked = QPoint(oldClicked.x() - i , oldClicked.y() + i );
                }
                else if(highlightType(oldClicked.x() + i, oldClicked.y() + i) == GameAlgorithm::PossibleDestroyEnemy)
                {
                    if(highlightType(oldClicked.x() + i - 1, oldClicked.y() + i - 1) == GameAlgorithm::Enemy)
                    {
                        board()->setBoardData(oldClicked.x() + i - 1, oldClicked.y() + i - 1, GameBoard::Empty);
                        oldClicked = QPoint(oldClicked.x() + i, oldClicked.y() + i);
                    }
                }
            }
        }
        }
         else{
             if(newClicked.x() < oldClicked.x())
             {
     //left up
                 if(highlightType(oldClicked.x() - i, oldClicked.y() - i) == GameAlgorithm::PossibleDestroyEnemy){
                     if(highlightType(oldClicked.x() - i + 1, oldClicked.y() - i + 1) == GameAlgorithm::Enemy)
                     {
                         board()->setBoardData(oldClicked.x() - i + 1, oldClicked.y() - i + 1, GameBoard::Empty);
                         oldClicked = QPoint(oldClicked.x() - i , oldClicked.y() - i );

                     }
                 }

             }
             else if(newClicked.x() > oldClicked.x() )
             {
     //right up
                 if(highlightType(oldClicked.x() + i, oldClicked.y() - i) == GameAlgorithm::PossibleDestroyEnemy)
                 {
                     if(highlightType(oldClicked.x() + i - 1, oldClicked.y() - i + 1) == GameAlgorithm::Enemy)
                     {
                         board()->setBoardData(oldClicked.x() + i - 1, oldClicked.y() - i + 1, GameBoard::Empty);
                         oldClicked = QPoint(oldClicked.x() + i, oldClicked.y() - i);
                     }
                 }
             }
             else{
     //left up
                                 if(highlightType(oldClicked.x() - i, oldClicked.y() - i) == GameAlgorithm::PossibleDestroyEnemy){
                                     if(highlightType(oldClicked.x() - i + 1, oldClicked.y() - i + 1) == GameAlgorithm::Enemy)
                                     {
                                         board()->setBoardData(oldClicked.x() - i + 1, oldClicked.y() - i + 1, GameBoard::Empty);
                                         oldClicked = QPoint(oldClicked.x() - i , oldClicked.y() - i );

                                     }
                                 }
     //potencially right up
                                 else if( highlightType(oldClicked.x() + i, oldClicked.y() - i ) == GameAlgorithm::PossibleDestroyEnemy )
                                 {
                                     if( highlightType(oldClicked.x() + i - 1, oldClicked.y() - i + 1) == GameAlgorithm::Enemy){
                                         board()->setBoardData(oldClicked.x() + i - 1, oldClicked.y() - i + 1, GameBoard::Empty);
                                         oldClicked = QPoint(oldClicked.x() + i, oldClicked.y() - i);
                                     }
                                 }
             }

         }

    }
}

void GameAlgorithm::checkReplacementPieceToQueen()
{
    for(int column = 0; column < board()->getColumns(); ++column)
    {
        if( board()->boardData(column, 0) == GameBoard::BlackPiece)
        {
            board()->setBoardData(column, 0, GameBoard::BlackQueen);
        }
    }

    for(int column = 0; column < board()->getColumns(); ++column)
    {
        if( board()->boardData(column, board()->getRows() - 1) == GameBoard::WhitePiece )
        {
            board()->setBoardData(column, board()->getRows() - 1, GameBoard::WhiteQueen);
        }
    }

}

void GameAlgorithm::setPlayerVsPlayerMode()
{
    setGameMode(PlayerVsPlayer);
}

void GameAlgorithm::setPlayerVsComputerMode()
{
    setGameMode(PlayerVsComputer);
}

QPoint GameAlgorithm::generateRandomWhiteHighlightPiece()
{
    QVector<QPoint> possiblePoints;
    for(int x = 0; x < board()->getRows(); ++x)
    {
        for(int y = 0; y < board()->getColumns(); ++y)
        {
            if( board()->boardData(x, y) == GameBoard::WhitePiece)
            {
                if( (x - 1) >= 0 && (y + 1) < board()->getRows() && board()->boardData(x-1, y + 1) == GameBoard::Empty)
                {
                    possiblePoints << QPoint(x,y);
                }

                if( (x + 1 ) >= 0 && (y + 1) < board()->getRows() && board()->boardData(x + 1, y + 1) == GameBoard::Empty )
                {
                     possiblePoints << QPoint(x, y);
                }
            }
        }
    }
    int choose = qrand() % possiblePoints.size();
    setHightlightPointPiece(possiblePoints.at(choose));
    //emit boardChanged();
    return  possiblePoints.at(choose);
}

QPoint GameAlgorithm::generateRandomWhiteMovePiece(QPoint piece)
{
    QVector<QPoint> possiblePoint;
    if(board()->boardData(piece.x(), piece.y()) == GameBoard::WhitePiece)
    {
        for(int r = 0; r < board()->getRows(); ++r)
        {
            for(int c = 0; c < board()->getColumns(); ++c)
            {
                if( highlightType(r, c) == Highlight)
                {
                    possiblePoint.append(QPoint(r,c));
                }
            }
        }
    }

    int randSize = possiblePoint.size();
    return  possiblePoint.at(qrand() % randSize);
}
