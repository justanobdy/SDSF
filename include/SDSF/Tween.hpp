#pragma once

#include <SDSF/fpm/fpm.hpp>

using FloatType = fpm::fixed_16_16;

// Trash tween class I scribbled up really quick

inline FloatType Lerp(FloatType start, FloatType end, FloatType percentage) {
    return (start + (end - start) * percentage);
}

inline FloatType Flip(FloatType t) {
    return 1 - t;
}

/// @brief Base class for tweens
class TweenBase {
public:
    virtual FloatType calculate(FloatType num) = 0;
};

/// @brief Class for Ease In Tweens
class EaseInClass : public TweenBase {
public:
    FloatType calculate(FloatType num) override {
        return num * num * num * num;
    }
};

/// @brief Class for Ease Out Tweens
class EaseOutClass : public TweenBase {
public:
    FloatType calculate(FloatType num) override {
        num = Flip(num);
        return Flip(num * num * num * num);
    }
};

/// @brief Class for In Back tweens
class InBackClass : public TweenBase {
public:
    FloatType calculate(FloatType num) override {
        constexpr FloatType s(1.7);

        return num * num * ((s + 1) * num - s);
    };
};


/// @brief Class for Linear Tweens
class LinearClass : public TweenBase {
public:
	FloatType calculate(FloatType num) override {
		return num;
	}
};


/// @brief Simple tween class
/// @tparam NumberType The number type this uses (TODO: remove)
/// @tparam TweenType The type of tween this is
template <typename NumberType, typename TweenType>
class Tween {
public:
    /// @brief Default constructor
    Tween() = default;

    /// @brief Reset the tween
    void Reset() {
        step = 0;
    }

    /// @brief Create a new tween
    /// @param start The starting number
    /// @param end The ending number
    /// @param totalSteps The total steps this tween takes to get there
    void Create(NumberType start, NumberType end, FloatType totalSteps) {
        this->start = start;
        this->end = end;
        this->totalSteps = totalSteps;
        this->step = FloatType(0);
    }

    /// @brief Step the tween
    /// @param amount The amount to step the tween
    /// @return The amount after the step was taken
    FloatType Step(FloatType amount) {
        if(step + amount < totalSteps)
            step += amount;
		else
			step = totalSteps;
        return GetValue();
    }

    /// @brief Get the value of the tween
    /// @return The value of the tween
    FloatType GetValue() const {
        return Lerp(FloatType(start), FloatType(end), TweenType().calculate(step / totalSteps));
    }

	/// @brief Get the current step the tween is on
	/// @return The current step the tween is on
	FloatType GetStep() const {
		return step;
	}

	/// @brief Get if the tween is done
	/// @return If the tween is done
	bool Done() const {
		return step == totalSteps;
	}
private:
    FloatType step = FloatType(0);
    FloatType totalSteps = FloatType(0);
    NumberType start = 0;
    NumberType end = 0;
};