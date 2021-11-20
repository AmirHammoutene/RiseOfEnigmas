/**
 * Amir Hammoutene - March 2021
 * Rise Of Enigmas
 * The principal goal is to have the control of mouse events (press, move, release) and of drawing of lines.
 * When not in Click by Click mode, we want lines to be drawn only near vertexes for the origin of the path, and that the user has to not release mouse button
 * to continue all lines, which will chain, to perform Euler Graph.
*/

#ifndef EGSCENE_H
#define EGSCENE_H

#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsLineItem>

class AdvancedPointShape;
class EdgeShape;
class Edge;

class EGScene : public QGraphicsScene
{
    Q_OBJECT
public:
    EGScene(QObject *parent = nullptr);
    ~EGScene();
    void appendPointShape(AdvancedPointShape* point); // adds an AdvancedPointShape (QGraphicsItem) each time it's created to this scene list
    void appendEdgeShape(EdgeShape* point); // adds an EdgeShape (QGraphicsLineItem) each time it's created to this scene list

    bool m_clickByClikMode;
    void resetDrawing(); // removes all lines drawn
    void clean();// cleans all items, vertexes, magneted points...
    void drawCurrentLine(QGraphicsSceneMouseEvent *event);
    uint m_currentStage; // current number of stage (level)
    static QColor s_lineColor;
    static const double s_lineWidth;
    static const QPointF s_forbiddenPoint; // this is coordinates that has not to be used for any graph, simulates null coordinates
    void sendStepUpSignal();
protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override; // overloads event of mouse pressed anywhere in the scene
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override; // overloads event of mouse move anywhere in the scene
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;  // overloads event of mouse released anywhere in the scene
private:
    QList<AdvancedPointShape*> m_pointsShapesList; // list of AdvancedPointShape (QGraphicsItem), needed to know if we are near a vertex when we press mouse button
    QList<EdgeShape *> m_edgesShapesList; // list of EdgeShape (QGraphicsLineItem), needed to know if line drawn is a correct line for the graph
    QPointF origPoint; // origin of line being drawn
    bool tryDrawing; // flag that signals that we are drawing, or trying to draw
    bool isAttached; // flag that signals that we are drawing with origin of line attached to its vertex
    QList<QGraphicsLineItem *> m_lineList; // list of lines already drawn and keep shown
    QList<Edge> m_edgesDrawnList; // list of edges already drawn and keep shown
    unsigned char m_currentLine; // index of current keeped line to draw or being drawn
    QGraphicsLineItem *tempDrawingLine; // line being drawn at the moment, has to be hide and created each time we move mouse
    QPointF findMagnetedVertex(QGraphicsSceneMouseEvent *event); // finds the vertex near we press mouse button, to have the origin of line exactly on the center of the vertex
    bool isFinishing;
    void checkAndDrawPermanentLine(QGraphicsSceneMouseEvent *event, const QPointF &reachedPoint); // draws the line permanently if correct
public slots:
    void changeLineColor(QColor color);
signals:
    void reseted();
    void stepedUp(uint step, uint total);
    void justFinished();
};

#endif // EGSCENE_H
