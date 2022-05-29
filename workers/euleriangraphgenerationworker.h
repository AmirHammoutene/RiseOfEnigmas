#ifndef EULERIANGRAPHGENERATIONWORKER_H
#define EULERIANGRAPHGENERATIONWORKER_H

#include <QObject>
#include "EulerGraphInteract/vertex.h"
#include "EulerGraphInteract/edge.h"
#include <QVariant>
#include <QElapsedTimer>

class EulerianGraphGenerationWorker : public QObject
{
    Q_OBJECT
public:
    explicit EulerianGraphGenerationWorker();
    uint m_verticesNumberRequested = 10;
    int m_vertexConnectionMaxNumber = 6;
    QPair< QList<Vertex> , QList<Edge> > m_generatedGraphData;
    QList<Vertex> m_oneDegreeVerticesList;
    const static double s_planLimitX;
    const static double s_planLimitY;
    const static double s_distanceSquareMinAcceptable;
    const static double s_cosineSquareMaxAcceptable;
    const static double s_distanceSquareConnectFirstPass;
    const static double s_distanceSquareConnectSecondPass;
    const static int s_timeout;
public slots:
    void process();
private:
    void placeOneVertexRandomly();
    void connectVertexRandomly(const Vertex & theVertex);
    bool connectVertexToOneRandom(const Vertex & theVertex);
    bool disconnectVertexFromOneRandom(const Vertex & theVertex);
    int checkVertexConnectionNumber(const Vertex & theVertex);
    bool doEveryVerticesAreEven();
    bool doesEdgeExist(const Edge & theEdge);
    int countOneDegreeVertices();
    bool wouldEdgesBeSuperimposed(const Edge & oneEdge, const Edge & otherEdge);
    bool wouldEdgesBeSuperimposedToAnyExisting(const Edge & theEdge);
    QElapsedTimer m_timer;
signals:
    void sendStepInfo(uint step, QVariant info1 = QVariant(), QVariant info2 = QVariant(), QVariant info3 = QVariant(), QVariant info4 = QVariant());
    void sendGraphData(QPair< QList<Vertex> , QList<Edge> > data);
    void finished();
};

#endif // EULERIANGRAPHGENERATIONWORKER_H
