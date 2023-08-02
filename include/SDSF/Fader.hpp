#pragma once

#include <SDSF/Tween.hpp>

#include <nds/arm9/video.h>

/// @brief Simple Fader class
/// @tparam TweenType The type of Tween to use
template <typename TweenType>
class Fader {
public:
	/// @brief Update the fader (call each frame)
	void Update() {
		if(tween.Done()) {
			fading = false;
		}

		if(fading) {
			tween.Step(FloatType(1));

			setBrightness(3, (int)tween.GetValue());
		}
	}

	/// @brief Start a fade in
	/// @param steps The amount of steps to use
	/// @param white Fade to white (instead of black)
	void FadeIn(uint16_t steps = 16, bool white = false) {
		tween.Create(0, white == true ? 16 : -16, FloatType(steps));
		
		fading = true;
	}

	/// @brief Start a fade out
	/// @param steps The amount of steps to use
	/// @param white Fade to white (instead of black)
	void FadeOut(uint16_t steps = 16, bool white = false) {
		tween.Create(white == true ? 16 : -16, 0, FloatType(steps));

		fading = true;
	}

	/// @brief Stop the fade
	void Stop() {
		fading = false;
	}

	/// @brief Get if the fade is done
	/// @return If the fade is done
	bool Done() const {
		return fading && tween.Done();
	}

	/// @brief Get if the fader is currently fading
	/// @return If the fader is fading
	bool Fading() const {
		return fading;
	}
private:
	Tween<int, TweenType> tween;

	bool fading = false;
};