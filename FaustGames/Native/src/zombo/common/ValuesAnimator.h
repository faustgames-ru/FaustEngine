#ifndef ZOMBO_VALUES_ANIMATOR_H
#define ZOMBO_VALUES_ANIMATOR_H

#include "../zombo_classes.h"
#include "SConfig.h"
#include "ZomboGameEnvironment.h"
#include "SEntry.h"

namespace zombo
{		
	typedef SEntry<float> SEntryFloat;
	typedef SEntry<float> SEntryVector2;
	typedef SEntry<float> SEntryVector3;

	class ValuesAnimator
	{
	public:
		static ValuesAnimator Default;
		ValuesAnimator();
		void animate(float *value, float target);
		void animate(core::Vector2 *value, const core::Vector2 &target);
		void animate(core::Vector3 *value, const core::Vector3 &target);
		bool animating(float *value);
		bool animating(core::Vector2 *value);
		bool animating(core::Vector3 *value);
		void terminate(float *value);
		void terminate(core::Vector2 *value);
		void terminate(core::Vector3 *value);
		void update();
	private:
		typedef std::map<float *, SEntryFloat *> MapFloat;
		typedef std::map<float *, SEntryVector2 *> MapVector2;
		typedef std::map<float *, SEntryVector3 *> MapVector3;
		MapFloat _valuesFloat;
		MapVector2 _valuesVector2;
		MapVector3 _valuesVector3;
	};
}

#endif /* ZOMBO_VALUES_ANIMATOR_H */