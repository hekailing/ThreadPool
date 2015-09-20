#ifndef __SYNC_QUEUE_H__
#define __SYNC_QUEUE_H__

#include <pthread.h>
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
        pthread_cond_init(&_emptyCond, NULL);
        pthread_cond_init(&_fullCond, NULL);
    }
    ~SyncQueue() { 
        pthread_mutex_destroy(&_qMutex);
        pthread_cond_destroy(&_emptyCond);
        pthread_cond_destroy(&_fullCond);
    }
    T pop();
    void push(T);
    void wakeAll();
 private:
    std::deque<T> _q;
    pthread_mutex_t _qMutex;
    pthread_cond_t _emptyCond;
    pthread_cond_t _fullCond;
    const size_t _capacity;
};


template<typename T>
T SyncQueue<T>::pop() {
    bool wakePush = false;
    T t;
    {
        ScopedLocker lock(_qMutex);
        if (_q.empty()) {
            pthread_cond_wait(&_emptyCond, &_qMutex);
        }
        wakePush = (_q.size() == _capacity);
        if (!_q.empty()) {
            t = _q.front();
            _q.pop_front();
        }
    }
    if (wakePush) {
        pthread_cond_broadcast(&_fullCond);
    }
    return t;
}


template <typename T>
void SyncQueue<T>::push(T t) {
    bool wakePop = false;
    {
        ScopedLocker lock(_qMutex);
        while (_q.size() == _capacity) {
            pthread_cond_wait(&_fullCond, &_qMutex);
        }
        wakePop = (_q.size() == 0);
        _q.push_back(t);
    }
    if (wakePop) {
        pthread_cond_signal(&_emptyCond);
    }
}

template <typename T>
void SyncQueue<T>::wakeAll() {
    pthread_cond_broadcast(&_emptyCond);
}

#endif
