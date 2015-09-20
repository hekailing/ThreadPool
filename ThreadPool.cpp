#include "ThreadPool.h"
#include <iostream>

void* ThreadPool::workLoop() {
    while (!pthread_spin_trylock(&_closeSpin)) {
        pthread_spin_unlock(&_closeSpin);
        WorkItemPtr workItemPtr = _taskQueue.pop();
        if (workItemPtr) {
            workItemPtr->process();
        }
    }
    return NULL;
}


void ThreadPool::pushWorkItem(WorkItemPtr workItem) {
    _taskQueue.push(workItem);
}

