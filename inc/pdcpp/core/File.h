/**
 *  This file is part of the Playdate CPP Extensions library, and covered under
 *  the license terms found in the LICENSE file at the root of the repository.
 *
 *  Copyright (c) 2023 - Metaphase
 *
 *  Created: 9/3/2023
 *  Original author: MrBZapp
 */

#pragma once
#include <memory>
#include <streambuf>
#include <string>
#include <vector>

#include <pd_api.h>
#include "util.h"

namespace pdcpp
{
    class FileHandle
    {
    public:
        enum Whence
        {
            CURRENT = SEEK_CUR,
            END = SEEK_END,
            SET = SEEK_SET,
        };

        /**
         * RIIA handle for reading/writing files to the Playdate's filesystem.
         *
         * @param path std::string of the path to open
         * @param mode the PlaydateSDK FileOptions for the opening mode to use
         */
        FileHandle(const std::string& path, FileOptions mode);

        // Move constructor
        FileHandle(FileHandle&& handle) noexcept;

        // Move-assignment constructor
        FileHandle& operator=(FileHandle&& other) noexcept;

        // Destructor. closes the file
        ~FileHandle();

        /**
         * Read bytes from the file into the buffer. Will log a system error
         * on failure.
         *
         * @param buffer a pointer to the buffer to fill
         * @param length the number of bytes to read
         * @return how many bytes were read
         */
        int read(void* buffer, unsigned int length);

        /**
         * Write bytes from a buffer into the file. Will log a system error
         * on failure.
         *
         * @param buffer a pointer to the buffer to write to the file
         * @param length the number of bytes to write
         * @return how many bytes were written
         */
        int write(void* buffer, unsigned int length);

        /**
         * Sets the read/write head to the given position relative to the
         * `whence` setting
         *
         * @param position N bytes relative to the `whence` setting to move the
         *     read/write head.
         * @param whence the reference position (end, current, or absolute)
         */
        int seek(int position, Whence whence);

        /**
         * @return the current position of the read/write head
         */
        [[ nodiscard ]] int tell() const;

        /**
         * @return the PlaydateSDK FileStat block describing the properties
         *     of the current file
         */
        [[ nodiscard ]] const FileStat& getDetails() const;

    private:
        static void handleError(const std::string& msg) ;
        FileStat m_Stat{};
        SDFile* p_File;

        PDCPP_DECLARE_NON_COPYABLE(FileHandle);
    };

    class FileHelpers
    {
    public:
        /**
         * Returns a list of files in the directory, if it exists and is a
         * directory, otherwise an error will print to the console.
         *
         * @param dirPath the path of which to list the contents
         * @param showHidden optionally include hidden files in the list.
         *     default is false.
         * @return a vector of strings for each path in this directory
         */
        static std::vector<std::string> listFilesInDirectory(const std::string& dirPath, bool showHidden=false);

        static int mkdir(const std::string& path, bool make_parents=false);
        static int rename(const std::string& from, const std::string& to);

    };

    template <std::size_t Size>
    struct PlaydateReadFileStreamBuffer
        : std::streambuf
    {
        explicit PlaydateReadFileStreamBuffer(const std::string& filename)
            : m_Handle(filename, kFileRead)
        {}

        int underflow() override
        {
            if (m_EOF)
                { return traits_type::eof(); }

            auto read_size = m_Handle.read(m_Buffer, Size);
            if (read_size < Size)
                { m_EOF = true; }

            setg(m_Buffer, m_Buffer, m_Buffer + read_size);
            return *gptr();
        }

        static std::unique_ptr<PlaydateReadFileStreamBuffer<Size>> makeStreamBuffer(const std::string& filepath)
        { return std::make_unique<PlaydateReadFileStreamBuffer<Size>>(filepath); }

    private:
        pdcpp::FileHandle m_Handle;
        char m_Buffer[Size] = {0};
        bool m_EOF = false;
    };
}
