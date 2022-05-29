#include "euleriangraphgenerationworker.h"
#include <QRandomGenerator>

const double EulerianGraphGenerationWorker::s_planLimitX = 600.;
const double EulerianGraphGenerationWorker::s_planLimitY = 300.;
const double EulerianGraphGenerationWorker::s_distanceSquareMinAcceptable = 5625.; //(78*75)
const double EulerianGraphGenerationWorker::s_distanceSquareConnectFirstPass = 22500.; //(150*150)
const double EulerianGraphGenerationWorker::s_distanceSquareConnectSecondPass = 90000.; //(300*300)
const int EulerianGraphGenerationWorker::s_timeout = 10000;
const double EulerianGraphGenerationWorker::s_cosineSquareMaxAcceptable = 0.99240395683; // 5 degrees angle

EulerianGraphGenerationWorker::EulerianGraphGenerationWorker()
    : QObject{}
{

}

void EulerianGraphGenerationWorker::process()
{
    emit sendStepInfo(0);

    // GENERATING RANDOM (any) GRAPH PHASE

    // Create vertices randomly placed
    uint currentVerticesListSize = 0;
    m_timer.start();
    while(currentVerticesListSize < m_verticesNumberRequested)
    {
        if(m_timer.elapsed() > s_timeout)
        {
            emit sendStepInfo(21);
            emit finished();
            return;
        }

        if(m_timer.elapsed() > s_timeout/2)
            emit sendStepInfo(11,currentVerticesListSize+1, m_verticesNumberRequested, m_timer.elapsed(), s_timeout);
        else
            emit sendStepInfo(1,currentVerticesListSize+1, m_verticesNumberRequested);

        placeOneVertexRandomly();
        currentVerticesListSize = m_generatedGraphData.first.size();
    }

    int totalVertices = m_generatedGraphData.first.size();

    if(totalVertices == 0)
    {
        emit sendStepInfo(31);
        emit finished();
        return;
    }

    // Create edges randomly
    if(totalVertices < 20)
        m_vertexConnectionMaxNumber = 4;
    else if(totalVertices < 40)
        m_vertexConnectionMaxNumber = 5;

    for(int i = 0 ; i < totalVertices ; ++i)
    {
        emit sendStepInfo(2,i+1, totalVertices);
        connectVertexRandomly(m_generatedGraphData.first.at(i));
    }

    for(QList<Vertex>::iterator it = m_generatedGraphData.first.begin() ; it != m_generatedGraphData.first.end() ; ++it)
    {
        if( checkVertexConnectionNumber(*it) == 0 )
        {
            emit sendStepInfo(32);
            emit finished();
            return;
        }
    }

    // CHECK IF EULERIAN PHASE
    // We will delete edges until it is checked as Eulerian...

    // First, we delete one-degree vertices until we have no more than 2 of them

    int oneDegreeVerticesNum = 0;
    int oneDegreeVerticesNumInitial = 0;

    for(QList<Edge>::iterator it = m_generatedGraphData.second.begin() ; it != m_generatedGraphData.second.end() ; ++it)
    {
        oneDegreeVerticesNum = countOneDegreeVertices();
        if(oneDegreeVerticesNum <= 2)
            break;

        if(it == m_generatedGraphData.second.begin())
            oneDegreeVerticesNumInitial = oneDegreeVerticesNum;

        emit sendStepInfo(3,oneDegreeVerticesNum, oneDegreeVerticesNumInitial);

        if(checkVertexConnectionNumber(it->m_theOne) <= 1)
        {
            m_generatedGraphData.first.removeOne(it->m_theOne);
            it = m_generatedGraphData.second.erase(it);
            --it;
            continue;
        }

        if(checkVertexConnectionNumber(it->m_theOther) <= 1)
        {
            m_generatedGraphData.first.removeOne(it->m_theOther);
            it = m_generatedGraphData.second.erase(it);
            --it;
        }
    }

    // Secondly, we save the few one-degree vertices left
    for(QList<Vertex>::iterator it = m_generatedGraphData.first.begin() ; it != m_generatedGraphData.first.end() ; ++it)
    {
        if( checkVertexConnectionNumber(*it) == 1 )
            m_oneDegreeVerticesList.append(*it);
    }

    // Finally, we either add or delete one edge for vertices that do not have even number of connections

    int passStep4 = 0;
    while(!doEveryVerticesAreEven())
    {
        passStep4++;
        int index = 0;
        for(QList<Vertex>::iterator it = m_generatedGraphData.first.begin() ; it != m_generatedGraphData.first.end() ; ++it)
        {
            index++;
            emit sendStepInfo(4,passStep4,index+1,totalVertices);

            if( m_oneDegreeVerticesList.contains(*it) )
                continue;
            int connectionsNumber = checkVertexConnectionNumber(*it);
            if( connectionsNumber % 2 != 0)
            {
                if(connectionsNumber < m_vertexConnectionMaxNumber)
                {
                    if(!connectVertexToOneRandom(*it))
                    {
                        emit sendStepInfo(24);
                        emit finished();
                        return;
                    }
                }
                else
                {
                    if(!disconnectVertexFromOneRandom(*it))
                    {
                        emit sendStepInfo(25);
                        emit finished();
                        return;
                    }
                }
            }
        }
        if(passStep4 > 30)
        {
            emit sendStepInfo(26);
            emit finished();
            return;
        }
    }

    emit sendGraphData(m_generatedGraphData);
    emit finished();
}

