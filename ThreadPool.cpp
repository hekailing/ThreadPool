#include "ThreadPool.h"
#include <iostream>

void* ThreadPool::workLoop() {
    while (true) {
        WorkItemPtr workItemPtr = _taskQueue.pop();
        if (workItemPtr) {
            workItemPtr->process();
        } else {
            break;
        }
    }
    return NULL;
}


void ThreadPool::pushWorkItem(WorkItemPtr workItem) {
    _taskQueue.push(workItem);
}

