//
// Created by Михаил Терентьев on 28/04/2018.
//

#pragma once

#include <iostream>
#include <memory.h>

using uint =unsigned int;
const int MAX_SMALL = 6;
//const int MAX_SMALLM = 51;


struct my_vector {
    struct info {
        int use_count;
        uint long_ob[];
    };

    struct big_data {
        info *ptr;
        size_t b_capacity;

        size_t capacity() const {
            return b_capacity;
        }

        uint &operator[](size_t i) {
            return ptr->long_ob[i];
        }

        uint const &operator[](size_t i) const {
            return ptr->long_ob[i];
        }

    };

    struct small_data {
//    size_t magic;
        size_t small_s;
        uint small_ob[MAX_SMALL];

        inline size_t size() const {
            return small_s;
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
            //magic -= 2;
            small_s--;
        }

        inline void push_back(uint const &value) {
            small_ob[size()] = value;
            //magic += 2;
            small_s++;
        }
    };


    size_t size_;
    bool isSmall;

    explicit my_vector(size_t n = 0) noexcept;

    my_vector(my_vector const &);

    inline my_vector(size_t n, uint element) {
        resize(n);
        for (int i = 0; i < n; ++i) {
            (*this)[i] = element;
        }
    }

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

    void ensure_cap(size_t);

    void reserve(size_t n);

private:

    union {
        big_data big_object;
        small_data small_object;
    };

    void unique_check();

    size_t capacity() const;

    void big_to_small();

};



