//
// Created by Михаил Терентьев on 01/05/2018.
//

#pragma once;

#include <iostream>
#include <memory.h>

using uint =unsigned int;
const int MAX_SMALL = 7;


struct info {
    int use_count;
    uint long_ob[];
};

struct big_data {
    info *ptr;
    size_t size_;
    size_t b_capacity;

    size_t size() const {
        return size_;
    }

    size_t capacity() const {
        return b_capacity;
    }

    uint &operator[](size_t i) {
        return ptr->long_ob[i];
    }

    uint const &operator[](size_t i) const {
        return ptr->long_ob[i];
    }

    void pop_back() {
        --size_;
    }

    void push_back(uint const &value) {
        ptr->long_ob[size()] = value;
        ++size_;
    }
};

struct small_data {
    char magic;
    uint small_ob[MAX_SMALL];

    inline size_t size() const {
        return magic / 2;
    }

    inline size_t capacity() const {
        return MAX_SMALL;
    }

    inline uint &operator[](size_t i) {
        return small_ob[i];
    }

    inline uint const &operator[](size_t i) const {
        return small_ob[i];
    }

    inline void pop_back() {
        magic -= 2;
    }

    inline void push_back(uint const &value) {
        small_ob[size()] = value;
        magic += 2;
    }
};


struct my_vector {

    inline my_vector(size_t n = 0);

    my_vector(my_vector const &);

    my_vector(int, uint);

    my_vector &operator=(my_vector const &);

    ~my_vector();

    bool is_small() const;

    size_t size() const;

    bool empty() const;

    void resize(size_t n);

    void pop_back();

    void push_back(uint const &);

    void clear();

    uint &back();

    uint &operator[](size_t);

    uint const &operator[](size_t) const;

private:

    union {
        big_data big_object;
        small_data small_object;
    };

    void try_split();

    void ensure_cap(size_t);

    size_t capacity() const;

    void big_to_small();

};



