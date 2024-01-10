/**
 *  This file is part of the Playdate CPP Extensions library, and covered under
 *  the license terms found in the LICENSE file at the root of the repository.
 *
 *  Copyright (c) 2023 - Metaphase
 *
 *  Created: 11/18/2023
 *  Original author: MrBZapp
 */
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
        /**
         * Components are simple UI elements that are not Sprites. They dictate
         * how to draw an element, but not when. The key difference is that
         * Components can have a hierarchy of children where Sprites cannot: the
         * system will call redraw on a Sprite regardless of if it is "owned" by
         * any other sprite, where as a Component's redraw method is called by
         * you, through its hierarchy.
         */
        Component() = default;

        /**
         * Set the bounds of the component.
         *
         * @param bounds the rectangle representing the bounds of the component
         *    in space.
         */
        void setBounds(pdcpp::Rectangle<float> bounds);

        /**
         * @returns a pdcpp::Rectangle<float> of the current bounds of this
         *     component.
         */
        [[ nodiscard ]] pdcpp::Rectangle<float> getBounds() const;

        /**
         * @return The bounds of this component with an origin at 0,0. Use this
         *     when you're unsure of where your component will be drawn on the
         *     screen, but want to maintain the same shape.
         */
        [[ nodiscard ]] pdcpp::Rectangle<float> getLocalBounds() const;

        /**
         * Draw the component and all of its children.
         */
        void redraw();

        /**
         * Add a component as a child of this component.
         *
         * @param child a pointer to the Component to add to the hierarchy.
         */
        void addChildComponent(Component* child);

        /**
         * Remove a Component from this Component's hierarchy. Does nothing if
         * the provided pointer is not a child of this Component.
         *
         * @param child a pointer to the child to remove from the hierarchy.
         */
        void removeChildComponent(Component* child);

        /**
         * Returns the nth component that is an immediate child of this
         * Component.
         *
         * @param index the index of the child Component to retrieve.
         * @return the child Component.
         */
        [[ nodiscard ]] pdcpp::Component* getChildComponent(int index) const;

        /**
         * @returns the current parent of the component. If the component has no
         * parent, it will return a nullptr.
         */
        [[ nodiscard ]] pdcpp::Component* getParentComponent() const;

        /**
         * @returns the number of immediate children of this Component.
         */
        [[ nodiscard ]] size_t childCount() const;

        /**
         * @returns a reference to the vector of immediate children of this
         *     Component. Note that the vector cannot be altered, but the
         *     children can be modified.
         */
        [[ nodiscard ]] const std::vector<Component*>& getChildren() const;

        /**
         * Remove all the children from this Component.
         */
        void removeAllChildren();

        /**
         * Change the bounds of this component to ensure that all children fit
         * within it.
         */
        void resizeToFitChildren();

        /**
         * Set the current "Look and Feel" of this component. Useful for having
         * multiple instances of the same Component which draw differently
         * without having to resort to inheritance.
         *
         * @param newLAF a pointer to the new LookAndFeel object for this
         *     Component will use.
         */
        void setLookAndFeel(LookAndFeel* newLAF);

        /**
         * @returns a pointer to the current LookAndFeel. Will return the
         *     default LookAndFeel if none have been expressly set on this
         *     Component.
         */
        [[ nodiscard ]] pdcpp::LookAndFeel* getLookAndFeel() const;

        /**
         * Called when the look and feel anywhere in the hierarchy changes.
         * Override this to react to changes.
         */
        virtual void lookAndFeelChanged() {};

    protected:
        /**
         * Draw your component in this method. It will be called whenever
         * something in the hierarchy calls `redraw()`.
         */
        virtual void draw() {};

        /**
         * Will be called whenever a new set of boundaries has been set for this
         * Component to allow inheritors to react to changes in the hierarchy.
         *
         * @param newBounds the new boundaries set for this Component.
         */
        virtual void resized(const pdcpp::Rectangle<float>& newBounds) {};


    private:
        pdcpp::Rectangle<float> m_Bounds = {0, 0, 0, 0};
        std::vector<Component*> m_Children;
        Component* p_Parent = nullptr;
        pdcpp::LookAndFeel* m_CustomLookAndFeel = nullptr;
        PDCPP_DECLARE_NON_COPYABLE(Component);
    };
}
