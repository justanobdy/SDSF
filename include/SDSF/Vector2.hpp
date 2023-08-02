#pragma once

/// @brief Simple Vector2 class
/// @tparam T The number type to use
template <typename T>
class Vector2 {
public:
    /// @brief The x value
    T x;
    /// @brief The y value
    T y;

    /// @brief Constructor which sets the x and y value
    /// @param x The new x value
    /// @param y The new y value
    Vector2(T x, T y)
        :   x(x), y(y)
        {}
    
    /// @brief Default constructor (sets x and y to 0)
    Vector2()
    :   x(T(0)), y(T(0))
        {}
};