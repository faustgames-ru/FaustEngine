#ifndef RENDER_SYSTEM_2D_H
#define RENDER_SYSTEM_2D_H

#include "entities_classes.h"
#include "System.h"
#include "Aabb2dComponent.h"

namespace entities
{
	class RenderSystem2d : public System
	{
	public:
		drawing::Camera* getCamera();
		virtual void updateSystem(float elapsed, const std::vector<Entity *> entities);
		int getDrawCalls();
	private:

		struct EntityZOrderCompare
		{
			bool operator()(Entity * l, Entity *r) const
			{
				Aadd2dComponent *lAabb = l->getComponent<Aadd2dComponent>();
				Aadd2dComponent *rAabb = r->getComponent<Aadd2dComponent>();
				float lZ = lAabb ? lAabb->ZOrder : core::Math::MaxValue;
				float rZ = rAabb ? rAabb->ZOrder : core::Math::MaxValue;
				return lZ < rZ;
			}
		};

		typedef std::multiset<Entity *, EntityZOrderCompare, core::DAlloc> DrawOrder;
		DrawOrder _drawOrder;
		drawing::Renderer2d _renderer;
		int _drawCalls;
	};
}

#endif /*RENDER_SYSTEM_2D_H*/