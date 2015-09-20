#ifndef __SYNC_QUEUE_H__
#define __SYNC_QUEUE_H__

#include <pthread.h>
#include <semaphore.h>
#include <assert.h>
#include <deque>
#include <iostream>

class ScopedLocker {
 public:
 ScopedLocker(pthread_mutex_t &m) : _mutex(m) {
        pthread_mutex_lock(&_mutex);
    }
    ~ScopedLocker() { 
        pthread_mutex_unlock(&_mutex);
    }
 private:
 ScopedLocker(const ScopedLocker &lock) : _mutex(lock._mutex) { }
    ScopedLocker& operator=(const ScopedLocker &) { return *this; }
 private:
    pthread_mutex_t &_mutex;
};

template<typename T>
class SyncQueue {
 public:
 SyncQueue(size_t queueSize) 
     : _capacity(queueSize)  {
        pthread_mutex_init(&_qMutex, NULL);
        sem_init(&_emptySem, 0, 0);
        sem_init(&_fullSem, 0, _capacity);
    }
    ~SyncQueue() { 
        pthread_mutex_destroy(&_qMutex);
        sem_destroy(&_emptySem);
        sem_destroy(&_fullSem);
    }
    T pop();
    void push(T);
    void wakeAll(size_t n);
 private:
    std::deque<T> _q;
    pthread_mutex_t _qMutex;
    sem_t _emptySem;
    sem_t _fullSem;
    const size_t _capacity;
};


template<typename T>
T SyncQueue<T>::pop() {
    sem_wait(&_emptySem);
    ScopedLocker lock(_qMutex);
    T t;
    if (!_q.empty()) {
        t = _q.front();
        _q.pop_front();
        sem_post(&_fullSem);
    }
    return t;
}


template <typename T>
void SyncQueue<T>::push(T t) {
    sem_wait(&_fullSem);
    ScopedLocker lock(_qMutex);
    assert(_q.size() < _capacity);
    _q.push_back(t);
    sem_post(&_emptySem);
}

template <typename T>
void SyncQueue<T>::wakeAll(size_t n) {
    for (size_t i = 0; i < n; ++i) {
        sem_post(&_emptySem);
    }
}

#endif
