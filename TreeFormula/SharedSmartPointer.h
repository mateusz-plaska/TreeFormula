#pragma once

#include "RefCounter.h"

#include <vector>
#include <iostream>
#include <map>
using namespace std;

template<typename T>
class SharedSmartPointer {
public:
	SharedSmartPointer(T* pointer) {
		this->pointer = pointer;
		this->counter = new RefCounter();
		this->counter->add();
        addToSharedMap();
	}

	SharedSmartPointer(const SharedSmartPointer<T>& otherSharedSmartPointer) {
		this->pointer = otherSharedSmartPointer.pointer;
		this->counter = otherSharedSmartPointer.counter;
		this->counter->add();
        addToSharedMap();   
	}

	SharedSmartPointer(SharedSmartPointer<T>&& otherSharedSmartPointer) {
		this->pointer = otherSharedSmartPointer.pointer;
		this->counter = otherSharedSmartPointer.counter;
		otherSharedSmartPointer.pointer = nullptr;
		otherSharedSmartPointer.counter = nullptr;
	}

	~SharedSmartPointer() {
        removeFromSharedMap();
        releaseMemory();
	}

	SharedSmartPointer<T>& operator=(const SharedSmartPointer<T>& otherSharedSmartPointer) {
		if (this != &otherSharedSmartPointer) {
			releaseMemory();

			this->pointer = otherSharedSmartPointer.pointer;
			this->counter = otherSharedSmartPointer.counter;
			this->counter->add();
            addToSharedMap();     
		}
		return *this;
	}

	SharedSmartPointer<T>& operator=(SharedSmartPointer<T>&& otherSharedSmartPointer) {
		if (this != &otherSharedSmartPointer) {
			releaseMemory();

			this->pointer = otherSharedSmartPointer.pointer;
			this->counter = otherSharedSmartPointer.counter;
			otherSharedSmartPointer.pointer = nullptr;
			otherSharedSmartPointer.counter = nullptr;
		}
		return *this;
	}

	T& operator*() {
		return *pointer;
	}

	T* operator->() {
		return pointer;
	}

    void print() {
        auto it = sharedMap.find(pointer);
        if (it != sharedMap.end()) {
            auto& vec = it->second;
            for (auto e : vec) {
                cout << e << " ";
            }
        }
        cout << endl;
    }


	// ew. argument jako np. (T&& arg) i potem new T(arg)
	template<typename... Args>
	static SharedSmartPointer<T> makeSharedSmartPointer(Args... args) {
		return SharedSmartPointer<T>(new T(std::forward<Args>(args)...));
	}

private:
	T* pointer;
	RefCounter* counter;
    static map<T*, vector<SharedSmartPointer<T>*>> sharedMap;

	void releaseMemory() {
		if (this->counter != nullptr && this->counter->dec() == 0) {
			delete pointer;
			delete counter;
		}
	}

    void addToSharedMap() {
        sharedMap[pointer].push_back(this);
    }

    void removeFromSharedMap() {
        if (pointer != nullptr) {
            auto it = sharedMap.find(pointer);
            if (it != sharedMap.end()) {
                auto& vec = it->second;
                vec.erase(remove(vec.begin(), vec.end(), this), vec.end());
                if (vec.empty()) {
                    sharedMap.erase(it);
                }
            }
        }
    }
};


template<typename T>
map<T*, vector<SharedSmartPointer<T>*>> SharedSmartPointer<T>::sharedMap;