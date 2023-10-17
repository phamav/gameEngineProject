#pragma once
#include "Component.h"
class JennComponent :
    public Component
{
    public:
        JennComponent();
        virtual void initialize() override;
        virtual void update(const float& deltaTime) override;
        virtual void processInput() override;
};

