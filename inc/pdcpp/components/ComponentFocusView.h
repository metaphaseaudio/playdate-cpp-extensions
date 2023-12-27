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

    private:
        class FocusContainer;
        std::unique_ptr<pdcpp::Component> m_FocusContainer;

        PDCPP_DECLARE_NON_COPYABLE(ComponentFocusView);
    };
}
