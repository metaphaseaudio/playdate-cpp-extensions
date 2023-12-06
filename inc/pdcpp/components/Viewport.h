//
// Created by Matt on 11/18/2023.
//

#pragma once

#include "Component.h"
#include "pdcpp/graphics/Point.h"
#include "pdcpp/graphics/Sprite.h"
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

        void setViewPosition(int x, int y);
        [[ nodiscard ]] pdcpp::Point<int> getViewPosition() const;

        void setContentOffset(int x, int y);
        [[ nodiscard ]] pdcpp::Point<int> getContentOffset() const;

        void moveContentBy(int x, int y);

    private:
        Component* p_Content;
        int m_OffsetX, m_OffsetY;
    };

} // pdcpp
