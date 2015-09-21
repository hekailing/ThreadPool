#include "ThreadPool.h"
#include <iostream>

void* ThreadPool::workLoop() {
    while (!_closing) {
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

