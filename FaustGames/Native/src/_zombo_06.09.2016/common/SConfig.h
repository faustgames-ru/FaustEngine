#ifndef ZOMBO_SCONFIG_H
#define ZOMBO_SCONFIG_H

#include "../zombo_classes.h"

namespace zombo
{
	class IZomboEase : IBaseObject
	{
	public:
		virtual float ease(float t) = 0;
	};

	class ZomboEaseLinear : public IZomboEase
	{
	public:
		virtual float ease(float t) OVERRIDE;
	};

	class ZomboEaseSmoothQuadric : public IZomboEase
	{
	public:
		virtual float ease(float t) OVERRIDE;
	};

	class ZomboEase
	{
	public:
		static ZomboEaseLinear Linear;
		static ZomboEaseSmoothQuadric SmoothQuadric;
	};

	class SConfig
	{
	public:
		static SConfig VerySlow;
		static SConfig Slow;
		static SConfig Default;
		static SConfig Fast;
		SConfig();
		SConfig(float duration, float minVelocity);
		float calcDuration(float len) const;
		float calcU(float len, ulong startTime) const;
		float intepolate(float from, float to, ulong startTime) const;
		core::Vector2 intepolate(const core::Vector2 &from, const core::Vector2 &to, ulong startTime) const;
		core::Vector3 intepolate(const core::Vector3 &from, const core::Vector3 &to, ulong startTime) const;
		static bool equal(float a, float b);
		static bool equal(const core::Vector2 &a, const core::Vector2 &b);
		static bool equal(const core::Vector3 &a, const core::Vector3 &b);
	private:
		IZomboEase* _ease;
		float _duration;
		float _minVelocity;
	};

}

#endif ZOMBO_SCONFIG_H
