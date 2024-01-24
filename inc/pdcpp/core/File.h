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

        /**
         * Creates the given path in the `Data/<gameid> folder. Does not create
         * intermediate folders
         *
         * @param path the path to create
         * @return 0 on success, -1 on failure.
         */
        static int mkdir(const std::string& path);

        /**
         * Renates a file from one name to another. Will overwrite the
         * destination name without confirmation.
         *
         * @param from the file to rename
         * @param to the new name of the file
         * @return 0 on success, -1 on failure.
         */
        static int rename(const std::string& from, const std::string& to);

        /**
         * Stats a path to indicate whether the file exists and is not a
         * directory.
         *
         * @param path the filepath to check
         * @return true if the file exists, false if it does not.
         */
        static bool fileExists(const std::string& path);

        /**
         * Run stat on a file without opening it
         *
         * @param path the path to stat
         * @return a FileStat object describing the file
         */
        static FileStat stat(const std::string& path);

        /**
         * Checks what error was retured by the filesystem and prints it to the
         * error stream along with an additional message
         *
         * @param msg the message to prepend to the error
         */
        static void handleError(const std::string& msg);
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
