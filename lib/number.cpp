#include "number.h"


void PrintError() {
    std::cerr << "Undefined Behaviour";
    exit(EXIT_FAILURE);
}

bool GetSignBit(const int2023_t &num) {
    return (num.block[int2023_t::SIZE - 1] >> (sizeof(num.block[int2023_t::SIZE]) * 8 - 1)) & 1;
}
int GetNotNullIndex(const int2023_t &value){
    int index = 0;
    for (int i = int2023_t::SIZE - 1; i >= 0; --i) {
        if (value.block[i] != 0) {
            index = i;
            break;
        }
    }
    return index;
}

int2023_t from_int(int32_t i) {
    int2023_t num;
    bool is_negative = false;
    if (i < 0) {
        is_negative = true;
        i = -i;
    }
    for (int j = 0; j < int2023_t::SIZE; ++j) {
        num.block[j] = i % int2023_t::BASE;
        i /= int2023_t::BASE;
    }
    if (is_negative) {
        if (GetSignBit(num)) {
            PrintError();
        } else {
             return int2023_t(-num);
        }
    }
    return int2023_t(num);
}

int2023_t from_string(const char* buff) { 
    int2023_t result;
    int2023_t degree = from_int(10);
    int i = 0;
    bool is_negative = false;
    if (!isdigit(buff[i])) {
        ++i;
        is_negative = true;
    }
    for (; i < strlen(buff); ++i) {
        if (isdigit(buff[i])) {
            result = result * degree + from_int(buff[i] - '0');
        } else {
            PrintError();
        }
    }
    if (is_negative) {
        return int2023_t(-result);
    }
    return int2023_t(result);
}

int2023_t GetAbs(const int2023_t& value) {
    if (GetSignBit(value)) {
        return -value;
    }
    return value;
}

int2023_t operator~(const int2023_t& value) {
    int2023_t result;
    for (int i = 0; i < int2023_t::SIZE; ++i) {
        result.block[i] = ~value.block[i];
    }
    return int2023_t(result);
}

int2023_t operator-(const int2023_t& value) {
    int2023_t result;
    result = ~value;
    result.block[0] += 1;
    return int2023_t(result);
}

int2023_t operator+(const int2023_t& lhs, const int2023_t& rhs) {
    bool bit_lhs = GetSignBit(lhs);
    bool bit_rhs = GetSignBit(rhs);
    if (bit_lhs) {
        if (bit_rhs) {
            return -(-lhs + (-rhs));
        }
        return rhs - (-lhs);
    } else if (bit_rhs) {
        return lhs - (-rhs);
    }
    int carry = 0;
    int temporary = 0;
    int2023_t result;
    for (int i = 0; i < int2023_t::SIZE || carry != 0; ++i) {
        temporary = lhs.block[i] + carry + rhs.block[i];
        result.block[i] = lhs.block[i] + carry + rhs.block[i];
        carry = temporary >= int2023_t::BASE;
        if (carry != 0) {
            result.block[i] = temporary - int2023_t::BASE;
        }
    }
    return int2023_t(result);
}

int2023_t operator-(const int2023_t& lhs, const int2023_t& rhs) {
    bool bit_lhs = GetSignBit(lhs);
    bool bit_rhs = GetSignBit(rhs);
    if (bit_rhs) {
        return lhs + (-rhs);
    } else if (bit_lhs) {
        return -(-lhs + rhs);
    } else if (lhs < rhs) {
        return -(rhs - lhs);
    }
    int carry = 0;
    int temporary = 0;
    int2023_t result;
    for (int i = 0; i < int2023_t::SIZE || carry != 0; ++i) {
        temporary = lhs.block[i] - carry - rhs.block[i];
        result.block[i] = lhs.block[i] - carry - rhs.block[i];
        carry = temporary < 0;
    }
    return int2023_t(result);
}

