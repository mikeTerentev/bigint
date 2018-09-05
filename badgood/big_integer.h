#pragma once

#include <vector>
#include <string>
#include <algorithm>
#include "my_vector.h"
using vector = my_vector;


using uint = unsigned int;
using ull = unsigned long long;
using ll=long long;
//using vector = std::vector<unsigned int>;
const long long base = 1LL << 32;

struct big_integer {


    big_integer();

    big_integer(big_integer const &num);

     big_integer(int num);

    big_integer(vector const &, bool);

    explicit big_integer(std::string const &line);

    ~big_integer();

    big_integer &operator=(big_integer const &num);

    big_integer &operator+=(big_integer const &arg);

    big_integer &operator-=(big_integer const &arg);

    big_integer &operator*=(big_integer const &secondNum);

    big_integer &operator/=(big_integer const &arg);

    big_integer &operator%=(big_integer const &rhs);

    big_integer &operator&=(big_integer const &second);

    big_integer &operator|=(big_integer const &rhs);

    big_integer &operator^=(big_integer const &arg);

    big_integer &operator<<=(int rank);

    big_integer &operator>>=(int ind);

    big_integer operator+() const;

    big_integer operator-() const;

    big_integer operator~() const;

    big_integer &operator++();

    big_integer operator++(int);

    big_integer &operator--();

    big_integer operator--(int);

    friend bool operator==(big_integer const &a, big_integer const &b);

    friend bool operator!=(big_integer const &a, big_integer const &b);

    friend bool operator<(big_integer const &a, big_integer const &b);

    friend bool operator>(big_integer const &a, big_integer const &b);

    friend bool operator<=(big_integer const &a, big_integer const &b);

    friend bool operator>=(big_integer const &a, big_integer const &b);

    friend std::string to_string(big_integer const &big_number);

    vector data;
    bool sign;
private:

    void
    mul_vector_and_short(vector const &first, unsigned int const &second, vector &res);

    big_integer do_div(big_integer const &, big_integer const &);


    void add_module(big_integer const &a, big_integer const &b, vector &result);

    void subtract_module(big_integer const &a, big_integer const &b, vector &result);

    void set_same_size(size_t, vector &, vector &);

    void addition2two(vector &, bool);

    void mul_long_short(unsigned int arg);

    bool is_absFirst_bigger_absSec(big_integer const &first, big_integer const &second) const;

    void zero_must_not_crash(big_integer &);

    unsigned int calculate_without_overflow(unsigned int, unsigned int, unsigned int);

    bool is_first_arg_smaller(vector const &, vector const &);

    void sub_same_size_digits(vector &, vector const &);

    enum type {
        XOR, OR, AND
    };

    big_integer &binary_operation(const big_integer &arg, type x);
};

big_integer operator+(big_integer a, big_integer const &b);

big_integer operator-(big_integer a, big_integer const &b);

big_integer operator*(big_integer a, big_integer const &b);

big_integer operator/(big_integer first, big_integer const &second);

big_integer operator%(big_integer a, big_integer const &b);

big_integer operator&(big_integer a, big_integer const &b);

big_integer operator|(big_integer a, big_integer const &b);

big_integer operator^(big_integer a, big_integer const &b);

big_integer operator<<(big_integer a, int b);

big_integer operator>>(big_integer a, int b);

std::ostream &operator<<(std::ostream &s, big_integer const &a);