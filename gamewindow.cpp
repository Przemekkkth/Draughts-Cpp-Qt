#include "gamewindow.h"
#include <QPainter>
#include <QMessageBox>
#include <QDebug>

QColor GameWindow::blackPieceColor = QColor(238, 221, 187);
QColor GameWindow::whitePieceColor = QColor(204,136, 68);
QColor GameWindow::highlightPieceColor = QColor(0,0,0);
QColor GameWindow::rankColor = QColor(211,211,211);

GameWindow::GameWindow(QWidget *parent)
    : QWidget(parent)
{
    algorithm = new GameAlgorithm(new GameBoard(8,8,this), this);
   // setCurrentClickedPiece(QPoint(-1, -1));
    connect(this, SIGNAL(pieceClicked(QPoint)), algorithm, SLOT(setModes(QPoint)));
    connect(this, SIGNAL(pieceClicked(QPoint)), this, SLOT(update()));
    connect(this, &GameWindow::pvpClicked, algorithm, &GameAlgorithm::setPlayerVsPlayerMode);
    connect(this, &GameWindow::pvcClicked, algorithm, &GameAlgorithm::setPlayerVsComputerMode);
  //  setGameMode(PlayerVsPlayer);
   // setCurrentPlayer(Player1);

}

GameWindow::~GameWindow()
{

}

void GameWindow::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    for( int h = 0 ; h < algorithm->board()->getColumns() ; ++h )
    {
        drawBoardRow(&painter, h);
    }
    drawColumnRank(&painter);
    drawCorners(&painter);
    drawPieces(&painter);
    drawHightlight(&painter);
    update();
}

void GameWindow::drawBoardRow(QPainter *painter, int y)
{
    QColor color;
    QColor rowRankColor = rankColor;
    painter->setBrush(rowRankColor);
    painter->drawRect(0, columnRankHeight() + y*squareHeight(), rowRankWidth() , squareHeight());
    painter->drawText(QRect(0, columnRankHeight() + y*squareHeight(), rowRankWidth() , squareHeight()), Qt::AlignCenter, QString::number(y+1) );

    if(y%2)
    {
        for(int i = 0; i < algorithm->board()->getColumns(); ++i)
        {
            if(i%2)
            {
                color = blackPieceColor;
            }
            else
            {
                color = whitePieceColor;
            }
            painter->setBrush(color);
            painter->drawRect( i * squareWidth() + rowRankWidth(),
                             y * squareHeight() + columnRankHeight(),
                             squareWidth(),
                             squareHeight());

        }
    }
    else
    {
        for(int i = 0; i < algorithm->board()->getColumns() ; ++i)
        {
            if(!(i%2))
            {
                color = blackPieceColor;
            }
            else
            {
                color = whitePieceColor;
            }
            painter->setBrush(color);
            painter->drawRect( i * squareWidth() + rowRankWidth(),
                             y * squareHeight() + columnRankHeight(),
                             squareWidth(),
                             squareHeight());


        }
    }
    painter->setBrush(rowRankColor);
    painter->drawRect(rowRankWidth() + algorithm->board()->getColumns()*squareWidth(), columnRankHeight() + y*squareHeight(), rowRankWidth() , squareHeight());
    painter->drawText(QRect(rowRankWidth() + algorithm->board()->getColumns()*squareWidth(), columnRankHeight() + y*squareHeight(), rowRankWidth() , squareHeight()), Qt::AlignCenter, QString::number(y+1) );
}

void GameWindow::drawColumnRank(QPainter* painter)
{
    QColor color = rankColor;
    painter->setBrush(color);
    for(int i = 0; i < algorithm->board()->getColumns(); ++i)
    {
        painter->drawRect(rowRankWidth() + i*squareWidth(), 0, squareWidth(), columnRankHeight());
        painter->drawText(QRect(rowRankWidth() + i*squareWidth(), 0, squareWidth(), columnRankHeight()), Qt::AlignCenter,  QString(QChar('a'+i)));
    }

    for(int i = 0; i < algorithm->board()->getColumns(); ++i)
    {
        painter->drawRect(rowRankWidth() + i*squareWidth(), columnRankHeight() + algorithm->board()->getRows()*squareHeight(), squareWidth(), columnRankHeight());
        painter->drawText(QRect(rowRankWidth() + i*squareWidth(), columnRankHeight() + algorithm->board()->getRows()*squareHeight(), squareWidth(), columnRankHeight()), Qt::AlignCenter, QString(QChar('a' + i)));
    }
}

