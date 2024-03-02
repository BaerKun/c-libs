#ifndef AONGRAPH_H
#define AONGRAPH_H

#ifndef INFINITY
#define INFINITY 0x7fffffff
#endif

#ifndef INITIAL_NODES_NUMBER
#define INITIAL_NODES_NUMBER 32
#endif

typedef int Time;
typedef int NodeId;
typedef struct AonGraph AonGraph, *AonGraphPtr;
typedef struct ActivityNode ActivityNode, *ActivityNodePtr;
typedef struct AdjacencyList AdjacencyList, *AdjacencyListPtr;
struct ActivityNode {
    Time duration;
    Time earlyStart;
    Time lateStart;
    Time slack;
    NodeId path;
    int inDegree;
    AdjacencyListPtr pSuccessorList;
    ActivityNodePtr next;
};
struct AdjacencyList {
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

void AddActivityNode(AonGraphPtr pAonGraph, Time duration);

void EstablishDependency(AonGraphPtr pAonGraph, NodeId start, NodeId end);

void BuildCriticalPath(AonGraphPtr pAonGraph);

void CopyPath(AonGraphPtr pAonGraph, NodeId copyArray[]);

#endif //AONGRAPH_H
