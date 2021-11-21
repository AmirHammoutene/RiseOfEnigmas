/**
 * Amir Hammoutene - March 2021
 * Rise Of Enigmas
 * Eulerian Graphs module
**/

#ifndef EULERGRAPHINTERACT_H
#define EULERGRAPHINTERACT_H

#include <QGraphicsView>
#include "egscene.h"
#include <QList>
#include <QPointer>
#include <QPropertyAnimation>
#include "vertex.h"
#include "edge.h"
#include "advancedpointshape.h"
#include "edgeshape.h"

class EulerGraphInteract : public QGraphicsView
{
    Q_OBJECT
    Q_PROPERTY(qreal scale READ scale WRITE setScale)
public:
    EulerGraphInteract(QWidget *parent);
    ~EulerGraphInteract();
    EGScene m_scene;
    QList<Vertex> m_vertexesList;
    QList<Edge> m_edgesList;
    void setGraph(uint stage, const QPair<const QList<Vertex>&, const QList<Edge>& > &vertexesAndEdges);
    qreal m_scale;
    qreal scale() const;
    void setScale(const qreal &scale);
    QPointer<QPropertyAnimation> m_scaleAnimation;
    const qreal s_scaleAtAnimEnd = 5.;
    void sceneSendStepUpSignal();
private:
    void renderEdges();
    void renderVertexes();
public slots:
    void sceneHasFinished();
    void afterFinishedAnim();
signals:
    void fullFinishedGraph();
};

#endif // EULERGRAPHINTERACT_H
