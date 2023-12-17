//
// Created by Matt on 12/17/2023.
//

#pragma once
#include <vector>

namespace pdcpp
{
    class CrankManager
    {
    public:
        /**
         * Base class for anything that cares about button state. Inherit from
         * this class, implement the `buttonStateChanged` method to define
         * what your class should do if the button state changes, and add it as
         * a listener to the game's ButtonManager.
         */
        class Listener
        {
        public:
            /**
             * Pure virtual. Will be called if any of the current, pressed, or
             * released button states are active during a given update cycle.
             *
             * @param current the buttons that are currently down
             * @param pressed the buttons that have transitioned to a down state
             *     since the last cycle
             * @param released the buttons that have transitioned to and up
             *     state since the last cycle
             */
            virtual void crankStateChanged(float absolute, float delta) {};
        };

        /**
         * Call this in your game's main update routine to notify all the
         * objects which care about the button state.
         */
        void checkStateAndNotify();

        /**
         * Add a listener to this manager.
         *
         * @param toAdd a pointer to the listener to add
         */
        void addListener(Listener* toAdd);

        /**
         * Remove a listener from this manager. It is important to call this
         * before deleting a listener to ensure that the manager knows not to
         * call it anymore.
         *
         * @param toRemove the listener to remove
         */
        void removeListener(Listener* toRemove);

    private:
        std::vector<Listener*> m_Listeners;
    };
}
