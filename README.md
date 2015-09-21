# ThreadPool
自己实现的一个简单的Linux下的线程池，可以设置线程池线程个数，任务队列大小

## master      使用条件变量和互斥量实现同步队列，volatile变量作为线程池退出标志
## feature/semaphore   使用信号量维护同步队列
## feature/NullPtrClose    使用一个空指针作为同步队列结束的标记，线程在同步队列pop到空指针时退出
## feature/sleepTest       利用usleep模拟IO密集型任务进行测试
