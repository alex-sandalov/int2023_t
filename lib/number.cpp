#include <cstring>
#include "number.h"

uint8_t GetSize(const int2023_t& value) {
    uint8_t lenght = 0;
    for (uint8_t i = 0; i < int2023_t::kMaxSizeNumber; ++i) {
        if (value.number[i] != 0x0) {
            lenght = i + 1;
        }
    }
    return lenght;
}

uint8_t GetSign(const int2023_t& hs) {
    return 1 << 7 & hs.number[int2023_t::kMaxSizeNumber - 1];
}

int2023_t operator-(const int2023_t& value) {
    int2023_t result;
    for (uint8_t i = 0; i < int2023_t::kMaxSizeNumber; ++i) {
        result.number[i] = ~value.number[i];
    }
    return result + from_int(1);
}

int2023_t from_int(int32_t i) {
    bool is_negative = false;
    if (i < 0) {
        is_negative = true;
        i = -i;
    }
    int2023_t result;
    uint8_t buffer = 0x0;
    uint8_t index = 0;
    while (i) {
        int curr = i % 2;
        if (curr) {
            buffer ^= (1 << ((index + int2023_t::kBase) % int2023_t::kBase));
        }
        index++;
        if (!(index % int2023_t::kBase) && index) {
            result.number[index / int2023_t::kBase - 1] = buffer;
            buffer &= 0;
        }
        i /= 2;
    }
    if (index % int2023_t::kBase) {
        result.number[index / int2023_t::kBase] = buffer;
    }
    return is_negative ? -result : result;
}

int2023_t from_string(const char* buff) {
    bool is_negative = (*buff == '-');
    const char* buf = buff + is_negative;
    char buffer[int2023_t::kSizeBase];
    std::memset(buffer, '0', int2023_t::kSizeBase);
    int64_t num[int2023_t::kSizeBaseNumber] = {};
    size_t lenght = std::strlen(buf);
    int64_t index = lenght - 1;
    int64_t ind_num;
    while (index >= 0) {
        ind_num = (lenght - index) - 1;
        if (!(ind_num % int2023_t::kSizeBase) && ind_num) {
            num[ind_num / int2023_t::kSizeBase - 1] = std::atoll(buffer);
            std::memset(buffer, '0', int2023_t::kSizeBase);
        }
        buffer[int2023_t::kSizeBase - ind_num % int2023_t::kSizeBase - 1] = *(buf + index);
        index--;
    }
    if (std::strlen(buffer)) { 
        num[ind_num / int2023_t::kSizeBase] = std::atoll(buffer);
    }
    int2023_t number_from_string;
    int64_t carry = 0;
    int64_t index_binary = 0;
    uint8_t buffer_binary = 0x0;
    while (num[0]) {
        for (int i = int2023_t::kSizeBaseNumber - 1; i >= 0; i--) {
        	long long cur = num[i] + carry * int2023_t::kBaseNumber;
        	num[i] = cur >> 1;
        	carry = cur & 1;
        }
        if (carry) {
            buffer_binary ^= (1 << ((index_binary + int2023_t::kBase) % int2023_t::kBase));
        }
        index_binary++;
        if (!(index_binary % int2023_t::kBase) && index_binary) {
            number_from_string.number[index_binary / int2023_t::kBase - 1] = buffer_binary;
            buffer_binary = 0;
        }
        carry = 0;
    }
    if (index_binary % int2023_t::kBase) {
        number_from_string.number[index_binary / int2023_t::kBase] = buffer_binary;
    }
    return is_negative ? -number_from_string : number_from_string;
}

int2023_t operator+(const int2023_t& lhs, const int2023_t& rhs) {
    int2023_t answer;
    int next_rank = 0;
    uint8_t buffer = 0x0;
    for (uint8_t i = 0; i < int2023_t::kMaxSizeNumber; i++) {
        for (uint8_t j = 0; j < int2023_t::kBase; j++) {
            uint8_t lhs_number_sign = 1 << j & lhs.number[i];
            uint8_t rhs_number_sign = 1 << j & rhs.number[i];
            if (lhs_number_sign && rhs_number_sign) {
                if (next_rank) {
                    next_rank--;
                    buffer ^= (1 << j);
                }
                next_rank++;
            } else {
                if (!next_rank && (lhs_number_sign || rhs_number_sign)) {
                    buffer ^= (1 << j);
                } else if (next_rank && (!lhs_number_sign && !rhs_number_sign)) {
                    buffer ^= (1 << j);
                    next_rank--;
                }
            }  
        }
        answer.number[i] = buffer;
        buffer = 0;
    }
    return answer;
}

