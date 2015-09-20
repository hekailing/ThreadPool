#ifndef __THREAD_POOL_H__
#define __THREAD_POOL_H__

#include <vector>
#include <tr1/memory>
#include <pthread.h>
#include "SyncQueue.h"

class WorkItem {
 public:
    virtual ~WorkItem() { }
    virtual void process() = 0;
};
typedef std::tr1::shared_ptr<WorkItem> WorkItemPtr;

class ThreadPool {
 public:
 ThreadPool(size_t threadCount = 20, size_t queueSize = 1000) 
     : _taskQueue(queueSize) {
        for (size_t i = 0; i < threadCount; ++i) {
            pthread_t tid;
            pthread_create(&tid, NULL, (ThreadLoop)&ThreadPool::workLoop, this);
            _threads.push_back(tid);
        }
        pthread_spin_init(&_closeSpin, PTHREAD_PROCESS_PRIVATE);
    }
    ~ThreadPool() {
        pthread_spin_lock(&_closeSpin);
        _taskQueue.wakeAll();
        for (size_t i = 0; i < _threads.size(); ++i) {
            pthread_join(_threads[i], NULL);
        }
        _threads.clear();
        pthread_spin_unlock(&_closeSpin);
        pthread_spin_destroy(&_closeSpin);
    }
    void pushWorkItem(WorkItemPtr workItem);
 private:
    typedef void*(*ThreadLoop)(void*);
    void* workLoop();
    std::vector<pthread_t> _threads;
    SyncQueue<WorkItemPtr> _taskQueue;
    pthread_spinlock_t _closeSpin;
};

#endif