int2023_t operator*(const int2023_t& lhs, const int2023_t& rhs) {
    int2023_t result;
    int2023_t abs_rhs = GetAbs(rhs);
    int2023_t abs_lhs = GetAbs(lhs);
    bool bit_lhs = GetSignBit(lhs);
    bool bit_rhs = GetSignBit(rhs);
    for (int i = 0; i < int2023_t::SIZE; ++i) {
        int carry = 0;
        for (int j = 0; j < int2023_t::SIZE || carry != 0; ++j) {
            if (i + j < int2023_t::SIZE) {
                long long current = result.block[i + j] + abs_lhs.block[i] * 1LL * abs_rhs.block[j] + carry;
                result.block[i + j] = current % int2023_t::BASE;
                carry = current / int2023_t::BASE;
            }
        }
    }
    if (GetSignBit(result)) {
        PrintError();
    }
    if ((int(bit_lhs) + int(bit_rhs)) % 2 == 1 && result != from_int(0)) {
        return int2023_t(-result);
    }
    return int2023_t(result);
}

int2023_t operator/(const int2023_t& lhs, const int2023_t& rhs) {
    int2023_t result;
    int2023_t abs_rhs = GetAbs(rhs);
    int2023_t abs_lhs = GetAbs(lhs);
    bool bit_lhs = GetSignBit(lhs);
    bool bit_rhs = GetSignBit(rhs);
    if (abs_rhs == from_int(0)) {
        PrintError();
    }
    int index = GetNotNullIndex(abs_lhs);
    int left;
    int right;
    int middle;
    int2023_t temporary;
    while (index >= 0) {
        left = -1;
        right = int2023_t::BASE;
        while (left + 1 < right) {
            result.block[index] = (left + right) / 2;
            temporary = result * abs_rhs;
            if (GetSignBit(temporary)) {
                right = result.block[index];
            }
            if (temporary == lhs) {
                return result;
            } else if (temporary > abs_lhs) {
                right = result.block[index];
            } else {
                left = result.block[index];
            }
        }
        result.block[index] = left;
        --index;
    }
    if ((int(bit_lhs) + int(bit_rhs)) % 2 == 1 && result != from_int(0)) {
        result = -result;
    }
    return int2023_t(result);
}

bool operator==(const int2023_t& lhs, const int2023_t& rhs) {
    for (int i = 0; i < int2023_t::SIZE; ++i) {
        if (lhs.block[i] != rhs.block[i]) {
            return false;
        }
    }
    return true;
}

bool operator<(const int2023_t& lhs, const int2023_t& rhs) {
    bool bit_lhs = GetSignBit(lhs);
    bool bit_rhs = GetSignBit(rhs);
    if (lhs == rhs) {
        return false;
    }
    if (bit_lhs) {
        if (bit_rhs) {
            return (-rhs) < (-lhs);
        }
        return true;
    }
    if (bit_rhs) {
        return false;
    }
    for (int i = int2023_t::SIZE - 1; i >= 0; --i) {
        if (lhs.block[i] != rhs.block[i]) {
            return lhs.block[i] < rhs.block[i];
        }
    }
    return false;
}

bool operator!=(const int2023_t& lhs, const int2023_t& rhs) {
    return !(lhs == rhs);
}

bool operator<=(const int2023_t& lhs, const int2023_t& rhs) {
    return (lhs < rhs || lhs == rhs);
}

bool operator>(const int2023_t& lhs, const int2023_t& rhs) {
    return !(lhs <= rhs);
}

bool operator>=(const int2023_t& lhs, const int2023_t& rhs) {
    return !(lhs < rhs);
}


std::ostream& operator<<(std::ostream& stream, const int2023_t& value) {
    bool is_negative = GetSignBit(value);
    int2023_t result = value;
    if (is_negative) {
        stream << '-';
        result = -value;
    } 
    for (int i = int2023_t::SIZE - 1; i >= 0; --i) {
         stream << static_cast<unsigned int>(result.block[i]) << ' ';
    }
    return stream;
}
