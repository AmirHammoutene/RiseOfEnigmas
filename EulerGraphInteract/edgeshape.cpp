#include "edgeshape.h"


QColor EdgeShape::s_defaultColor =  QColor( 200,200,200,100 ) ;
qreal EdgeShape::s_lineWidth = 5.5 ;

EdgeShape::EdgeShape(EGScene *scene, Edge edge):QObject(),QGraphicsLineItem(),m_Ox(edge.m_theOne.m_x),m_Oy(edge.m_theOne.m_y),
    m_Ex(edge.m_theOther.m_x),m_Ey(edge.m_theOther.m_y),m_scene(scene)
{
    if(m_scene != nullptr)
    {
        m_scene->addLine(m_Ox,m_Oy,m_Ex,m_Ey,QPen(s_defaultColor,s_lineWidth));
        m_scene->appendEdgeShape(this);
    }
}

EdgeShape::~EdgeShape()
{

}

bool EdgeShape::operator==(const EdgeShape &other) const
{
    if( (other.m_Ox == m_Ox && other.m_Oy != m_Oy && other.m_Ex != m_Ex && other.m_Ey != m_Ey)
        || (other.m_Ex == m_Ox && other.m_Ey != m_Oy && other.m_Ox != m_Ex && other.m_Oy != m_Ey) )
        return true;

    return false;
}

bool EdgeShape::operator!=(const EdgeShape &other) const
{
    return !( (*this) == other );
}