void EulerianGraphGenerationWorker::placeOneVertexRandomly()
{
    double randomX = (QRandomGenerator::global()->generateDouble()-0.5)*2*s_planLimitX;
    double randomY = (QRandomGenerator::global()->generateDouble()-0.5)*2*s_planLimitY;

    for(QList<Vertex>::iterator it = m_generatedGraphData.first.begin() ; it != m_generatedGraphData.first.end() ; ++it)
    {
        if( (it->m_x - randomX)*(it->m_x - randomX) + (it->m_y - randomY)*(it->m_y - randomY) < s_distanceSquareMinAcceptable)
            return;
    }
    m_generatedGraphData.first.append(Vertex(randomX, randomY));
}

void EulerianGraphGenerationWorker::connectVertexRandomly(const Vertex &theVertex)
{
    int randomConnectionNumber = QRandomGenerator::global()->bounded(1,m_vertexConnectionMaxNumber+1);

    int currentConnectionNumber = checkVertexConnectionNumber(theVertex);

    if(currentConnectionNumber >= m_vertexConnectionMaxNumber)
        return;

    // First Pass (to create edge with the nearest vertices, for user confort)

    for(QList<Vertex>::iterator it = m_generatedGraphData.first.begin() ; it != m_generatedGraphData.first.end() ; ++it)
    {
        if(currentConnectionNumber >= randomConnectionNumber)
            return;

        if(*it == theVertex)
            continue;

        Edge edgeToAdd(*it, theVertex);
        if(doesEdgeExist(edgeToAdd))
            continue;

        double distance = (it->m_x - theVertex.m_x)*(it->m_x - theVertex.m_x) + (it->m_y - theVertex.m_y)*(it->m_y - theVertex.m_y);
        if( distance < s_distanceSquareConnectFirstPass)
        {
            if(checkVertexConnectionNumber(theVertex) < m_vertexConnectionMaxNumber)
            {
                if(wouldEdgesBeSuperimposedToAnyExisting(edgeToAdd))
                    continue;
                else
                {
                    m_generatedGraphData.second.append(edgeToAdd);
                    ++currentConnectionNumber;
                }
            }
        }
    }

    // Second Pass

    for(QList<Vertex>::iterator it = m_generatedGraphData.first.begin() ; it != m_generatedGraphData.first.end() ; ++it)
    {
        if(currentConnectionNumber >= randomConnectionNumber)
            return;

        if(*it == theVertex)
            continue;

        Edge edgeToAdd(*it, theVertex);
        if(doesEdgeExist(edgeToAdd))
            continue;

        double distance = (it->m_x - theVertex.m_x)*(it->m_x - theVertex.m_x) + (it->m_y - theVertex.m_y)*(it->m_y - theVertex.m_y);
        if( distance >= s_distanceSquareConnectFirstPass && distance < s_distanceSquareConnectSecondPass)
        {
            if(checkVertexConnectionNumber(theVertex) < m_vertexConnectionMaxNumber)
            {
                if(wouldEdgesBeSuperimposedToAnyExisting(edgeToAdd))
                    continue;
                else
                {
                    m_generatedGraphData.second.append(edgeToAdd);
                    ++currentConnectionNumber;
                }
            }
        }
    }

    // Last Pass

    for(QList<Vertex>::iterator it = m_generatedGraphData.first.begin() ; it != m_generatedGraphData.first.end() ; ++it)
    {
        if(currentConnectionNumber >= randomConnectionNumber)
            return;

        if(*it == theVertex)
            continue;

        Edge edgeToAdd(*it, theVertex);
        if(doesEdgeExist(edgeToAdd))
            continue;

        double distance = (it->m_x - theVertex.m_x)*(it->m_x - theVertex.m_x) + (it->m_y - theVertex.m_y)*(it->m_y - theVertex.m_y);
        if( distance >= s_distanceSquareConnectSecondPass)
        {
            if(checkVertexConnectionNumber(theVertex) < m_vertexConnectionMaxNumber)
            {
                if(wouldEdgesBeSuperimposedToAnyExisting(edgeToAdd))
                    continue;
                else
                {
                    m_generatedGraphData.second.append(edgeToAdd);
                    ++currentConnectionNumber;
                }
            }
        }
    }

}

