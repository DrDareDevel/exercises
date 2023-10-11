#include <list>
#include <unordered_map>

using namespace std;

class lru_cache {
    int capacity;
    unordered_map<int, list<pair<int,int>>::iterator> store;
    list<pair<int,int>> lru;
    
public:
    lru_cache(int capacity) : capacity(capacity) {}
        
    int get(int key) {
        if(store.count(key) == 0) return -1;
        auto v = store[key];
        pair<int,int> n{v->first,v->second};
        lru.erase(v);
        lru.push_back(n);
        store[key] = --lru.end();

        return n.second;
    }

    void put(int key, int value) {     
        auto it = store.find(key);   
        if(it != store.end()) {
            lru.erase(it->second);
        }

        pair<int,int> n{key,value};
        lru.push_back(n);
        store[key] = --lru.end();
        
        if(store.size() > capacity) {
            auto k = lru.front();
            lru.pop_front();
            store.erase(k.first);
        }
    }
};
