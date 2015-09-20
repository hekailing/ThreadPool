#include <pthread.h>
#include <vector>
#include <math.h>
#include "ThreadPool.h"

using namespace std;


class SimpleWorkItem : public WorkItem {
public:
    SimpleWorkItem(size_t len) : _len(len) { } 
    void process() {
        _val.resize(_len);
        for (size_t i = 0; i < _len; ++i) {
            _val[i] = i;
        }
    }
private:
    vector<int> _val;
    size_t _len;
};

    
void* pushThread(void* p) {
    ThreadPool* pool = (ThreadPool*)p;
    size_t sr = time(0);
    for (size_t i = 0; i < 100000; ++i) {
        size_t len = rand_r(&sr) % 1000;
        pool->pushWorkItem(WorkItemPtr(new SimpleWorkItem(len)));
    }
    return NULL;
}


int main()
{
    size_t case_num = 1000;
    for (size_t t = 0; t < case_num; ++t)
    {
        cout << "case : " << t << endl;
        ThreadPool threadPool(20);
        vector<pthread_t> tids;
        for (size_t i = 0; i < 5; ++i) {
            pthread_t tid;
            pthread_create(&tid, NULL, pushThread, &threadPool);
            tids.push_back(tid);
        }
        for (size_t i = 0; i < 5; ++i) {
            pthread_join(tids[i], NULL);
        }
    }
    return 0;
}

