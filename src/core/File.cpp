/**
 *  This file is part of the Playdate CPP Extensions library, and covered under
 *  the license terms found in the LICENSE file at the root of the repository.
 *
 *  Copyright (c) 2023 - Metaphase
 *
 *  Created: 9/3/2023
 *  Original author: MrBZapp
 */

#include <pdcpp/core/File.h>
#include <pdcpp/core/GlobalPlaydateAPI.h>

pdcpp::FileHandle::FileHandle(const std::string& path, FileOptions mode)
{
    auto pd = pdcpp::GlobalPlaydateAPI::get();
    p_File = pd->file->open(path.c_str(), mode);

    if (p_File == nullptr)
        { pdcpp::FileHelpers::handleError("Failed to open file " + path); }

    m_Stat = pdcpp::FileHelpers::stat(path);
}

pdcpp::FileHandle::FileHandle(pdcpp::FileHandle&& other) noexcept
    : m_Stat(other.m_Stat)
    , p_File(other.p_File)
{ other.p_File = nullptr; }

pdcpp::FileHandle& pdcpp::FileHandle::operator=(pdcpp::FileHandle&& other) noexcept
{
    m_Stat = other.m_Stat;
    p_File = other.p_File;
    other.p_File = nullptr;
    return *this;
}

pdcpp::FileHandle::~FileHandle()
{
    if (p_File != nullptr)
    { pdcpp::GlobalPlaydateAPI::get()->file->close(p_File); }
}

int pdcpp::FileHandle::read(void* buffer, unsigned int len)
{
    auto pd = pdcpp::GlobalPlaydateAPI::get();
    auto bytesRead = pd->file->read(p_File, buffer, len);

    if (bytesRead < 0)
        { pdcpp::FileHelpers::handleError("Failed to read from file."); }

    return bytesRead;
}

const FileStat& pdcpp::FileHandle::getDetails() const { return m_Stat; }

int pdcpp::FileHandle::write(void* buffer, unsigned int length)
    { return pdcpp::GlobalPlaydateAPI::get()->file->write(p_File, buffer, length); }

int pdcpp::FileHandle::seek(int position, pdcpp::FileHandle::Whence whence)
    { return pdcpp::GlobalPlaydateAPI::get()->file->seek(p_File, position, whence); }

int pdcpp::FileHandle::tell() const { return pdcpp::GlobalPlaydateAPI::get()->file->tell(p_File); }

void pdcpp::FileHelpers::handleError(const std::string& msg)
{
    auto pd = pdcpp::GlobalPlaydateAPI::get();
    auto errStr = msg + " -- " + pd->file->geterr();
    pd->system->error(errStr.c_str());
}

std::vector<std::string> pdcpp::FileHelpers::listFilesInDirectory(const std::string& dirPath, bool showHidden)
{
    auto pd = pdcpp::GlobalPlaydateAPI::get();

    std::vector<std::string> rv;
    pd->file->listfiles(dirPath.c_str(), [](const char* dirName, void* userData)
    {
        auto v = static_cast<std::vector<std::string>*>(userData);
        v->emplace_back(dirName);
    }, &rv, showHidden);

    return rv;
}

bool pdcpp::FileHelpers::fileExists(const std::string& path)
{
    FileStat stat;
    stat.isdir = true;
    return pdcpp::GlobalPlaydateAPI::get()->file->stat(path.c_str(), &stat) == 0 && !stat.isdir;
}

int pdcpp::FileHelpers::mkdir(const std::string& path)
{
    return pdcpp::GlobalPlaydateAPI::get()->file->mkdir(path.c_str());
}

int pdcpp::FileHelpers::rename(const std::string& from, const std::string& to)
{
    return 0;
}

FileStat pdcpp::FileHelpers::stat(const std::string& path)
{
    FileStat rv;

    auto pd = pdcpp::GlobalPlaydateAPI::get();
    if (pd->file->stat(path.c_str(), &rv) < 0)
        { pdcpp::FileHelpers::handleError("Failed to stat file: " + path); }

    return rv;
}

std::string pdcpp::FileHelpers::parentDir(const std::string& fileOrDirname)
{
    int end = fileOrDirname.size();
    if (fileOrDirname.ends_with('/')) { end--; }
    return fileOrDirname.substr(0, fileOrDirname.substr(0, end).find_last_of('/'));
}
