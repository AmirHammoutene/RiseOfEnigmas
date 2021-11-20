#include "vertex.h"

Vertex::Vertex(double x, double y):QObject(),m_x(x),m_y(y)
{

}

Vertex::Vertex(const Vertex &other):QObject(),m_x(other.m_x), m_y(other.m_y)
{

}

Vertex &Vertex::operator=(const Vertex &other)
{
    if (this == &other)
        return *this;

    m_x = other.m_x;
    m_y = other.m_y;
    return *this;
}

bool Vertex::operator==(const Vertex &other) const
{
    if( (other.m_x != m_x)
      || (other.m_y != m_y) )
        return false;

    return true;
}

bool Vertex::operator!=(const Vertex &other) const
{
    return !( (*this) == other);
}
