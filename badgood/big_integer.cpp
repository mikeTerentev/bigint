#include "big_integer.h"
#include "my_vector.h"
void delete_fake_0(vector &tmp) {
    while (tmp.size() > 1 && tmp.back() == 0)
        tmp.pop_back();
}

void big_integer::zero_must_not_crash(big_integer &a) {
    if (a.data.size() == 1 && a.data[0] == 0) {
        a.sign = false;
    }
}

//< --true, > -- false
bool big_integer::is_first_arg_smaller(vector const &left, vector const &right) {
    for (size_t i = left.size(); i > 0; --i) {
        if (left[i - 1] == right[i - 1]) continue;
        return left[i - 1] < right[i - 1];
    }
    return false;
}

void big_integer::sub_same_size_digits(vector &first, vector const &second) {
    uint cf = 0;
    for (size_t i = 0; i < second.size(); ++i) {
        long long sum = first[i] * 1LL - cf - second[i];
        cf = sum < 0 ? 1 : 0;
        first[i] = static_cast<uint>(sum < 0 ? base + sum : sum);
    }
}

bool big_integer::is_absFirst_bigger_absSec(big_integer const &first, big_integer const &second) const {
    if (first.data.size() != second.data.size()) return first.data.size() > second.data.size();

    for (int i = (int) first.data.size() - 1; i >= 0; --i) {
        if (first.data[i] > second.data[i]) {
            return true;
        }
        if (second.data[i] > first.data[i]) {
            return false;
        }
    }
    return false;
}

big_integer::big_integer() {
    *this = big_integer(0);
}

big_integer::big_integer(big_integer const &num) : data(num.data), sign(num.sign) {}

big_integer::big_integer(int num) : sign(num < 0) {
    long long tmp = num;
    if (sign) tmp *= -1;
    data.push_back(static_cast<uint>(tmp));
}

big_integer::big_integer(std::string const &line) : data(), sign(false) {
    std::string digit = line[0] == '-' ? line.substr(1, line.size() - 1) : line;
    for (char i : digit) {
        mul_long_short(10);
        *this += (int) (i - '0');
    }
    sign = (line[0] == '-');
    zero_must_not_crash(*this);
}

big_integer::~big_integer() = default;

big_integer &big_integer::operator=(big_integer const &num) = default;


void big_integer::mul_long_short(uint arg) {
    uint carry_digit = 0;
    vector result(data);
    for (size_t i = 0; i < result.size() || carry_digit > 0; ++i) {
        if (i == result.size()) result.push_back(0);

        ull current = result[i] * 1ull * arg + carry_digit;
        result[i] = static_cast<uint>(current % base);
        carry_digit = static_cast<uint>(current / base);
    }
    delete_fake_0(result);
    data = result;
}


bool operator<(big_integer const &fst_arg, big_integer const &sc_arg) {
    if (fst_arg.sign) {
        return !sc_arg.sign ? true : sc_arg.is_absFirst_bigger_absSec(fst_arg, sc_arg);
    } else {
        return sc_arg.sign ? false : sc_arg.is_absFirst_bigger_absSec(sc_arg, fst_arg);
    }
}

bool operator==(big_integer const &fst_arg, big_integer const &sc_arg) {
    return !(fst_arg > sc_arg) && !(fst_arg < sc_arg);
}

bool operator!=(big_integer const &fst_arg, big_integer const &sc_arg) {
    return !(fst_arg == sc_arg);
}

bool operator>(big_integer const &fst_arg, big_integer const &sc_arg) {
    return sc_arg < fst_arg;
}

bool operator<=(big_integer const &fst_arg, big_integer const &sc_arg) {
    return fst_arg < sc_arg || fst_arg == sc_arg;
}

bool operator>=(big_integer const &fst_arg, big_integer const &sc_arg) {
    return !(fst_arg < sc_arg);
}


