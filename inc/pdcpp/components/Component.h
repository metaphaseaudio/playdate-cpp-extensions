//
// Created by Matt on 11/18/2023.
//

#pragma once

#include <vector>
#include <pd_api.h>

namespace pdcpp
{
    class Component
    {
    public:
        void setBounds(PDRect bounds);
        [[ nodiscard ]] virtual PDRect getBounds() const;

        void redraw();

        void addChild(Component* child);
        void removeChild(Component* child);

    protected:
        virtual void draw() {};
        virtual void resized(PDRect newBounds) {};

    private:
        PDRect m_Bounds = {0, 0, 0, 0};
        std::vector<Component*> m_Children;
    };
}
