/**
 *  This file is part of the Playdate CPP Extensions library, and covered under
 *  the license terms found in the LICENSE file at the root of the repository.
 *
 *  Copyright (c) 2023 - Metaphase
 *
 *  Created: 11/20/2023
 *  Original author: MrBZapp
 */
#pragma once
#include <memory>
#include "Viewport.h"

namespace pdcpp
{
    class ComponentFocusView
        : public pdcpp::Viewport
    {
    public:
        /**
         * Makes a ComponentFocusView. This component is like a viewport, but
         * instead of one single content component, it can hold multiple
         * Components, and provides an API which automatically brings one of the
         * content components into view. This is used for classes like the
         * GridView, where panning/scrolling is required to bring cells into
         * view.
         */
        ComponentFocusView();

        /**
         * Brings the given Component into the center of the view.
         *
         * @param index the index of the component to center.
         */
        void setComponentFocus(size_t index);

        /**
         * Brings a potentially out-of-view component into the viewport. Does
         * not center the component.
         *
         * @param index the index of the component to bring into view.
         */
        void bringComponentIntoView(size_t index);

        /**
         * @param index the index of the component to query
         * @returns whether or not this component is currently fully in the
         *     viewport
         */
        [[ nodiscard ]] bool isComponentVisible(size_t index) const;

        /**
         * Adds a focusable Component
         * @param child a pointer to the Component to add as a child.
         */
        void addChildToFocusContainer(pdcpp::Component* child);

        /**
         * Removes the child from the container, if found.
         * @param child a pointer to the child to remove
         */
        void removeChildFromFocusContainer(pdcpp::Component* child);

        /**
         * Removes all children from the container.
         */
        void clearFocusView();

    private:
        class FocusContainer;
        std::unique_ptr<pdcpp::Component> m_FocusContainer;

        PDCPP_DECLARE_NON_COPYABLE(ComponentFocusView);
    };
}
