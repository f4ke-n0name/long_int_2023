#pragma once
#include <cinttypes>
#include <iostream>


struct int2023_t {
    uint8_t block[253] = { 0 };
    const static int BASE = 256;
    const static int SIZE = 253;
};

static_assert(sizeof(int2023_t) <= 253, "Size of int2023_t must be no higher than 253 bytes");

bool GetSignBit(const int2023_t& num);

int GetNotNullIndex(const int2023_t& value);

int2023_t from_int(int32_t i);

int2023_t from_string(const char* buff);

int2023_t GetAbs(const int2023_t& value);

int2023_t operator~(const int2023_t& value); 

int2023_t operator-(const int2023_t& lhs);

int2023_t operator+(const int2023_t& lhs, const int2023_t& rhs);

int2023_t operator-(const int2023_t& lhs, const int2023_t& rhs);

int2023_t operator*(const int2023_t& lhs, const int2023_t& rhs);

int2023_t operator/(const int2023_t& lhs, const int2023_t& rhs);

bool operator==(const int2023_t& lhs, const int2023_t& rhs);

bool operator<(const int2023_t& lhs, const int2023_t& rhs);

bool operator!=(const int2023_t& lhs, const int2023_t& rhs);

bool operator<=(const int2023_t& lhs, const int2023_t& rhs);

bool operator>(const int2023_t& lhs, const int2023_t& rhs);

bool operator>=(const int2023_t& lhs, const int2023_t& rhs);

std::ostream& operator<<(std::ostream& stream, const int2023_t& value);