void GameWindow::drawCorners(QPainter* painter)
{
    QColor color;
    color.setRgb(90,0,0);
    painter->setBrush(color);

    painter->drawRect(0,
                      0,
                      rowRankWidth(),
                      columnRankHeight() );

    painter->drawRect(rowRankWidth() + algorithm->board()->getColumns()*squareWidth(),
                      0,
                      rowRankWidth(),
                      columnRankHeight() );

    painter->drawRect(0,
                      columnRankHeight() + algorithm->board()->getRows()*squareHeight(),
                      rowRankWidth(),
                      columnRankHeight());

    painter->drawRect(rowRankWidth() + algorithm->board()->getColumns()*squareWidth(),
                      columnRankHeight() + algorithm->board()->getRows()*squareHeight(),
                      rowRankWidth(),
                      columnRankHeight());
}

QSize GameWindow::sizeHint() const
{
    return QSize(500, 500);
}

void GameWindow::mousePressEvent(QMouseEvent* e)
{
    if(e->button() == Qt::LeftButton)
    {
         QPoint clickedPoint = e->pos();
         if( clickedPoint.x() < rowRankWidth() ||
                 clickedPoint.y() < columnRankHeight())
         {
             return;
         }

         int curPieceX = (clickedPoint.x() - rowRankWidth() ) / squareWidth();
         int curPieceY = (clickedPoint.y() - columnRankHeight() ) / squareHeight();

         if( curPieceX >= 8 || curPieceX >= 8)
             return;
        setCurrentClickedPiece(QPoint(curPieceX, curPieceY));



    }
}

void GameWindow::setCurrentClickedPiece(const QPoint&p)
{
    if(m_currentClickedPiece == p)
        return;
    m_currentClickedPiece = p;
    emit pieceClicked(m_currentClickedPiece);
}

QPoint GameWindow::currentClickedPiece() const
{
    return  m_currentClickedPiece;
}

void GameWindow::drawPieces(QPainter* painter)
{
    QPixmap piecePixmap;
    for(int r = 0; r < algorithm->board()->getRows(); r++)
    {
        for(int c = 0; c < algorithm->board()->getColumns(); c++)
        {
            if(algorithm->board()->boardData(r,c) == GameBoard::BlackPiece)
            {
                piecePixmap.load(":/img/black.svg");
                int x = r*squareWidth()+rowRankWidth();
                int y = c*squareHeight() + columnRankHeight();
                int widthPiece = squareWidth();
                int heightPiece = squareHeight();
                painter->drawPixmap(x, y, widthPiece, heightPiece, piecePixmap);
            }
            else if(algorithm->board()->boardData(r,c) == GameBoard::WhitePiece)
            {
                piecePixmap.load(":/img/white.svg");
                int x = r*squareWidth()+rowRankWidth();
                int y = c*squareHeight() + columnRankHeight();
                int widthPiece = squareWidth();
                int heightPiece = squareHeight();
                painter->drawPixmap(x, y, widthPiece, heightPiece, piecePixmap);
            }
            else if( algorithm->board()->boardData(r, c) == GameBoard::BlackQueen)
            {
                piecePixmap.load(":/img/blackQ.svg");
                int x = r*squareWidth() + rowRankWidth();
                int y = c * squareHeight() + columnRankHeight();
                int widthPiece = squareWidth();
                int heightPiece = squareHeight();
                painter->drawPixmap(x, y, widthPiece, heightPiece, piecePixmap);
            }
            else if( algorithm->board()->boardData(r, c) == GameBoard::WhiteQueen)
            {
                piecePixmap.load(":/img/whiteQ.svg");
                int x = r*squareWidth() + rowRankWidth();
                int y = c * squareHeight() + columnRankHeight();
                int widthPiece = squareWidth();
                int heightPiece = squareHeight();
                painter->drawPixmap(x, y, widthPiece, heightPiece, piecePixmap);
            }
        }
    }
}

void GameWindow::drawHightlight(QPainter* painter)
{
    QColor hightlighColor(255, 0, 0, 128);
    QColor possibleDestroyEnemy(20, 255, 22, 128);

    for(int r = 0; r < algorithm->board()->getRows(); ++r)
    {
        for(int c = 0; c < algorithm->board()->getColumns(); ++c)
        {
            if(algorithm->highlightType(r, c) == GameAlgorithm::Highlight)
            {
                painter->setBrush(QBrush(hightlighColor));
                int x = r*squareWidth()+rowRankWidth();
                int y = c*squareHeight() + columnRankHeight();
                int widthPiece = squareWidth();
                int heightPiece = squareHeight();
                painter->drawRect(x, y, widthPiece, heightPiece);
            }
            else if(algorithm->highlightType(r, c) == GameAlgorithm::PossibleDestroyEnemy )
            {
                painter->setBrush(QBrush(possibleDestroyEnemy));
                int x = r*squareWidth()+rowRankWidth();
                int y = c*squareHeight() + columnRankHeight();
                int widthPiece = squareWidth();
                int heightPiece = squareHeight();
                painter->drawRect(x, y, widthPiece, heightPiece);
            }
        }
    }
}
