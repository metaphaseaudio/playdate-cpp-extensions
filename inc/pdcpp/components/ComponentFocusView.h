//
// Created by Matt on 11/20/2023.
//

#pragma once
#include <memory>
#include "Viewport.h"

namespace pdcpp
{
    class ComponentFocusView
        : public pdcpp::Component
    {
    public:
        ComponentFocusView();
        void setComponentFocus(size_t index, int transitionFrames=0);
        [[ nodiscard ]] int getComponentFocusIndex() const;

        void addChildToFocusContainer(pdcpp::Component* child);
        void removeChildFromFocusContainer(pdcpp::Component* child, bool resetFocus=true);

        bool updateTransition();

    protected:
        void resized(PDRect newBounds) override;

    private:
        void resizeFocusContainer();
        class FocusContainer;
        std::unique_ptr<pdcpp::Component> m_FocusContainer;
        pdcpp::Viewport m_Viewport;
        int m_FocusIndex = -1;
        int m_TransitionFramesRemaining = 0;
    };
}
