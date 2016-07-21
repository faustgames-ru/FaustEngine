#ifndef CONTENT_FRAME_ANIMATION_H
#define CONTENT_FRAME_ANIMATION_H

#include "content_classes.h"

namespace content
{
	class ContentFrameAnimation: public IContent
	{
	public:
		std::vector<ContentImage *> frames;
		static ContentFrameAnimation* create();
		ContentImage *frameFromPercent(float percent);
		virtual void dispose() OVERRIDE;
		virtual void load(const LoadArgs& e) OVERRIDE;
		float getFrameRate();
	private:
		float _frameRate;
	};
}

#endif /*CONTENT_FRAME_ANIMATION_H*/
