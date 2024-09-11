#include <stdlib.h>
#include <time.h>

#define NUM_OF_PLACES 222   // 地点数量
#define MAX_WORK_TIME 8.5   // 最大工作时间
#define INIT_PHEROMONE 0.1f // 初始信息素

typedef struct Path Path;

struct Path {
    int place;
    Path *next;
};

float dency = 0.1f; // 挥发率
float alpha = 1.0f;
int nplaces = NUM_OF_PLACES;
float moveTime[NUM_OF_PLACES][NUM_OF_PLACES]; // 两地移动所需时间
float pheromone[NUM_OF_PLACES][NUM_OF_PLACES]; // 信息素
float workTime[NUM_OF_PLACES]; // 工作所需时间
float trendency[NUM_OF_PLACES][NUM_OF_PLACES]; // alpha * 信息素 / 移动时间

float randomFloat(float max) {
    static unsigned int i = 0;
    srand(time(NULL) + i++);
    return max * rand() / RAND_MAX;
}

int randomInt(int n) {
    static unsigned int i = 100;
    srand(time(NULL) + i++);
    return rand() % n;
}

void init() {
    int i, j;
    float *_pheromone, *_trendency, *_moveTime;

    for(i = 0; i < nplaces; ++i) {
        _pheromone = pheromone[i];
        _trendency = trendency[i];
        _moveTime = moveTime[i];
        for(j = 0; j < nplaces; ++j) {
            _pheromone[j] = INIT_PHEROMONE;
            _trendency[j] = alpha * INIT_PHEROMONE * _moveTime[j];
        }
    }
}

int haveVisited(Path *head, int place) {
    while(head != NULL) {
        if(head->place == place)
            return 1;
        head = head->next;
    }
    return 0;
}

int chooseTarget(Path *head, int source, float totalTime) {
    float sum = 0.f,
        *_moveTime = moveTime[source],
        *_trendency = trendency[source];
    char mask[nplaces];
    int i;

    // 筛选所有可行路径
    for(i = 0; i < nplaces; ++i) {
        if (totalTime + _moveTime[i] + workTime[i] > MAX_WORK_TIME || haveVisited(head, i)) {
            mask[i] = 0;
            continue;
        }
        mask[i] = 1;
        sum += _trendency[i];
    }

    if(sum == 0.f)
        return -1;

    // 随机选择，左闭右开
    float random = randomFloat(sum);
    for(i = 0; i < nplaces; ++i) {
        if(mask[i] == 0)
            continue;
        random -= _trendency[i];
        if(random < 0.f)
            return i;
    }

    return nplaces - 1;
}

void update(Path *head, float totalTime) {
    Path *source = head,
        *target = head->next;

    while(target != NULL) {
        pheromone[source->place][target->place] *= 1.f - dency;
        pheromone[source->place][target->place] += 1.f / totalTime;
        trendency[source->place][target->place] = alpha * pheromone[source->place][target->place] / moveTime[source->place][target->place];
        source = target;
        target = target->next;
    }
}

void makeEmpty(Path *head) {
    Path *next = head->next;
    while (next != NULL) {
        head->next = next->next;
        free(next);
        next = head->next;
    }
}

void antColony(int nants) {
    float totalTime;
    int source, target;
    Path head, *path;

    while (nants-- > 0){
        // 随机出发点
        source = randomInt(nplaces);

        head.place = source;
        head.next = NULL;
        path = &head;

        totalTime = workTime[source];

        while (1) {
            // 选择下一个点
            target = chooseTarget(&head, source, totalTime);

            // 没有可行路径
            if(target == -1)
                break;

            totalTime += moveTime[source][target] + workTime[target];

            // 链接路径
            path->next = (Path *) malloc(sizeof(Path));
            path = path->next;
            path->place = target;
            path->next = NULL;

            source = target;
        }
        path->next = NULL;

        // 更新信息素和趋势
        update(&head, totalTime);
        // 清空路径
        makeEmpty(&head);
    }
}

int main(){
    /*
     * 换算、导入数据 moveTime/workTime
     * 设置dency/alpha ...
     */

    init();
    antColony(666);

    // 后续筛选...
}