void big_integer::add_module(big_integer const &a, big_integer const &b, vector &result) {
    result = a.data;
    uint carry = 0;
    for (size_t i = 0; carry || i < std::max(result.size(), b.data.size()); ++i) {
        if (i == result.size()) result.push_back(0);
        ll current_digit;
        current_digit = result[i] * 1ll + carry + (i < b.data.size() ? b.data[i] : 0);
        carry = current_digit >= base ? 1 : 0;
        if (carry) current_digit -= base;
        result[i] = static_cast<uint>(current_digit);
    }
    delete_fake_0(result);
}


void big_integer::subtract_module(big_integer const &a, big_integer const &b, vector
&result) {
    result = a.data;
    int carry = 0;
    for (size_t i = 0; carry || (i < b.data.size()); ++i) {
        ll current_digit = result[i] * 1ll - (carry * 1ll + (i < b.data.size() ? b.data[i] : 0));
        carry = current_digit < 0 ? 1 : 0;
        if (carry) current_digit += base;
        result[i] = static_cast<uint>(current_digit);
    }
    delete_fake_0(result);
}


big_integer &big_integer::operator+=(big_integer const &arg) {
    vector result;
    if (sign == arg.sign) {
        add_module(*this, arg, result);
    } else if (is_absFirst_bigger_absSec(*this, arg)) {
        subtract_module(*this, arg, result);
    } else {
        subtract_module(arg, *this, result);
        sign = arg.sign;
    }
    data = result;
    zero_must_not_crash(*this);
    return *this;
}


big_integer &big_integer::operator-=(big_integer const &arg) {
    vector memory;
    if (sign != arg.sign) {
        add_module(*this, arg, memory);
    } else if (is_absFirst_bigger_absSec(*this, arg)) {
        subtract_module(*this, arg, memory);
    } else {
        subtract_module(arg, *this, memory);
        this->sign = !arg.sign;
    }
    data = memory;
    zero_must_not_crash(*this);
    return *this;
}

big_integer operator+(big_integer fst_arg, big_integer const &sc_arg) {
    fst_arg += sc_arg;
    return fst_arg;
}

big_integer operator-(big_integer fst_arg, big_integer const &sc_arg) {
    fst_arg -= sc_arg;
    return fst_arg;
}

big_integer operator*(big_integer fst_arg, big_integer const &sc_arg) {
    fst_arg *= sc_arg;
    return fst_arg;
}

big_integer operator%(big_integer fst_arg, big_integer const &sc_arg) {
    fst_arg %= sc_arg;
    return fst_arg;
}

big_integer operator&(big_integer a, big_integer const &b) {
    a &= b;
    return a;
}

big_integer operator|(big_integer a, big_integer const &b) {
    a |= b;
    return a;
}

big_integer operator^(big_integer a, big_integer const &b) {
    a ^= b;
    return a;
}


void big_integer::mul_vector_and_short(vector const &first, uint const &second, vector &res) {
    uint c_digit = 0;
    res.resize(first.size() + 1);
    for (size_t i = 0; i < first.size(); ++i) {
        ull cur = c_digit + first[i] * 1ULL * second;
        res[i] = static_cast<uint>(cur % base);
        c_digit = static_cast<uint>(cur / base);
    }
    res[first.size()] = c_digit;
}

uint big_integer::calculate_without_overflow(uint f, uint s, uint c) {
    ull res = f;
    res = ((res << (32ull)) + s) / c;
    res = std::min(res, base * 1ull - 1);
    return static_cast<uint>(res);
}