int2023_t operator-(const int2023_t& lhs, const int2023_t& rhs) {
    return lhs + (-rhs);
}

int2023_t MakeBinaryOffset(const int2023_t hs) {
    bool is_next = false;
    int2023_t ans;
    for (uint8_t i = 0; i < int2023_t::kMaxSizeNumber; i++) {
        ans.number[i] = hs.number[i] << 1;
        if (i && (hs.number[i - 1] & int2023_t::kMaxSizeUint8_t )) {
            ans.number[i] ^= int2023_t::kOne;
        }
    }
    return ans;
}

int2023_t operator*(const int2023_t& lhs, const int2023_t& rhs) {
    int2023_t answer;
    uint8_t lhs_sign = GetSign(lhs);
    uint8_t rhs_sign = GetSign(rhs);
    int2023_t lhs_t = lhs;
    int2023_t rhs_t = rhs;
    if (lhs_sign && rhs_sign) {
        lhs_t = -lhs_t;
        rhs_t = -rhs_t;
    }
    for (uint8_t i = 0; i < int2023_t::kMaxSizeNumber; i++) {
        for (uint8_t j = 0; j < int2023_t::kBase; j++) {
            if ((1 << j) & rhs_t.number[i]) {
                answer = answer + lhs_t;
            }
            lhs_t = MakeBinaryOffset(lhs_t);
        }
    }
    return answer;
}
int2023_t DividingShortNumbers(int2023_t number, int divider) {
    uint16_t carry = 0x0;
    int2023_t answer;
    for (int16_t i = int2023_t::kMaxSizeNumber - 1; i >= 0; i--) {
        uint16_t cur = number.number[i] + carry;
        answer.number[i] = cur / divider;
        carry = cur % divider;
        if (carry) {
            carry = int2023_t::kMaxSizeUint8_tPlusOne;
        }
    }
    return answer;
}

bool operator<(const int2023_t& lhs, const int2023_t& rhs) {
    for (int16_t i = int2023_t::kMaxSizeNumber - 1; i >= 0; i--) {
        if (lhs.number[i] != rhs.number[i]) {
            return lhs.number[i] < rhs.number[i];
        }
    }
    return false;
}

bool operator<=(const int2023_t& lhs, const int2023_t& rhs) {
    return !(rhs < lhs);
}

int2023_t operator/(const int2023_t& lhs, const int2023_t& rhs) {
    int2023_t lhs_t = lhs;
    int2023_t rhs_t = rhs;
    int2023_t answer;
    if (answer == rhs) {
        return answer;
    }
    bool is_negative = false;
    uint8_t lhs_sign = GetSign(lhs);
    uint8_t rhs_sign = GetSign(rhs);
    if (lhs_sign && !rhs_sign) {
        lhs_t = -lhs_t;
        is_negative = true;
    } 
    if (rhs_sign && !lhs_sign) {
        rhs_t = -rhs_t;
        is_negative = true;
    }
    if (lhs_sign && rhs_sign) {
        lhs_t = -lhs_t;
        rhs_t = -rhs_t;
    }
    uint32_t max_size_number = int2023_t::kMaxSizeNumber * int2023_t::kBase - 1;
    uint32_t right = max_size_number;
    int32_t left = max_size_number;
    int2023_t value;
    int2023_t carry;
    int2023_t ans;
    while (left >= 0) {
        value = MakeBinaryOffset(value);
        if (lhs_t.number[left / int2023_t::kBase] & (1 << (left % int2023_t::kBase))) {
            value.number[0] ^= int2023_t::kOne;
        }
        --left;
        if (rhs_t <= value) {
            carry = value - rhs_t;
            right = left;
            ans.number[(left + 1) / int2023_t::kBase] ^= (1 << ((left + 1) % int2023_t::kBase));
            value = carry;
        }
    }
    return is_negative ? -ans : ans;
}

bool operator==(const int2023_t& lhs, const int2023_t& rhs) {
    for (int i = 0; i < int2023_t::kMaxSizeNumber; i++) {
        if (lhs.number[i] != rhs.number[i]) {
            return false;
        }
    }
    return true;
}

bool operator!=(const int2023_t& lhs, const int2023_t& rhs) {
    return !(lhs == rhs);
}

std::ostream& operator<<(std::ostream& stream, const int2023_t& value) {
    for (uint8_t i = GetSize(value); i >= 0; i--) {
        for (int j = int2023_t::kBase - 1; j >= 0; j--) {
            if ((1 << j) & value.number[i]) {
                stream << '1';
            } else {
                stream << '0';
            }
        }
    }
    return stream;
}
