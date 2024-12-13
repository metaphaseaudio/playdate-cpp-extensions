/**
 *  This file is part of the Playdate CPP Extensions library, and covered under
 *  the license terms found in the LICENSE file at the root of the repository.
 *
 *  Copyright (c) 2023 - Metaphase
 *
 *  Created: 12/17/2023
 *  Original author: MrBZapp
 */

#include <pdcpp/core/InputContext.h>
#include <cassert>


void pdcpp::InputContext::popContext()
{
    if (p_CurrentManager != nullptr)
        { p_CurrentManager->popContext(); }
}

void pdcpp::InputContext::pushChildContext(pdcpp::InputContext* context)
{
    if (p_CurrentManager == nullptr)
    {
        assert(false);
        return;
    }
    p_CurrentManager->pushContext(context);
}

pdcpp::InputContext::InputContext()
    : p_CurrentManager(nullptr)
{}

pdcpp::InputContextManager::InputContextManager(InputContext* baseContext)
{ pushContext(baseContext); }

void pdcpp::InputContextManager::update()
{
    if (m_ContextStack.back()->wantsCrankBeforeButtons())
    {
        pdcpp::CrankManager::checkStateAndNotify();
        pdcpp::ButtonManager::checkStateAndNotify();
    }
    else
    {
        pdcpp::ButtonManager::checkStateAndNotify();
        pdcpp::CrankManager::checkStateAndNotify();
    }
}

void pdcpp::InputContextManager::pushContext(InputContext* newContext)
{
    if (!m_ContextStack.empty())
    {
        pdcpp::ButtonManager::removeListener(m_ContextStack.back());
        pdcpp::CrankManager::removeListener(m_ContextStack.back());
        m_ContextStack.back()->contextExited();
    }

    newContext->p_CurrentManager = this;
    m_ContextStack.push_back(newContext);
    pdcpp::ButtonManager::addListener(m_ContextStack.back());
    pdcpp::CrankManager::addListener(m_ContextStack.back());

    newContext->contextEntered();
}

void pdcpp::InputContextManager::popContext()
{
    if (m_ContextStack.size() <= 1) { return; }

    InputContext* lastContext = m_ContextStack.back();
    m_ContextStack.pop_back();

    pdcpp::ButtonManager::removeListener(lastContext);
    pdcpp::CrankManager::removeListener(lastContext);
    lastContext->p_CurrentManager = nullptr;
    pdcpp::ButtonManager::addListener(m_ContextStack.back());
    pdcpp::CrankManager::addListener(m_ContextStack.back());


    lastContext->contextExited();
    m_ContextStack.back()->contextEntered();
}

void pdcpp::InputContextManager::resetToBaseContext()
{
    while (m_ContextStack.size() != 1)
        { popContext(); }
}

pdcpp::InputContext* pdcpp::InputContextManager::getCurrentContext() const
{
    return m_ContextStack.back();
}

