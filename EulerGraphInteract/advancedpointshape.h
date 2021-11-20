#ifndef ADVANCEDPOINTSHAPE_H
#define ADVANCEDPOINTSHAPE_H

#include <QObject>
#include <QGraphicsItem>
#include <QGraphicsEllipseItem>
#include "egscene.h"
#include <QSharedPointer>
#include <QPainter>
#include <QPropertyAnimation>
#include "vertex.h"

class AdvancedPointShape : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
    Q_PROPERTY(QColor color READ color WRITE setColor)
    Q_PROPERTY(qreal scale READ scale WRITE setScale)
public:
    double m_x;
    double m_y;
    EGScene *m_scene;
    QPropertyAnimation* m_colorAnimation;
    QPropertyAnimation* m_scaleAnimation;
    QColor m_color;
    qreal m_scale;

    static QColor s_defaultColor;
    static double s_centralDiameter;
    static double s_ringDiameter;
    static double s_maxDiameter;
    static QColor s_colorAfterAnim;
    static qreal s_scaleAfterAnim;

    AdvancedPointShape(EGScene *scene = nullptr, Vertex vertex = Vertex());
    ~AdvancedPointShape();
    bool operator==(const AdvancedPointShape &other) const;
    bool operator!=(const AdvancedPointShape &other) const;

    QColor color() const;
    void setColor(const QColor &color);
    qreal scale() const;
    void setScale(const qreal &scale);

    virtual QRectF boundingRect() const override;
    virtual void paint(QPainter * painter,const QStyleOptionGraphicsItem * ,QWidget * ) override;
private:
    QSharedPointer<QGraphicsEllipseItem> m_centralPoint;
    QSharedPointer<QGraphicsEllipseItem> m_ring;
protected:
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *ev) override;
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *ev) override;
};

#endif // ADVANCEDPOINTSHAPE_H
