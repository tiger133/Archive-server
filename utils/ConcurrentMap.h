//
// Created by piotr on 30.04.16.
//

#ifndef UNTITLED_CONCURRENTMAP_H
#define UNTITLED_CONCURRENTMAP_H

#include <unordered_map>
#include <mutex>
namespace Utils{
    template<typename K, typename V>
    class ConcurrentMap {
    public:
        void insert(std::pair<K,V> &elem)
        {
            mutex.lock();
            map.insert(elem);
            mutex.unlock();
        }
        void erase(K key)
        {
            mutex.lock();
            map.erase(key);
            mutex.unlock();
        }
        void foreach(std::function<void(V)> fun)
        {
            mutex.lock();
            for(auto it = map.begin(); it!=map.end();it++)
            {
                fun(*(it)->second);
            }
            mutex.unlock();
        }
        void clear()
        {
            mutex.lock();
            map.clear();
            mutex.unlock();
        }
    private:
        std::mutex mutex;
        std::unordered_map<K,V> map;

    };
}



#endif //UNTITLED_CONCURRENTMAP_H


