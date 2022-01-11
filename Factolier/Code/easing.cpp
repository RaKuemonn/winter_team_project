#include "easing.h"

//==============================================================================
//
//		Easing.cpp
//
//      Easing Function 早見表
//      URL https://easings.net/ja
//
//      ※下記サイトのコードを参考にしています（double型を全てfloat型に変更済み）
//      URL https://qiita.com/hart_edsf/items/962ac03281b871dcc0df
//
//==============================================================================


//イージング関数
namespace easing
{
	float in_quad(float t, float totaltime, float max, float min)
	{
		max -= min;
		t /= totaltime;
		return max * t*t + min;
	}
	float out_quad(float t, float totaltime, float max, float min)
	{
		max -= min;
		t /= totaltime;
		return -max * t*(t - 2) + min;
	}
	float in_out_quad(float t, float totaltime, float max, float min)
	{
		max -= min;
		t /= totaltime;
		if (t / 2 < 1)
			return max / 2 * t * t + min;
		--t;
		return -max * (t * (t - 2) - 1) + min;
	}
	float in_cubic(float t, float totaltime, float max, float min)
	{
		max -= min;
		t /= totaltime;
		return max * t*t*t + min;
	}
	float out_cubic(float t, float totaltime, float max, float min)
	{
		max -= min;
		t = t / totaltime - 1;
		return max * (t*t*t + 1) + min;
	}
	float in_out_cubic(float t, float totaltime, float max, float min)
	{
		max -= min;
		t /= totaltime;
		if (t / 2 < 1)
			return max / 2 * t*t*t + min;
		t -= 2;
		return max / 2 * (t*t*t + 2) + min;
	}
	float in_quart(float t, float totaltime, float max, float min)
	{
		max -= min;
		t /= totaltime;
		return max * t*t*t*t + min;
	}
	float out_quart(float t, float totaltime, float max, float min)
	{
		max -= min;
		t = t / totaltime - 1;
		return -max * (t*t*t*t - 1) + min;
	}
	float in_out_quart(float t, float totaltime, float max, float min)
	{
		max -= min;
		t /= totaltime;
		if (t / 2 < 1)
			return max / 2 * t*t*t*t + min;
		t -= 2;
		return -max / 2 * (t*t*t*t - 2) + min;
	}
	float in_quint(float t, float totaltime, float max, float min)
	{
		max -= min;
		t /= totaltime;
		return max * t*t*t*t*t + min;
	}
	float out_quint(float t, float totaltime, float max, float min)
	{
		max -= min;
		t = t / totaltime - 1;
		return max * (t*t*t*t*t + 1) + min;
	}
	float in_out_quint(float t, float totaltime, float max, float min)
	{
		max -= min;
		t /= totaltime;
		if (t / 2 < 1)
			return max / 2 * t*t*t*t*t + min;
		t -= 2;
		return max / 2 * (t*t*t*t*t + 2) + min;
	}
	float in_sine(float t, float totaltime, float max, float min)
	{
		max -= min;
		return -max * cosf(t*to_radian(90) / totaltime) + max + min;
	}
	float out_sine(float t, float totaltime, float max, float min)
	{
		max -= min;
		return max * sinf(t*to_radian(90) / totaltime) + min;
	}
	float in_out_sine(float t, float totaltime, float max, float min)
	{
		max -= min;
		return -max / 2 * (cosf(t*DirectX::XM_PI / totaltime) - 1) + min;
	}
	float in_exp(float t, float totaltime, float max, float min)
	{
		max -= min;
		return t == 0.0f ? min : max * powf(2, 10 * (t / totaltime - 1)) + min;
	}
	float out_exp(float t, float totaltime, float max, float min)
	{
		max -= min;
		return t == totaltime ? max + min : max * (-powf(2, -10 * t / totaltime) + 1) + min;
	}
	float in_out_exp(float t, float totaltime, float max, float min)
	{
		if (t == 0.0f)
			return min;
		if (t == totaltime)
			return max;
		max -= min;
		t /= totaltime;

		if (t / 2 < 1)
			return max / 2 * powf(2, 10 * (t - 1)) + min;
		--t;
		return max / 2 * (-powf(2, -10 * t) + 2) + min;

	}
	float in_circ(float t, float totaltime, float max, float min)
	{
		max -= min;
		t /= totaltime;
		return -max * (sqrtf(1 - t * t) - 1) + min;
	}
	float out_circ(float t, float totaltime, float max, float min)
	{
		max -= min;
		t = t / totaltime - 1;
		return max * sqrtf(1 - t * t) + min;
	}
	float in_out_circ(float t, float totaltime, float max, float min)
	{
		max -= min;
		t /= totaltime;
		if (t / 2 < 1)
			return -max / 2 * (sqrtf(1 - t * t) - 1) + min;
		t -= 2;
		return max / 2 * (sqrtf(1 - t * t) + 1) + min;
	}
	float in_back(float t, float totaltime, float max, float min, float s)
	{
		max -= min;
		t /= totaltime;
		return max * t*t*((s + 1)*t - s) + min;
	}
	float out_back(float t, float totaltime, float max, float min, float s)
	{
		max -= min;
		t = t / totaltime - 1;
		return max * (t*t*((s + 1)*t*s) + 1) + min;
	}
	float out_bounce(float t, float totaltime, float max, float min)
	{
		max -= min;
		t /= totaltime;

		if (t < 1 / 2.75f)
			return max * (7.5625f*t*t) + min;
		else if (t < 2 / 2.75f)
		{
			t -= 1.5f / 2.75f;
			return max * (7.5625f*t*t + 0.75f) + min;
		}
		else if (t< 2.5f / 2.75f)
		{
			t -= 2.25f / 2.75f;
			return max * (7.5625f*t*t + 0.9375f) + min;
		}
		else
		{
			t -= 2.625f / 2.75f;
			return max * (7.5625f*t*t + 0.984375f) + min;
		}
	}
	float in_bounce(float t, float totaltime, float max, float min)
	{
		return max - out_bounce(totaltime - t, totaltime, max - min, 0) + min;
	}
	float in_out_bounce(float t, float totaltime, float max, float min)
	{
		if (t < totaltime / 2)
			return in_bounce(t * 2, totaltime, max - min, max)*0.5f + min;
		else
			return out_bounce(t * 2 - totaltime, totaltime, max - min, 0)*0.5f + min + (max - min)*0.5f;
	}
	float linear(float t, float totaltime, float max, float min)
	{
		return (max - min)*t / totaltime + min;
	}
}
