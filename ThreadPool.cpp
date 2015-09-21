#include "ThreadPool.h"
#include <iostream>

void* ThreadPool::workLoop() {
    WorkItemPtr workItemPtr;
    while (_taskQueue.popUnlessClosed(workItemPtr)) {
        workItemPtr->process();
    }
    return NULL;
}


void ThreadPool::pushWorkItem(WorkItemPtr workItem) {
    _taskQueue.push(workItem);
}

