//
// Created by Matt on 11/20/2023.
//

#pragma once
#include <memory>
#include "Viewport.h"

namespace pdcpp
{
    class ComponentFocusView
        : public pdcpp::Viewport
    {
    public:
        ComponentFocusView();
        void setComponentFocus(size_t index);
        void bringComponentIntoView(size_t index);
        [[ nodiscard ]] bool isComponentVisible(size_t index) const;

        void addChildToFocusContainer(pdcpp::Component* child);
        void removeChildFromFocusContainer(pdcpp::Component* child, bool resetFocus=true);
        void clearFocusView();



        [[ nodiscard ]] const pdcpp::Viewport& getViewport() const { return m_Viewport; }

    protected:
        void resized(const pdcpp::Rectangle<float>& newBounds) override;

    private:
        class FocusContainer;
        std::unique_ptr<pdcpp::Component> m_FocusContainer;
        pdcpp::Viewport m_Viewport;
    };
}
