#include "edge.h"

Edge::Edge(Vertex theone, Vertex theother):QObject(),m_theOne(theone),m_theOther(theother)
{

}

Edge::Edge(const Edge &other):QObject(),m_theOne(other.m_theOne),m_theOther(other.m_theOther)
{

}

Edge &Edge::operator=(const Edge &other)
{
    if (this == &other)
        return *this;

    m_theOne = other.m_theOne;
    m_theOther = other.m_theOther;
    return *this;
}

bool Edge::operator==(const Edge &other) const
{
    if( ((other.m_theOne == m_theOne) && (other.m_theOther == m_theOther))
    || ((other.m_theOne == m_theOther) && (other.m_theOther == m_theOne)) )
        return true;

    return false;
}

bool Edge::operator!=(const Edge &other) const
{
    return !( (*this) == other);
}