big_integer big_integer::do_div(big_integer const &first, big_integer const &second) {
    big_integer abs_f(first.data, false), abs_s(second.data, false);
    size_t f_size = first.data.size(), s_size = second.data.size();
    if (s_size > f_size) return 0;

    auto f = static_cast<uint> ((UINT32_MAX * 1ULL + 1) / (abs_s.data.back() * 1ULL + 1));//normalization
    abs_f.mul_long_short(f);
    abs_s.mul_long_short(f);

    size_t len = f_size - s_size + 1;
    vector data_result(len);
    vector dividend(s_size + 1), div(s_size + 1);

    for (size_t i = 0; i < s_size; ++i)
        dividend[i] = abs_f.data[f_size + i - s_size];

    dividend[s_size] = abs_f.data.size() > f_size ? abs_f.data[f_size] : 0;
    for (size_t i = 0; i < len; ++i) {
        dividend[0] = abs_f.data[f_size - s_size - i];
        size_t res_ind = len - 1 - i;
        uint key = calculate_without_overflow(dividend[s_size], dividend[s_size - 1], abs_s.data.back());
        mul_vector_and_short(abs_s.data, key, div);
        while (is_first_arg_smaller(dividend, div)) {
            mul_vector_and_short(abs_s.data, --key, div);
        }
        sub_same_size_digits(dividend, div);
        for (size_t j = s_size; j > 0; --j) dividend[j] = dividend[j - 1];
        data_result[res_ind] = key;
    }
    delete_fake_0(data_result);
    return big_integer(data_result, first.sign ^ second.sign);
}


big_integer &big_integer::operator/=(big_integer const &arg) {
    if (is_absFirst_bigger_absSec(*this, arg)) {
        *this = do_div(*this, arg);
    } else {
        sign = false;
        data = vector(1, 0);
    }
    return *this;
}

big_integer &big_integer::operator%=(big_integer const &other) {
    big_integer tmp(*this);
    *this = tmp - (tmp / other) * other;
    return *this;
}


void big_integer::set_same_size(size_t size, vector
&first, vector &second) {
    while (first.size() < size) first.push_back(0);
    while (second.size() < size) second.push_back(0);
}

void big_integer::addition2two(vector &tmp, bool sign) {
    if (sign) {
        for (int i = 0; i < tmp.size(); i++) tmp[i] = ~tmp[i];
        ll cur_digit;
        uint c_digit = 1;
        for (size_t i = 0; c_digit != 0; i++) {
            cur_digit = c_digit + tmp[i];
            tmp[i] = static_cast<uint>(cur_digit >= base ? cur_digit - base : cur_digit);
            c_digit = cur_digit >= base ? 1 : 0;
        }
    }
}

big_integer &big_integer::operator&=(big_integer const &arg) {
    return binary_operation(arg, AND);
}

big_integer &big_integer::binary_operation(big_integer const &arg, type x) {
    vector sec_mem(arg.data);

    size_t sz = std::max(data.size(), sec_mem.size());
    set_same_size(sz, data, sec_mem);
    addition2two(data, this->sign);
    addition2two(sec_mem, arg.sign);
    switch (x) {
        case AND: {
            for (size_t i = 0; i < data.size(); ++i) data[i] = data[i] & sec_mem[i];
            sign = (this->sign) & arg.sign;
            break;
        }
        case OR: {
            for (size_t i = 0; i < data.size(); ++i) data[i] = data[i] | sec_mem[i];
            sign = (this->sign) | arg.sign;
            break;
        }
        case XOR: {
            for (size_t i = 0; i < data.size(); ++i) data[i] = data[i] ^ sec_mem[i];
            sign = (this->sign) ^ arg.sign;
            break;
        }
    }

    addition2two(data, sign);
    delete_fake_0(data);
    return *this;
}

big_integer &big_integer::operator|=(big_integer const &arg) {
    return binary_operation(arg, OR);
}

big_integer &big_integer::operator^=(big_integer const &arg) {
    return binary_operation(arg, XOR);
}


