/**
 *  This file is part of the Playdate CPP Extensions library, and covered under
 *  the license terms found in the LICENSE file at the root of the repository.
 *
 *  Copyright (c) 2023 - Metaphase
 *
 *  Created: 12/17/2023
 *  Original author: MrBZapp
 */

#pragma once
#include <deque>
#include <pdcpp/core/ButtonManager.h>
#include "CrankManager.h"
#include <pdcpp/core/util.h>

namespace pdcpp
{
    class InputContext
        : protected pdcpp::ButtonManager::Listener
        , protected pdcpp::CrankManager::Listener
    {
    public:
        /**
         * Defines the current state of user input when handled by an
         * `InputContextManager`.
         */
        InputContext();

        // Default virtual destructor
        virtual ~InputContext() = default;

        /**
         * Tells the context manager to leave this context. Note that this only
         * works if this is the currently active context.
         */
        void popContext();

        /**
         * Called by the context manager whenever this context is entered. Use
         * it to set up things for your new context, show sprites, etc.
         */
        virtual void contextEntered() {};

        /**
         * Called by the context manger when leaving this context in either
         * direction. Use it to clean up your context, hide sprites, etc.
         */
        virtual void contextExited() {};

        /**
         * Tells the context manager this input context wants to be notified of
         * changes in the crank before changes in the buttons. This is not the
         * case by default.
         */
        [[ nodiscard ]] virtual bool wantsCrankBeforeButtons() const { return false; };

    protected:
        /**
         * Allows for cascading context ownership. Use this to push a context
         * onto the stack as a child context.
         *
         * @param context the context to push.
         */
        void pushChildContext(InputContext* context);
    private:
        friend class InputContextManager;
        class InputContextManager* p_CurrentManager;

        /**
         * Input context managers keep track of contexts by pointer, so we can't
         * let you go moving them around all willy-nilly.
         */
        PDCPP_DECLARE_NON_COPYABLE_NON_MOVABLE(InputContext);
    };


    class InputContextManager
        : public pdcpp::ButtonManager
        , public pdcpp::CrankManager
    {
    public:

        /**
         * Use one of these in your application to quickly swap out what the
         * controls do for a given scenario, which is useful for redirecting
         * control flow to menus, and such things. Make one at your base App
         * layer, and call `update` during every update system call.
         *
         * @param baseContext the "default" context to define what the controls
         *     do normally.
         */
        explicit InputContextManager(InputContext* baseContext);

        /**
         * Call this during your application's update routine to notify the
         * active context of any user input changes.
         */
        void update();

        /**
         * Push a new context onto the stack. The previous context will be
         * notified its context has exited, and the new context will be notified
         * it has taken over control of the input.
         *
         * @param newContext the new `InputContext` to receive input updates.
         */
        void pushContext(InputContext* newContext);

        /**
         * Pops the top-most context off the stack, unless it's the base
         * context, passing context input to the next context in the stack. The
         * popped context will be notified of the exit, and the new context will
         * be notified of entry.
         */
        void popContext();

        /**
         * Pops *all* contexts off the stack, down to the base context,
         * following the enter/exit context chain the whole way down.
         */
        void resetToBaseContext();

        /**
         * @return pointer to the current context.
         */
        [[ nodiscard ]] InputContext* getCurrentContext() const;

    private:
        std::deque<InputContext*> m_ContextStack;
    };

}
