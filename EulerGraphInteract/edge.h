#ifndef EDGE_H
#define EDGE_H

#include <QObject>
#include "vertex.h"

class Edge : public QObject
{
    Q_OBJECT
public:
    Vertex m_theOne;
    Vertex m_theOther;
    Edge(Vertex theone = Vertex(), Vertex theother = Vertex());
    Edge(const Edge &other);
    Edge &operator=(const Edge &other);
    bool operator==(const Edge &other) const;
    bool operator!=(const Edge &other) const;
};

#endif // EDGE_H
