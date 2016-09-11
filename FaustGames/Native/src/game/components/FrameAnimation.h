#ifndef GAME_COMPONENTS_FRAME_ANIMATION_H
#define GAME_COMPONENTS_FRAME_ANIMATION_H

#include "../game_classes.h"
#include "../../content/ContentFrameAnimation.h"
#include "../Component.h"
#include "../services/Drawing.h"

namespace game
{
	class FrameAnimation : public Component
	{
	public:
		Rotation rotation;
		FrameAnimation();
		virtual void enqueueResources(const LoadArgs& e) OVERRIDE;
		virtual void update(const UpdateArgs& e) OVERRIDE;
		virtual void render(const RenderArgs& e) OVERRIDE;

		void setPercent(float percent);
		void setFrame(float frameIndex);
		void setTime(float time);
		void play();
		void pause();
	private:
		void display() const;
		void resourcesLoaded(content::ContentStatus* status);
		content::ContentFrameAnimation* _animation;
		content::ContentImage* _frame;

		float _frameIndex;
		float _frameRate;
		float _scale;
		int _loops;
	};
}


#endif /*GAME_COMPONENTS_FRAME_ANIMATION_H*/