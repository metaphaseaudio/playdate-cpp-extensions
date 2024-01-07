/**
 *  This file is part of the Playdate CPP Extensions library, and covered under
 *  the license terms found in the LICENSE file at the root of the repository.
 *
 *  Copyright (c) 2023 - Metaphase
 *
 *  Created: 9/24/2023
 *  Original author: MrBZapp
 */
#pragma once

#include <functional>
#include <vector>
#include <pd_api.h>
#include "Timer.h"

namespace pdcpp
{
    /**
     * A class to enable a "listener" pattern for button state modification.
     * This patter allows the logic for button input management to be spread out
     * across multiple classes, minimizing the need for a monolithic and
     * repetitive button-management function/method in the main update routine.
     *
     * Create one, add a bunch of listeners, and call `checkStateAndNotify` in
     * your game's main update loop. All your listeners will be informed when
     * the button state changes.
     */
    class ButtonManager
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
            virtual void buttonStateChanged(const PDButtons& current, const PDButtons& pressed, const PDButtons& released) {};
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

    class KeyRepeatTimer
        : public pdcpp::Timer
    {
    public:
        /**
         * Creates a KeyRepeatTimer. These timers take a function in the
         * `keyPressed which will be hit once on key-press, once after the
         * `initialDelayMs` time has elapsed, and then repeatedly at intervals
         * specified by the `repeatDelayMs` time. Use this to allow users to do
         * things like press-and-hold an arrow key to move a cursor across a
         * keyboard.
         *
         * @param initialDelayMs the initial time in milliseconds to wait before
         *     repeating the key-press action.
         * @param repeatDelayMs the interval time to repeat the key-press action
         *     after the initial delay has elapsed.
         */
        explicit KeyRepeatTimer(int initialDelayMs=300, int repeatDelayMs=100);

        /**
         * Start the key-press action and timer.
         *
         * @param action the action to execute
         * @param resetTimer whether or not to reset the timer. This can be
         *     used to changing the action without interrupting the repetition
         *     such as a user changing directions on a D-pad to a diagonal.
         */
        void keyPressed(std::function<void()> action, bool resetTimer=true);

        /**
         * Stops and resets the timer to its 'ready' state.
         */
        void keyReleased();

    private:
        void timerCallback() override;

        std::function<void()> m_OnKeyRepeat;
        int m_InitialDelay, m_RepeatDelay;
    };
}
