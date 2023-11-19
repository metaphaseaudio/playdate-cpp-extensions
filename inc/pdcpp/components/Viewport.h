//
// Created by Matt on 11/18/2023.
//

#pragma once

#include "Component.h"
#include "pdcpp/graphics/Point.h"
#include <pd_api.h>

namespace pdcpp
{

    class Viewport
        : public Component
    {
    public:
        Viewport();
        explicit Viewport(Component* content);

        void setContent(Component* content);

        void draw() override;

        void setContentOffset(int x, int y);
        void moveContentBy(int x, int y);

    private:
        Component* p_Content;
        int m_OffsetX, m_OffsetY;
    };

} // pdcpp
