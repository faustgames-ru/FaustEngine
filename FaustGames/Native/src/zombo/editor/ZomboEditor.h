#ifndef ZOMBO_EDITOR_H
#define ZOMBO_EDITOR_H

#include "zombo.h"
#include "../../geometry/Quadtree.h"
#include "../../game/Scene.h"

namespace zombo
{
	class ZomboEditor: public IZomboEditor
	{
	public:
		ZomboEditor();
		virtual void API_CALL setRootPath(String rootPath) OVERRIDE;
		virtual void API_CALL updateMouse(int rawX, int rawY, int windowX, int windowY, uint buttons) OVERRIDE;
		virtual void API_CALL updateEnvironment(int w, int h, float ellapsedTime) OVERRIDE;
		virtual void API_CALL load() OVERRIDE;
		virtual void API_CALL update() OVERRIDE;
		virtual void API_CALL render() OVERRIDE;
		virtual void API_CALL release() OVERRIDE;
	private:
		void gameLoaded(content::ContentStatus* status);
		void sceneFileLoaded(content::ContentStatus* status);
		content::ContentBlock* _content;
		game::Scene* _loadedScene;
		game::Scene* _updateScene;
		graphics::Viewport _viewport;
	};
}

#endif /*ZOMBO_EDITOR_H*/
