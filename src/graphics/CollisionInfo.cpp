/**
 *  This file is part of the Playdate CPP Extensions library, and covered under
 *  the license terms found in the LICENSE file at the root of the repository.
 *
 *  Copyright (c) 2023 - Metaphase
 *
 *  Created: 8/30/2023
 *  Original author: MrBZapp
 */

#include <pdcpp/graphics/CollisionInfo.h>
#include <pdcpp/core/GlobalPlaydateAPI.h>

pdcpp::CollisionInfo::CollisionInfo(SpriteCollisionInfo* info, float x, float y, int nCollisions)
    : m_X(x)
    , m_Y(y)
    , m_N(nCollisions)
    , p_Info(info)
{}

pdcpp::CollisionInfo::~CollisionInfo()
{
    if (p_Info != nullptr)
        { pdcpp::GlobalPlaydateAPI::get()->system->realloc(p_Info, 0); }
}

pdcpp::CollisionInfo::CollisionInfo(pdcpp::CollisionInfo&& other) noexcept
    : m_X(other.m_X)
    , m_Y(other.m_Y)
    , m_N(other.m_N)
    , p_Info(other.p_Info)
{ other.p_Info = nullptr; }

pdcpp::CollisionInfo& pdcpp::CollisionInfo::operator=(pdcpp::CollisionInfo&& other) noexcept
{
    m_X = other.m_X;
    m_Y = other.m_Y;
    m_N = other.m_N;
    p_Info = other.p_Info;
    other.p_Info = nullptr;
    return *this;
}
