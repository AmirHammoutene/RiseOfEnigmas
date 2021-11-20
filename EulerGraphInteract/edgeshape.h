#ifndef EDGESHAPE_H
#define EDGESHAPE_H

#include <QObject>
#include <QGraphicsLineItem>
#include "egscene.h"
#include "edge.h"

class EdgeShape : public QObject, public QGraphicsLineItem
{
    Q_OBJECT
public:
    double m_Ox;
    double m_Oy;
    double m_Ex;
    double m_Ey;
    EGScene *m_scene;

    static QColor s_defaultColor;
    static qreal s_lineWidth;
    EdgeShape(EGScene *scene = nullptr, Edge edge = Edge());
    ~EdgeShape();
    bool operator==(const EdgeShape &other) const;
    bool operator!=(const EdgeShape &other) const;
};

#endif // EDGESHAPE_H
