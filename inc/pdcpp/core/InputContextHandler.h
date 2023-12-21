//
// Created by Matt on 12/17/2023.
//

#pragma once
#include <deque>
#include <pdcpp/core/ButtonManager.h>
#include "CrankManager.h"

namespace pdcpp
{
    class InputContextHandler
        : protected pdcpp::ButtonManager::Listener
        , protected pdcpp::CrankManager::Listener
    {
    public:
        InputContextHandler();
        virtual ~InputContextHandler() = default;
        void popContext();
        virtual void contextEntered() {};
        virtual void contextExited() {};
    protected:
        void pushChildContext(InputContextHandler* context);
    private:
        friend class InputContextManager;
        class InputContextManager* p_CurrentManager;
    };


    class InputContextManager
        : public pdcpp::ButtonManager
        , public pdcpp::CrankManager
    {
    public:
        explicit InputContextManager(InputContextHandler* baseContext, bool crankBeforeButtons=false);

        void update();
        void pushContext(InputContextHandler* newContext);
        void popContext();

    private:
        bool m_CheckCrankBeforeButtons;
        std::deque<InputContextHandler*> m_ContextStack;
    };

}
