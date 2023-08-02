#pragma once

#include <exception>
#include <string>

/// @brief Simple Exception class
class Exception : public std::exception {
public:
    /// @brief Constructor
    /// @param what The error message
    Exception(const std::string& what)
        :   error(what)
        {}
    
    /// @brief Get the error message
    /// @return The error message
    const char* what() const noexcept override {
        return error.c_str();
    }

private:
    const std::string error;
};