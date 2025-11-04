#pragma once

#include "Input.h"

class IComponent {
public:
    virtual void update(Input input) = 0;
    virtual ~IComponent() = default;
};
