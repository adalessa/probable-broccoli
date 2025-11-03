#pragma once

#include <vector>
#include <cstdint>

class IEncodeable {
public:
    virtual ~IEncodeable() = default; // Virtual destructor for proper cleanup
    virtual std::vector<uint8_t> encode() const = 0; // Pure virtual method
};
