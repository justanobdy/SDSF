#pragma once

#include <string>
#include <vector>

#include <cstdio>

#include <SDSF/Exception.hpp>

/// @brief A simple file reader class that uses the C file API
class FileReader {
public:
    using byte = unsigned char;

    /// @brief Default constructor
    FileReader() = default;
    /// @brief Opening constructor
    /// @param filename The filename to open
    /// @param binaryMode If binary mode is enabled or not
    FileReader(const std::string& filename, bool binaryMode = true)
    {
        OpenFile(filename, binaryMode);
    }
    ~FileReader() {
        Close();
    }

    /// @brief Open a file for reading
    /// @param filename The filename
    /// @param binaryMode If binary mode is enabled or not
    void OpenFile(const std::string& filename, bool binaryMode = true) {
        const char* openMode = binaryMode ? "rb" : "r";

        file = fopen(filename.c_str(), openMode);
        if(file == nullptr) {
            throw Exception("Could not open " + filename + "!");
            return;
        }
        
        fseek(file, 0, SEEK_END);
        fileSize = ftell(file);
        fseek(file, 0, SEEK_SET);
    }

    /// @brief Seek to a position in the file
    /// @param position The new position
    void Seek(long position) {
        fseek(file, position, SEEK_SET);
    }

    /// @brief Get the file size
    /// @return The file size
    size_t GetFileSize() const {
        return fileSize;
    }

    /// @brief Get the current cursor position
    /// @return The current cursor position
    size_t GetCursorPosition() const {
        return ftell(file);
    }

    /// @brief Get the amount of bytes left in the file
    /// @return The amount of bytes left in the file
    size_t GetBytesLeft() const {
        return fileSize - GetCursorPosition();
    }

    /// @brief Close the file
    void Close() {
        if(file != nullptr) {
            fclose(file);
            fileSize = 0;
            file = nullptr;
        }
    }

    /// @brief Read a specific amount of numbers
    /// @tparam T The type of number to read
    /// @param numbers The amount of numbers to read
    /// @return The numbers that were read
    template <typename T>
    std::vector<T> ReadNumbers(size_t numbers) {
        canRead(numbers * sizeof(T));

        std::vector<T> data;
        data.assign(numbers, T(0));

        fread(&data[0], sizeof(T), numbers, file);

        return data;
    }

    /// @brief Read an entire file
    /// @tparam T The type of number to return (the file size must be a multiple of sizeof(T))
    /// @return The entire file
    template <typename T>
    std::vector<T> ReadEntireFile() {
        return ReadNumbers<T>(fileSize / sizeof(T));
    }

    /// @brief Read a single number from the file
    /// @tparam T The type of number to read
    /// @return The number that was read
    template <typename T>
    std::vector<T> ReadNumber() {
        canRead(sizeof(T));

        T item;
        
        fread(&item, sizeof(T), 1, file);
        
        return item;
    }

    /// @brief Read a string from the file
    /// @param amount The amount of characters to read
    /// @return The string that was read
    std::string Read(size_t amount) {
        canRead(amount);

        std::string data;
        data.assign(amount, '\0');

        fread(&data[0], sizeof(char), amount, file);

        return data;
    }

    /// @brief Read a raw array
    /// @tparam T The type of array to read
    /// @param data The array
    /// @param amount The amount of numbers to read
    template <typename T>
    void Read(T* data, size_t amount) {
        canRead(amount * sizeof(T));

        fread(data, sizeof(T), amount, file);
    }

    /// @brief Get if the file is open
    /// @return If the file is open
    bool IsOpen() const {
        return file != nullptr;
    }
private:
    /// @brief Check if a specific amount can be read from the file
    /// @param amount The amount of bytes to check for
    /// @return If the amount of bytes can be read or not
    bool canRead(size_t amount) {
        if(!IsOpen()) {
            throw Exception("File is not open!");
        }

        if(GetBytesLeft() < amount) {
            throw Exception("Too many bytes are trying to be read!");
        }

        return true;
    }

    FILE* file = nullptr;
    size_t fileSize = 0;
};