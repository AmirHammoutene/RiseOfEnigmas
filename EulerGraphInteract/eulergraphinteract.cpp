#include "eulergraphinteract.h"

EulerGraphInteract::EulerGraphInteract(QWidget *parent)
    : QGraphicsView(parent)
{
    setScene(&m_scene);
    setResizeAnchor(QGraphicsView::AnchorViewCenter);
    screenScaleFactor = 1.;
    setScale(screenScaleFactor);
    setFrameStyle(0);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff );
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff );
    setMouseTracking(true);
    viewport()->setMouseTracking(true);
    m_scaleAnimation = nullptr;

    QObject::connect(&m_scene, SIGNAL( justFinished() ), this, SLOT( sceneHasFinished() ) );
}

EulerGraphInteract::~EulerGraphInteract()
{
    m_scene.clean();
}

qreal EulerGraphInteract::scale() const
{
    return m_scale;
}

void EulerGraphInteract::setScale(const qreal &scale)
{
    // First reset
    QTransform identity;
    identity.reset();
    setTransform(identity);

    // Then apply scale
    m_scale = scale;
    QTransform matrix = transform();
    matrix.scale(m_scale, m_scale);
    setTransform(matrix);
}

void EulerGraphInteract::setGraph(uint stage, const QPair<const QList<Vertex> &, const QList<Edge> &> &vertexesAndEdges)
{
    if(!m_scaleAnimation.isNull())
    {
        m_scaleAnimation->stop();
        setMouseTracking(true);
        viewport()->setMouseTracking(true);
        delete m_scaleAnimation;
        m_scaleAnimation = nullptr;
    }
    m_scene.clean();
    setScale(screenScaleFactor);
    if(stage == 0) // Stage begins at 1
        return;

    m_scene.m_currentStage = stage;
    m_vertexesList = vertexesAndEdges.first;
    m_edgesList = vertexesAndEdges.second;

    renderEdges();
    renderVertexes();
}

void EulerGraphInteract::renderEdges()
{
    for(Edge &edg : m_edgesList)
    {
        EdgeShape *currentEdge = new EdgeShape(&m_scene,edg);
        m_scene.addItem(currentEdge);
    }
}

void EulerGraphInteract::renderVertexes()
{
    for(Vertex &ver : m_vertexesList)
    {
        AdvancedPointShape *currentPoint = new AdvancedPointShape(&m_scene,ver);
        m_scene.addItem(currentPoint);
    }
}

void EulerGraphInteract::sceneSendStepUpSignal()
{
    m_scene.sendStepUpSignal();
}

void EulerGraphInteract::scaleToScreen()
{
    screenScaleFactor = property("heightFactor").toReal();
    setScale(screenScaleFactor);
}


void EulerGraphInteract::sceneHasFinished()
{
    setMouseTracking(false);
    viewport()->setMouseTracking(false);
    m_scaleAnimation = new QPropertyAnimation(this, "scale", this);
    m_scaleAnimation->setDuration(3500);
    m_scaleAnimation->setEndValue(s_scaleAtAnimEnd);
    m_scaleAnimation->start();
    QObject::connect(m_scaleAnimation, SIGNAL( finished()),this , SLOT( afterFinishedAnim() ));
}

void EulerGraphInteract::afterFinishedAnim()
{
    m_scene.clean();
    setScale(screenScaleFactor);
    if(!m_scaleAnimation.isNull())
    {
        delete m_scaleAnimation;
        m_scaleAnimation = nullptr;
    }
    setMouseTracking(true);
    viewport()->setMouseTracking(true);
    emit fullFinishedGraph();
}
