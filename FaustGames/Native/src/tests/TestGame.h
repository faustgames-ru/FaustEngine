#ifndef TEST_GAME_H
#define TEST_GAME_H
#include "../core/BasicTest.h"
#include "../spine/SpineSkeletonResource.h"
#include "../spine/SpineSkeleton.h"
#include "../spine/SpineAnimationStateData.h"
#include "../spine/SpineAnimationState.h"
#include "../spine/SpineSkeletonAnimation.h"

namespace tests
{
	class TestGame: public llge::ITestGame
	{
	public:
		TestGame();
		virtual void API_CALL setRootPath(String rootPath) OVERRIDE;
		virtual void API_CALL updateColors(IntPtr colors);
		virtual void API_CALL updateMouse(int rawX, int rawY, int windowX, int windowY, uint buttons) OVERRIDE;
		virtual void API_CALL updateEnvironment(int w, int h, float ellapsedTime) OVERRIDE;
		virtual void API_CALL load() OVERRIDE;
		virtual void API_CALL update() OVERRIDE;
		virtual void API_CALL render() OVERRIDE;
		virtual void API_CALL release() OVERRIDE;
	private:
		std::string _root;
		spine::SpineSkeletonResource* _skeletonResource;
		spine::SpineSkeleton* _skeleton;
		spine::SpineSkeletonAnimation* _walk;
		spine::SpineAnimationStateData* _animationStateData;
		spine::SpineAnimationState* _animationState;
		graphics::Texture* _paintmask;
		drawing::Batcher* _batcher;
		float _w;
		float _h;
		float _t;
	};
}


#endif /*TEST_GAME_H*/

