#include "egscene.h"
#include "advancedpointshape.h"
#include "edgeshape.h"
#include "edge.h"
#include "vertex.h"
#include <QGraphicsView>

QColor EGScene::s_lineColor =  QColor( Qt::GlobalColor::darkBlue ) ;
const qreal EGScene::s_lineWidth = 9. ;
const QPointF EGScene::s_forbiddenPoint = QPointF(-52.365 , -52.365) ;

EGScene::EGScene(QObject * parent):QGraphicsScene(parent)
{
    setSceneRect(-350,-350,700,700);
    tryDrawing = false;
    isAttached = false;
    m_currentLine = 0;
    tempDrawingLine = nullptr;
    isFinishing = false;
    m_clickByClikMode = false;
}

EGScene::~EGScene()
{
    qDeleteAll(m_lineList);
    m_lineList.clear();
    if( tempDrawingLine!= nullptr)
        delete tempDrawingLine;
}

void EGScene::appendPointShape(AdvancedPointShape *point)
{
    m_pointsShapesList.append(point);
}

void EGScene::appendEdgeShape(EdgeShape *point)
{
    m_edgesShapesList.append(point);
}

void EGScene::resetDrawing()
{
    qDeleteAll(m_lineList);
    m_lineList.clear();
    m_edgesDrawnList.clear();
    if( tempDrawingLine!= nullptr)
    {
        delete tempDrawingLine;
        tempDrawingLine = nullptr;
    }
    m_currentLine = 0;
    emit reseted();
}

void EGScene::clean()
{
    resetDrawing();
    qDeleteAll(m_pointsShapesList);
    m_pointsShapesList.clear();
    qDeleteAll(m_edgesShapesList);
    m_edgesShapesList.clear();
    tryDrawing = false;
    isAttached = false;
    isFinishing = false;
    for(QGraphicsItem * it : items())
        removeItem(it);
}

void EGScene::drawCurrentLine(QGraphicsSceneMouseEvent *event)
{
    // refreshing the drawing by hiding and recreating line with new position
    if(tempDrawingLine!= nullptr)
        tempDrawingLine->setVisible(false);
    tempDrawingLine = addLine(origPoint.x(),origPoint.y(), event->scenePos().x(), event->scenePos().y(),QPen(s_lineColor,s_lineWidth));
}

void EGScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(isFinishing)
        return;

    if(!m_clickByClikMode)
    {
        tryDrawing = true;
        isAttached = false;
    }
    else
    {
        if(event->button() == Qt::RightButton)
        {
            resetDrawing();
            emit stepedUp(m_currentLine, m_edgesShapesList.size());
            isAttached = false;
            return;
        }
        QPointF computedMagnetedVertex = findMagnetedVertex(event);
        if(computedMagnetedVertex != s_forbiddenPoint)// we are near a vertex
        {
            if( !isAttached )
            {
                origPoint = computedMagnetedVertex;
                isAttached = true;
            }
            if( computedMagnetedVertex != origPoint)
                checkAndDrawPermanentLine(event,computedMagnetedVertex);
            else
                drawCurrentLine(event);
        }
        else
        {
            if(isAttached )
                drawCurrentLine(event);
        }
    }
    QGraphicsScene::mousePressEvent(event);
}

void EGScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(isFinishing)
        return;
    if(!m_clickByClikMode)
    {
        if (tryDrawing) // we pressed the mouse button, and we keeped the mouse button pressed while moving
        {
            QPointF computedMagnetedVertex = findMagnetedVertex(event);
            if(!isAttached) // previously, we were not drawing a line
            {
                if(computedMagnetedVertex != s_forbiddenPoint) // we are near a vertex
                {
                    origPoint = computedMagnetedVertex; // sets the origin of line as the center of vertex near we clicked
                    isAttached = true;
                }
            }
            else // previously, we were drawing a line
            {
                if( computedMagnetedVertex == origPoint || computedMagnetedVertex == s_forbiddenPoint)
                { // we are near the vertex we beginned the line, or ouside of if, but not on another vertex
                    // refreshing the drawing by hiding and recreating line with new position
                    drawCurrentLine(event);
                }
                else // we reached another vertex
                    checkAndDrawPermanentLine(event, computedMagnetedVertex);
            }
        }
    }
    else
    {
        if(isAttached)
            drawCurrentLine(event);
    }
    QGraphicsScene::mouseMoveEvent(event);
}

void EGScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(isFinishing)
        return;
    if(!m_clickByClikMode)
    {
        tryDrawing = false;
        isAttached = false;
        resetDrawing();
        emit stepedUp(m_currentLine, m_edgesShapesList.size());
    }
    QGraphicsScene::mouseReleaseEvent(event);
}

QPointF EGScene::findMagnetedVertex(QGraphicsSceneMouseEvent *event)
{
    for(int i = 0; i < m_pointsShapesList.size(); i++)
    {
        AdvancedPointShape *point = m_pointsShapesList.at(i);
        if(point->contains(point->mapFromScene(event->scenePos())))
            return QPointF(point->m_x,point->m_y);
    }
    return s_forbiddenPoint;
}

void EGScene::checkAndDrawPermanentLine(QGraphicsSceneMouseEvent *event, const QPointF &reachedPoint)
{
    bool edgeFound = false;
    bool edgeAlreadyDrawn = false;
    Edge thisEdge( Vertex(origPoint.x(),origPoint.y()), Vertex(reachedPoint.x(), reachedPoint.y()) );
    for( int i = 0; i<m_edgesShapesList.size() ; i++ )
    {// looks if the edge we draw has to be drawn
        auto aEdgeShape = m_edgesShapesList.at(i);
        Edge aEdge( Vertex(aEdgeShape->m_Ox,aEdgeShape->m_Oy), Vertex(aEdgeShape->m_Ex,aEdgeShape->m_Ey) );
        if(aEdge == thisEdge)
        {
            edgeFound = true;
            break;
        }
    }

    for( int i = 0; i<m_edgesDrawnList.size() ; i++ )
    {// looks if the edge we draw has already been drawn (forbidden in Euler Graph rules)
        if(m_edgesDrawnList.at(i) == thisEdge)
        {
            edgeAlreadyDrawn = true;
            break;
        }
    }

    if(!edgeFound || edgeAlreadyDrawn)
    { // if the edge does not respect requirements, we continue drawing current line
        drawCurrentLine(event);
    }
    else
    { // we permanently draw the line and set another origin point for a new line
        QGraphicsLineItem *permanentLine = addLine(origPoint.x(),origPoint.y(), reachedPoint.x(), reachedPoint.y(),QPen(s_lineColor,s_lineWidth));
        m_lineList.append(permanentLine);
        m_edgesDrawnList.append( Edge( Vertex(origPoint.x(),origPoint.y()) , Vertex(reachedPoint.x(), reachedPoint.y())) );
        m_currentLine++;
        origPoint = reachedPoint;
    }

    emit stepedUp(m_currentLine, m_edgesShapesList.size());
    if( m_currentLine >= m_edgesShapesList.size())
    {
        isFinishing = true;
        emit justFinished();
    }

}

void EGScene::sendStepUpSignal()
{
    emit stepedUp(m_currentLine, m_edgesShapesList.size());
}

void EGScene::changeLineColor(QColor color)
{
    s_lineColor = color;
}
