//
// Created by Matt on 12/17/2023.
//

#pragma once
#include <deque>
#include <pdcpp/core/ButtonManager.h>
#include "CrankManager.h"

namespace pdcpp
{
    class InputContext
        : protected pdcpp::ButtonManager::Listener
        , protected pdcpp::CrankManager::Listener
    {
    public:
        InputContext();
        virtual ~InputContext() = default;
        void popContext();
        virtual void contextEntered() {};
        virtual void contextExited() {};
    protected:
        void pushChildContext(InputContext* context);
    private:
        friend class InputContextManager;
        class InputContextManager* p_CurrentManager;
    };


    class InputContextManager
        : public pdcpp::ButtonManager
        , public pdcpp::CrankManager
    {
    public:
        explicit InputContextManager(InputContext* baseContext, bool crankBeforeButtons=false);

        void update();
        void pushContext(InputContext* newContext);
        void popContext();
        void resetToBaseContext();

    private:
        bool m_CheckCrankBeforeButtons;
        std::deque<InputContext*> m_ContextStack;
    };

}
