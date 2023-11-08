/**
 *  This file is part of the Playdate CPP Extensions library, and covered under
 *  the license terms found in the LICENSE file at the root of the repository.
 *
 *  Copyright (c) 2023 - Metaphase
 *
 *  Created: 8/24/2023
 *  Original author: MrBZapp
 */

#pragma once


namespace pdcpp
{
    class Timer
    {
    public:
        /**
         * Creates a timer which will measure, to the best of it's ability, a
         * time interval, after which its callback will be triggered. There
         * may be variance due to system load, the callback is guaranteed not to
         * be triggered before the interval, but is not guaranteed to be
         * triggered exactly on the time interval. This makes it ideal for
         * longer-term measurements where some jitter is acceptable.
         *
         * @param intervalMs the number of milliseconds to allow to elapse
         *     triggering the callback
         * @param startEnabled optionally defer the enablement of the timer
         */
        explicit Timer(unsigned int intervalMs, bool startEnabled=true);

        /**
         * Change the interval of this timer, optionally resetting the timer.
         *
         * @param intervalMs the new interval in milliseconds
         * @param reset optionally resets the timer
         */
        void setInterval(unsigned int intervalMs, bool reset=false);

        /**
         * @returns the millisecond interval between timer callbacks.
         */
        [[ nodiscard ]] int getInterval() const { return m_Interval; };

        /**
         * Turns the timer on or off. Will not reset the frame count if disabled
         * in the middle of a countdown.
         *
         * @param shouldEnable enable/disable the timer
         */
        void enable(bool shouldEnable);

        /**
         * Check the time and fire the callback if the time has elapsed. Higher
         * tick rates will ensure better timing accuracy.
         */
        void tick();

    protected:
        /**
         * Pure virtual. Will be called after the interval has elapsed.
         */
        virtual void timerCallback() = 0;

    private:
        unsigned int m_LastMilliseconds, m_Interval;
        bool m_Enabled;
    };

////////////////////////////////////////////////////////////////////////////////

    class FrameTimer
    {
    public:
        /**
         * Similar to a `Timer`, but operates on a fixed count of ticks, as
         * opposed to relying on the system time. This is ideal for triggering
         * events on fixed frame counts, or reducing the effective refresh rate
         * for sprites, etc.
         *
         * @param nFrames the number of `tick` calls to wait before calling the
         *     `timerCallback` method.
         * @param startEnabled optionally defer the enablement of the timer
         */
        explicit FrameTimer(unsigned int nFrames, bool startEnabled=true);

        /**
         * Changes the number of tick calls before hitting the callback,
         * optionally resetting the count.
         *
         * @param nFrames the new count to use.
         * @param reset whether the count should be reset to 0
         */
        void setNFrames(unsigned int nFrames, bool reset=false);

        /**
         * @returns the current number of frames in a cycle
         */
        [[ nodiscard ]] int getNFrames() const { return m_NFrames; };

        /**
         * Turns the timer on or off. Will not reset the frame count if disabled
         * in the middle of a countdown.
         *
         * @param shouldEnable enable/disable the timer
         */
        void enable(bool shouldEnable);

        /**
         * Call this to advance the timer. Once the timer has been `tick`'d
         * the set number of times, the `timerCallback` will be hit
         */
        void tick();

    protected:
        /**
         * Pure virtual. Will be called after the number of ticks/frames has
         * elapsed
         */
        virtual void timerCallback() = 0;

    private:
        unsigned int m_NFrames, m_FramesRemaining;
        bool m_Enabled;
    };
}
