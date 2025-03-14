#pragma once

class RefCounter {
public:
	RefCounter() : count(0) {}

	int get() const {
		return count;
	}

	int add() {
		return (++count);
	}

	int dec() {
		return (--count);
	}

    RefCounter& operator++() {
        ++count;
        return *this;
    }

    RefCounter operator++(int) {
        RefCounter temp = *this;
        ++(*this); 
        return temp;
    }

    RefCounter& operator--() {
        --count;
        return *this;
    }

    RefCounter operator--(int) {
        RefCounter temp = *this;
        --(*this); 
        return temp;
    }

private:
	int count;
};

