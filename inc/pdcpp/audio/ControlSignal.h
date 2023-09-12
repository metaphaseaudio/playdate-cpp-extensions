/**
 *  This file is part of the Playdate CPP Extensions library, and covered under
 *  the license terms found in the LICENSE file at the root of the repository.
 *
 *  Copyright (c) 2023 - Metaphase
 *
 *  Created: 10/29/2023
 *  Original author: MrBZapp
 */

#pragma once
#include <pd_api.h>
#include "Signal.h"


namespace pdcpp
{
    class ControlSignalContainer
        : public Signal
    {
    public:
        /**
         * Creates a ControlSignalContainer for a pre-existing C API pointer to
         * a ControlSignal. A container only provides the API access to the
         * pointer, and does not own the pointer.
         *
         * @param signal the ::ControlSignal* to use
         */
        explicit ControlSignalContainer(::ControlSignal* signal);

        /**
         * Clears all events from the signal.
         */
        void clearEvents();

        /**
         * Adds a value to the signal’s timeline.
         *
         * @param step the step at which to change value
         * @param value the new value
         * @param interpolate enable interpolation between the previous
         *     step+value and this one
         */
        void addEvent(int step, float value, bool interpolate);

        /**
         * Removes an event from the timeline
         *
         * @param step the step which should be removed
         */
        void removeEvent(int step);

        /**
         * @returns the MIDI controller number for this ControlSignal
         */
        int getMIDIControllerNumber();

        // Implements virtual method
        [[ nodiscard ]] operator ::PDSynthSignalValue*() const override;  // NOLINT (*-explicit-constructor)

        /**
         * @returns if the control signal pointed to by this container is
         *     addressable.
         */
        bool isValid() { return p_Signal != nullptr; }

    protected:
        ::ControlSignal* p_Signal;
    };

    class ControlSignal
        : public ControlSignalContainer
    {
    public:
        /**
         * A ControlSignal is a ControlSignalContainer which owns its C API
         * pointer, and will free the memory when the destructor is called.
         */
        ControlSignal();

        /**
         * Frees the internal pointer.
         */
        ~ControlSignal();

        // Move constructor
        ControlSignal(ControlSignal&& other) noexcept;

        // Move-assignment constructor
        ControlSignal& operator=(ControlSignal&& other) noexcept;

    private:
        PDCPP_DECLARE_NON_COPYABLE(ControlSignal);
    };
}
