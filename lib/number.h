#pragma once
#include <cinttypes>
#include <iostream>

struct int2023_t {
    const static uint8_t kBase = 8;
    const static uint8_t kSizeBase = 9;
    const static uint8_t kMaxSizeNumber = 253;
    const static uint32_t kBaseNumber = 1e9;
    const static uint8_t kSizeBaseNumber = 68;
    const static uint8_t kMaxSizeUint8_t = 0b10000000;
    const static uint8_t kOne = 0b00000001;
    const static uint16_t kMaxSizeUint8_tPlusOne = 0b100000000;

    uint8_t number[kMaxSizeNumber] = {};
};

static_assert(sizeof(int2023_t) <= 253, "Size of int2023_t must be no higher than 253 bytes");

int2023_t from_int(int32_t i);

int2023_t from_string(const char* buff);

int2023_t operator+(const int2023_t& lhs, const int2023_t& rhs);

int2023_t operator-(const int2023_t& lhs, const int2023_t& rhs);

int2023_t operator*(const int2023_t& lhs, const int2023_t& rhs);

int2023_t operator/(const int2023_t& lhs, const int2023_t& rhs);

bool operator==(const int2023_t& lhs, const int2023_t& rhs);

bool operator!=(const int2023_t& lhs, const int2023_t& rhs);

std::ostream& operator<<(std::ostream& stream, const int2023_t& value);
