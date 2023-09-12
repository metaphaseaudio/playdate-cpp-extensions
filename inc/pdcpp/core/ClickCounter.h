/**
 *  This file is part of the Playdate CPP Extensions library, and covered under
 *  the license terms found in the LICENSE file at the root of the repository.
 *
 *  Copyright (c) 2023 - Metaphase
 *
 *  Created: 9/25/2023
 *  Original author: MrBZapp
 */

#pragma once
#include <pd_api.h>


namespace pdcpp
{
    class ClickCounter
    {
    public:
        /**
         * Creates a ClickCounter. This class keeps track of all the new button
         * presses for one or a combination of buttons within a window, and
         * returns how many clicks were detected. It is very useful for
         * detecting and handling double clicks, or more if your user interface
         * requires it.
         *
         * @param toDetect the button or button combination to detect
         * @param checkWindow the window within which unique button presses will
         *     be counted
         * @param nClickMax an optional early-return setting. ie. if you know
         *     you want to detect double clicks, set this to 2, and the `detect`
         *     method will return 2 after the second click, even if the window
         *     of detection hasn't closed yet. default is 0: "count them all"
         */
        ClickCounter(PDButtons toDetect, unsigned int checkWindow, unsigned int nClickMax=0);

        /**
         * Call in your button handling routines to detect clicks.
         *
         * @param pressed the pressed buttons for this update cycle
         * @param released the released buttons for this update cycle
         * @param checkTime the time at which the buttons were checked
         * @return how many (or the max) clicks were detected within the window
         */
        unsigned int detect(const PDButtons& pressed, const PDButtons& released, unsigned int checkTime);

        /**
         * Resets the detection window if it's in progress.
         */
        void reset();

    private:
        PDButtons m_ButtonsToDetect;
        bool m_Up;
        unsigned int m_CheckWindowMS, m_MsChecked, m_ClicksDetected, m_NClickMax;
    };
}
