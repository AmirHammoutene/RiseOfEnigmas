#ifndef VERTEX_H
#define VERTEX_H

#include <QObject>

class Vertex : public QObject
{
    Q_OBJECT
public:
    double m_x;
    double m_y;
    Vertex(double x = 0., double y = 0.);
    Vertex(const Vertex &other);
    Vertex &operator=(const Vertex &other);
    bool operator==(const Vertex &other) const;
    bool operator!=(const Vertex &other) const;

};

#endif // VERTEX_H