bool EulerianGraphGenerationWorker::connectVertexToOneRandom(const Vertex &theVertex)
{
    int randomVertexIndex = 0;
    m_timer.start();
    uint numRefusedSuperimposed = 0;
    uint numRefusedConnectNum = 0;
    Vertex randomVertex;
    do
    {
        randomVertexIndex = QRandomGenerator::global()->bounded(0,m_generatedGraphData.first.size());
        randomVertex = m_generatedGraphData.first.at(randomVertexIndex);

        if(wouldEdgesBeSuperimposedToAnyExisting(Edge(randomVertex,theVertex)))
            ++numRefusedSuperimposed;
        if(checkVertexConnectionNumber(randomVertex) >= m_vertexConnectionMaxNumber)
            ++numRefusedConnectNum;

        if(m_timer.elapsed() > s_timeout/2)
            emit sendStepInfo(14, m_timer.elapsed(), s_timeout);
        if(m_timer.elapsed() > s_timeout)
            return false;
    }
    while(randomVertex == theVertex || doesEdgeExist(Edge(randomVertex,theVertex))
          || m_oneDegreeVerticesList.contains(randomVertex) || numRefusedConnectNum < 20
          || numRefusedSuperimposed < 20);

    m_generatedGraphData.second.append(Edge(randomVertex,theVertex));

    return true;
}

bool EulerianGraphGenerationWorker::disconnectVertexFromOneRandom(const Vertex &theVertex)
{
    QList<Edge> edgesConnectedList;
    for(QList<Edge>::iterator it = m_generatedGraphData.second.begin() ; it != m_generatedGraphData.second.end() ; ++it)
    {
        if(it->m_theOne == theVertex || it->m_theOther == theVertex)
            edgesConnectedList.append(*it);
    }

    int randomEdgeIndex = 0;
    Edge randomEdge;
    Vertex theOtherVertex;
    m_timer.start();
    do
    {
        randomEdgeIndex = QRandomGenerator::global()->bounded(0,edgesConnectedList.size());
        randomEdge = edgesConnectedList.at(randomEdgeIndex);
        theOtherVertex = randomEdge.m_theOne == theVertex ? randomEdge.m_theOther : randomEdge.m_theOne;
        if(m_timer.elapsed() > s_timeout/2)
            emit sendStepInfo(15, m_timer.elapsed(), s_timeout);
        if(m_timer.elapsed() > s_timeout)
            return false;
    }
    while(checkVertexConnectionNumber(theOtherVertex) <= 2 );

    m_generatedGraphData.second.removeOne(randomEdge);

    return true;
}

int EulerianGraphGenerationWorker::checkVertexConnectionNumber(const Vertex &theVertex)
{
    int numReturned = 0;
    for(QList<Edge>::iterator it = m_generatedGraphData.second.begin() ; it != m_generatedGraphData.second.end() ; ++it)
    {
        if(it->m_theOne == theVertex || it->m_theOther == theVertex)
            ++numReturned;
    }
    return numReturned;
}

