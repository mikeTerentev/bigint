//
// Created by Михаил Терентьев on 28/04/2018.
//

#include "my_vector.h"

my_vector::my_vector(size_t n) noexcept {
    //todo
    small_object.small_s = 0;
    resize(n);
}


my_vector::my_vector(my_vector const &other) {
    //todo
    small_object.small_s = 0;
    *this = other;
}


my_vector &my_vector::operator=(my_vector const &other) {
    clear();
    memcpy(this, &other, sizeof(my_vector));
    if (!is_small()) {
        big_object.ptr->use_count++;
    }
    return *this;
}

void my_vector::clear() {
    if (!is_small()) {
        big_object.ptr->use_count--;
        if (!big_object.ptr->use_count)
            big_object.~big_data();
    }
}

my_vector::~my_vector() {
    clear();
}


bool my_vector::is_small() const {
    return small_object.small_s <= MAX_SMALL;
}


size_t my_vector::size() const {
    return is_small() ? small_object.size() : size_;
}


bool my_vector::empty() const {
    return size() == 0;
}

void my_vector::reserve(size_t n) {
    ensure_cap(n);
}

void my_vector::resize(size_t n) {
    unique_check();
    if (n <= MAX_SMALL) {
        if (is_small()) {
            for (size_t i = size(); i < n; ++i) (*this)[i] = 0;
            small_object.small_s = n;
        } else {
            size_ = n;
            big_to_small();
        }
    } else {
        ensure_cap(n);
        for (size_t i = size(); i < n; ++i) {
            (*this)[i] = 0;
        }
        size_ = n;
    }
}


void my_vector::pop_back() {
    unique_check();
    if (is_small()) {
        small_object.pop_back();
    } else {
        size_--;
    }
}


void my_vector::push_back(uint const &value) {
    unique_check();
    ensure_cap(size() + 1);
    if (is_small()) {
        small_object.push_back(value);
    } else {
        big_object.ptr->long_ob[size_] = value;
        size_++;
    }
}


uint &my_vector::back() {
    return (*this)[size() - 1];
}


uint &my_vector::operator[](size_t i) {
    unique_check();
    return is_small() ? small_object[i] : big_object[i];
}


uint const &my_vector::operator[](size_t i) const {
    return is_small() ? small_object[i] : big_object[i];
}


void my_vector::unique_check() {
    if (!is_small() && big_object.ptr->use_count > 1) {
        big_object.ptr->use_count--;
        info *tmp = big_object.ptr;
        big_object.ptr = (info *) operator new(sizeof(info) + 2*big_object.b_capacity * sizeof(uint));
        big_object.ptr->use_count = 1;
        for (int i = 0; i < (int) size(); ++i) {
            big_object.ptr->long_ob[i] = tmp->long_ob[i];
        }
    }
}


void my_vector::ensure_cap(size_t n) {

    if (capacity() < n) {
        size_t cur_size = std::min(size(), n);
        uint *tmp = new uint[cur_size];
        //
        for (size_t i = 0; i < cur_size; ++i) {
            tmp[i] = (*this)[i];
        }
        /*if (is_small()) {
            memcpy(tmp, small_object.small_ob, sizeof(uint) * cur_size);
        } else {
            memcpy(tmp, big_object.ptr->long_ob, sizeof(uint) * cur_size);
        }*/
        //
        clear();
        big_object.ptr = static_cast<info *>(operator new(sizeof(info) + sizeof(uint) * n * 2));
        big_object.ptr->use_count = 1;
        size_ = cur_size;
        big_object.b_capacity = n * 2;
        //
        for (size_t i = 0; i < cur_size; ++i) {
            (*this)[i] = tmp[i];
        }
        //
        delete[] tmp;
    }
}

size_t my_vector::capacity() const {
    return is_small() ? small_object.capacity() : big_object.capacity();
}


void my_vector::big_to_small() {
    info *tmp = big_object.ptr;
    size_t sz = size();
    memcpy(small_object.small_ob, tmp->long_ob, sizeof(uint) * sz);
    //todo
    small_object.small_s = sz;
    tmp->use_count--;
    if (!tmp->use_count) {
        delete tmp;
    }
}