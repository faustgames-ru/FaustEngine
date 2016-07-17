#ifndef RAZOR_GAME_H
#define RAZOR_GAME_H

#include "razor.h"

#include "razor_classes.h"
#include "../geometry/Quadtree.h"

namespace razor
{
	class RazorGame : public IRazorGame
	{
	public:
		RazorGame();
		virtual void API_CALL setRootPath(String rootPath) OVERRIDE;
		virtual void API_CALL updateEnvironment(int w, int h, float ellapsedTime) OVERRIDE;
		virtual void API_CALL load() OVERRIDE;
		virtual void API_CALL update() OVERRIDE;
		virtual void API_CALL render() OVERRIDE;
		virtual void API_CALL release() OVERRIDE;
	private:

		void spaceShipSpriteLoaded(content::ContentStatus* status);
		void spaceShipAnimationLoaded(content::ContentStatus* status);

		content::ContentImage* _spaceShipSprite;
		content::ContentFrameAnimation* _spaceShipAnimation;
		content::ContentBlock* _content;

		graphics::Camera2d _camera;
		graphics::Viewport _viewport;
		geometry::QuadTree _tree;
	};
}

#endif /*RAZOR_GAME_H*/
