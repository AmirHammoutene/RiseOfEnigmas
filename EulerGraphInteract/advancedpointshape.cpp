#include "advancedpointshape.h"

QColor AdvancedPointShape::s_defaultColor =  QColor( Qt::GlobalColor::black ) ;
double AdvancedPointShape::s_centralDiameter = 20. ;
double AdvancedPointShape::s_ringDiameter = 30. ;
double AdvancedPointShape::s_maxDiameter = 30. ;
QColor AdvancedPointShape::s_colorAfterAnim = QColor( 70,85,150 ) ;
qreal AdvancedPointShape::s_scaleAfterAnim = 1.3 ;

AdvancedPointShape::AdvancedPointShape(EGScene *scene, Vertex vertex):QObject(),QGraphicsItem(),m_x(vertex.m_x),
    m_y(vertex.m_y),m_scene(scene)
{
    setAcceptHoverEvents(true);
    if(m_scene != nullptr)
    {
        m_centralPoint = QSharedPointer<QGraphicsEllipseItem>( m_scene->addEllipse(m_x-s_centralDiameter/2.,m_y-s_centralDiameter/2.,
                                                         s_centralDiameter,s_centralDiameter,QPen(s_defaultColor),QBrush(s_defaultColor)) );
        m_ring = QSharedPointer<QGraphicsEllipseItem>( m_scene->addEllipse(m_x-s_ringDiameter/2.,m_y-s_ringDiameter/2.,
                                                         s_ringDiameter,s_ringDiameter,QPen(s_defaultColor),QBrush()) );
        m_scene->appendPointShape(this);
    }
    setScale(1.);
    setColor(s_defaultColor);
}

AdvancedPointShape::~AdvancedPointShape()
{
}

bool AdvancedPointShape::operator==(const AdvancedPointShape &other) const
{
    if( other.m_x != m_x || other.m_y != m_y)
        return false;

    return true;
}

bool AdvancedPointShape::operator!=(const AdvancedPointShape &other) const
{
    return !( (*this) == other );
}

QColor AdvancedPointShape::color() const
{
    return m_color;
}

void AdvancedPointShape::setColor(const QColor &color)
{
    m_color = color;
    m_centralPoint->setPen(QPen(m_color));
    m_centralPoint->setBrush(QBrush(m_color));
    m_ring->setPen(QPen(m_color));
}

qreal AdvancedPointShape::scale() const
{
    return m_scale;
}

void AdvancedPointShape::setScale(const qreal &scale)
{
    m_scale = scale;
    m_ring->setTransformOriginPoint(m_x,m_y);
    m_ring->setScale(m_scale);
}

QRectF AdvancedPointShape::boundingRect() const
{
    return QRectF(m_x-s_maxDiameter/2.,m_y-s_maxDiameter/2.,s_maxDiameter,s_maxDiameter);
}

void AdvancedPointShape::paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *)
{
}

void AdvancedPointShape::hoverEnterEvent(QGraphicsSceneHoverEvent *ev)
{
    m_colorAnimation = new QPropertyAnimation(this, "color", this);
    m_colorAnimation->setDuration(1000);
    m_colorAnimation->setEndValue(s_colorAfterAnim);
    m_colorAnimation->start();
    QObject::connect(m_colorAnimation, SIGNAL( finished()), m_colorAnimation, SLOT(deleteLater()));

    m_scaleAnimation = new QPropertyAnimation(this, "scale", this);
    m_scaleAnimation->setDuration(1000);
    m_scaleAnimation->setEndValue(s_scaleAfterAnim);
    m_scaleAnimation->start();
    QObject::connect(m_scaleAnimation, SIGNAL( finished()), m_scaleAnimation, SLOT(deleteLater()));

    QGraphicsItem::hoverEnterEvent(ev);
}

void AdvancedPointShape::hoverLeaveEvent(QGraphicsSceneHoverEvent *ev)
{
    m_colorAnimation = new QPropertyAnimation(this, "color", this);
    m_colorAnimation->setDuration(1000);
    m_colorAnimation->setEndValue(s_defaultColor);
    m_colorAnimation->start();
    QObject::connect(m_colorAnimation, SIGNAL( finished()), m_colorAnimation, SLOT(deleteLater()));

    m_scaleAnimation = new QPropertyAnimation(this, "scale", this);
    m_scaleAnimation->setDuration(1000);
    m_scaleAnimation->setEndValue(1.);
    m_scaleAnimation->start();
    QObject::connect(m_scaleAnimation, SIGNAL( finished()), m_scaleAnimation, SLOT(deleteLater()));

    QGraphicsItem::hoverLeaveEvent(ev);
}