bool EulerianGraphGenerationWorker::doEveryVerticesAreEven()
{
    for(QList<Vertex>::iterator it = m_generatedGraphData.first.begin() ; it != m_generatedGraphData.first.end() ; ++it)
    {
        if( m_oneDegreeVerticesList.contains(*it) )
            continue;
        if( checkVertexConnectionNumber(*it) % 2 != 0)
            return false;
    }
    return true;
}

bool EulerianGraphGenerationWorker::doesEdgeExist(const Edge &theEdge)
{
    for(QList<Edge>::iterator it = m_generatedGraphData.second.begin() ; it != m_generatedGraphData.second.end() ; ++it)
    {
        if(*it == theEdge)
            return true;
    }
    return false;
}

int EulerianGraphGenerationWorker::countOneDegreeVertices()
{
    int numReturned = 0;
    for(QList<Vertex>::iterator it = m_generatedGraphData.first.begin() ; it != m_generatedGraphData.first.end() ; ++it)
    {
        if(checkVertexConnectionNumber(*it))
            ++numReturned;
    }

    return numReturned;
}

bool EulerianGraphGenerationWorker::wouldEdgesBeSuperimposed(const Edge &oneEdge, const Edge &otherEdge)
{
    if(oneEdge == otherEdge)
        return true;

    double OAx,OAy,OBx,OBy = 0;

    if(oneEdge.m_theOne == otherEdge.m_theOne)
    {
        OAx = oneEdge.m_theOther.m_x-oneEdge.m_theOne.m_x;
        OAy = oneEdge.m_theOther.m_y-oneEdge.m_theOne.m_y;
        OBx = otherEdge.m_theOther.m_x-oneEdge.m_theOne.m_x;
        OBy = otherEdge.m_theOther.m_y-oneEdge.m_theOne.m_y;
    }
    else if(oneEdge.m_theOne == otherEdge.m_theOther)
    {
        OAx = oneEdge.m_theOther.m_x-oneEdge.m_theOne.m_x;
        OAy = oneEdge.m_theOther.m_y-oneEdge.m_theOne.m_y;
        OBx = otherEdge.m_theOne.m_x-oneEdge.m_theOne.m_x;
        OBy = otherEdge.m_theOne.m_y-oneEdge.m_theOne.m_y;
    }
    else if(oneEdge.m_theOther == otherEdge.m_theOne)
    {
        OAx = oneEdge.m_theOne.m_x-oneEdge.m_theOther.m_x;
        OAy = oneEdge.m_theOne.m_y-oneEdge.m_theOther.m_y;
        OBx = otherEdge.m_theOther.m_x-oneEdge.m_theOther.m_x;
        OBy = otherEdge.m_theOther.m_y-oneEdge.m_theOther.m_y;
    }
    else if(oneEdge.m_theOther == otherEdge.m_theOther)
    {
        OAx = oneEdge.m_theOne.m_x-oneEdge.m_theOther.m_x;
        OAy = oneEdge.m_theOne.m_y-oneEdge.m_theOther.m_y;
        OBx = otherEdge.m_theOne.m_x-oneEdge.m_theOther.m_x;
        OBy = otherEdge.m_theOne.m_y-oneEdge.m_theOther.m_y;
    }
    else
        return false;

    double scalarProduct = OAx * OBx + OAy * OBy;

    if(scalarProduct < 0.)
        return false;

    double magnitudeSquareProduct = (OAx * OAx + OAy * OAy)*(OBx * OBx + OBy * OBy);

    if(magnitudeSquareProduct == 0.)
        return true;

    double cosineSquare = scalarProduct * scalarProduct / magnitudeSquareProduct;

    if( cosineSquare > s_cosineSquareMaxAcceptable)
        return true;
    return false;
}

bool EulerianGraphGenerationWorker::wouldEdgesBeSuperimposedToAnyExisting(const Edge &theEdge)
{
    for(QList<Edge>::iterator it = m_generatedGraphData.second.begin() ; it != m_generatedGraphData.second.end() ; ++it)
    {
        if(wouldEdgesBeSuperimposed(theEdge,*it))
            return true;
    }
    return false;
}