big_integer &big_integer::operator*=(big_integer const &secondNum) {
    sign = (sign) ^ (secondNum.sign);
    zero_must_not_crash(*this);

    vector tmp(data.size() + secondNum.data.size());
    for (size_t i = 0; i < data.size(); ++i) {
        uint c_digit = 0;
        for (int j = 0; j < (int) secondNum.data.size() || c_digit; ++j) {
            ull cur;
            if (j < (int) secondNum.data.size()) {
                cur = tmp[i + j] * 1ull
                      + data[i] * 1ull * secondNum.data[j]
                      + c_digit;
            } else {
                cur = tmp[i + j] * 1ull
                      + data[i] * 1ull * 0
                      + c_digit;
            }
            tmp[i + j] = static_cast<uint> (cur % base);
            c_digit = static_cast<uint> (cur / base);
        }
    }

    delete_fake_0(tmp);
    data = tmp;

    return *this;
}


std::ostream &operator<<(std::ostream &s, big_integer const &a) {
    s << to_string(a);
    return s;
}


big_integer big_integer::operator+() const {
    return *this;
}


big_integer big_integer::operator-() const {
    big_integer tmp = *this;
    tmp.sign ^= true;
    tmp.zero_must_not_crash(tmp);
    return tmp;
}


big_integer &big_integer::operator++() {
    *this += 1;
    return *this;
}


big_integer big_integer::operator++(int) {
    big_integer tmp = *this;
    ++(*this);
    return tmp;
}


big_integer &big_integer::operator--() {
    *this -= 1;
    return *this;
}


big_integer big_integer::operator--(int) {
    big_integer tmp = *this;
    --(*this);
    return tmp;
}


big_integer big_integer::operator~() const {
    big_integer tmp = -(*this + 1);
    return tmp;
}


big_integer &big_integer::operator<<=(int rank) {
    int add = rank / 32;
    rank %= 32;
    vector temp;
   // temp.reserve(static_cast<unsigned long>(add));
    for (int i = 0; i < add; i++) {
        temp.push_back(0);
    }
    for (unsigned int i = 0; i < this->data.size(); i++) {
        temp.push_back(this->data[i]);
    }
    this->data = temp;
    this->mul_long_short(static_cast<uint>(1) << rank);

    return *this;
}


big_integer::big_integer(vector const &a, bool sign) : data(a), sign(sign) {}


big_integer &big_integer::operator>>=(int ind) {
    uint delta = 0;
    if (sign) *this -= 1;
    vector result;
    size_t start = 0;
    while (ind >= 32) {
        start = static_cast<size_t>(ind / (32));
        ind %= 32;
    }
    for (size_t i = data.size() - 1; (i >= start) && (i < data.size()); --i) {
        uint newdigit = static_cast<uint>((data[i] >> ind) & ((1ll << (32 - ind)) - 1ll));//curr
        newdigit |= delta;  //make union using 2 parts
        result.push_back(newdigit);
        delta = (data[i] & (static_cast<uint>((1ll << ind) - 1))) << (32 - ind); //other
    }
    // std::reverse(result.begin(), result.end());
    this->data = result;
    if (this->sign) *this -= 1;
    delete_fake_0(data);
    return *this;
}


big_integer operator/(big_integer first, big_integer const &second) {
    first /= second;
    return first;

}


big_integer operator<<(big_integer fst_arg, int sc_arg) {
    fst_arg <<= sc_arg;
    return fst_arg;
}


big_integer operator>>(big_integer fst_arg, int sc_arg) {
    fst_arg >>= sc_arg;
    return fst_arg;
}

std::string to_string(big_integer const &big_number) {
    vector a = big_number.data;
    std::string string_line;
    while (true) {
        uint c_digit = 0;
        for (auto i = (int) a.size() - 1; i >= 0; i--) {
            ll cur_digit = a[i] + c_digit * 1ll * base;
            a[i] = static_cast<uint>(cur_digit / 10);
            c_digit = static_cast<uint>(cur_digit % 10);
        }
        while (a.size() > 1 && a.back() == 0) a.pop_back();
        string_line += (char) (c_digit + '0');
        if (a.size() == 1 && a[0] == 0) break;
    }

    if (big_number.sign && string_line != "0") string_line += '-';
    reverse(string_line.begin(), string_line.end());
    return string_line;
}