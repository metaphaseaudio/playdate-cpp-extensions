//
// Created by Matt on 11/18/2023.
//

#pragma once

#include <vector>
#include <pd_api.h>
#include "pdcpp/core/util.h"
#include "pdcpp/graphics/LookAndFeel.h"
#include "pdcpp/graphics/Rectangle.h"

namespace pdcpp
{
    class Component
    {
    public:
        Component() = default;

        void setBounds(pdcpp::Rectangle<float> bounds);
        [[ nodiscard ]] pdcpp::Rectangle<float> getBounds() const;

        [[ nodiscard ]] pdcpp::Rectangle<float> getLocalBounds() const;

        void redraw();

        void addChildComponent(Component* child);
        void removeChildComponent(Component* child);

        [[ nodiscard ]] pdcpp::Component* getChildComponent(int index) const;
        [[ nodiscard ]] size_t childCount() const;
        [[ nodiscard ]] const std::vector<Component*>& getChildren() const;
        void removeAllChildren();

        void resizeToFitChildren();

        void setLookAndFeel(LookAndFeel* newLAF);
        pdcpp::LookAndFeel* getLookAndFeel() const;

    protected:
        virtual void draw() {};
        virtual void resized(const pdcpp::Rectangle<float>& newBounds) {};

    private:
        pdcpp::Rectangle<float> m_Bounds = {0, 0, 0, 0};
        std::vector<Component*> m_Children;
        pdcpp::LookAndFeel* m_CustomLookAndFeel = nullptr;
        PDCPP_DECLARE_NON_COPYABLE(Component);
    };
}
