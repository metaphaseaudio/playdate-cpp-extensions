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
        void popContext();
        virtual void contextEntered() {};
        virtual void contextExited() {};

    private:
        friend class InputContextManager;
        InputContextManager* p_CurrentManager = nullptr;
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
