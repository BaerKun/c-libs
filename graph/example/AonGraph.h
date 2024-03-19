#ifndef AONGRAPH_H
#define AONGRAPH_H

#ifndef INFINITY
#define INFINITY 0x7fffffff
#endif

#ifndef INITIAL_NODES_NUMBER
#define INITIAL_NODES_NUMBER 32
#endif

typedef int TimeType;
typedef int NodeId;
typedef struct AonGraph AonGraph, *AonGraphPtr;
typedef struct ActivityNode ActivityNode, *ActivityNodePtr;
typedef struct Edge AdjacencyList, *AdjacencyListPtr;
struct ActivityNode {
    TimeType duration;
    TimeType earlyStart;
    TimeType lateStart;
    TimeType slack;
    NodeId path;
    int inDegree;
    AdjacencyListPtr pSuccessorList;
};
struct Edge {
    NodeId id;
    AdjacencyListPtr next;
};
struct AonGraph {
    int capacity;
    int nodeNum;
    ActivityNodePtr nodes;
};

AonGraphPtr CreateAonGraph(int nodeNum, const int *activityDurations);

void DeleteAonGraph(AonGraphPtr pAonGraph);

void AddActivityNode(AonGraphPtr pAonGraph, TimeType duration);

void EstablishDependency(AonGraphPtr pAonGraph, NodeId start, NodeId end);

void BuildCriticalPath(AonGraphPtr pAonGraph);

void CopyPath(AonGraphPtr pAonGraph, NodeId copyArray[]);

#endif //AONGRAPH_H
