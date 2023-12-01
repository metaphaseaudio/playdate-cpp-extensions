//
// Created by Matt on 11/18/2023.
//

#pragma once

#include <vector>
#include <pd_api.h>
#include "pdcpp/core/util.h"

namespace pdcpp
{
    class Component
    {
    public:
        Component() = default;

        void setBounds(PDRect bounds);
        [[ nodiscard ]] PDRect getBounds() const;

        [[ nodiscard ]] PDRect getLocalBounds() const;

        void redraw();

        void addChildComponent(Component* child);
        void removeChildComponent(Component* child);

        [[ nodiscard ]] pdcpp::Component* getChildComponent(int index) const;
        [[ nodiscard ]] size_t childCount() const;
        [[ nodiscard ]] const std::vector<Component*>& getChildren() const;
        void removeAllChildren();

        void resizeToFitChildren();

        virtual bool updateAnimation() { return false; };

    protected:
        virtual void draw() {};
        virtual void resized(PDRect newBounds) {};

    private:
        PDRect m_Bounds = {0, 0, 0, 0};
        std::vector<Component*> m_Children;
        PDCPP_DECLARE_NON_COPYABLE(Component);
    };
}
