#include "RenderSystem2d.h"
#include "Render2dComponent.h"
#include "Transform2dComponent.h"
#include "MatrixTransformComponent.h"

namespace entities
{
	drawing::Camera* RenderSystem2d::getCamera()
	{
		return &(_renderer.RendererCamera);
	}

	void RenderSystem2d::updateSystem(float elapsed, const std::vector<Entity *> entities)
	{
		int count = entities.size();
		_drawOrder.clear();
		for (int i = 0; i < count; i++)
		{
			_drawOrder.insert(entities[i]);
		}

		_renderer.start();
		
		for (DrawOrder::iterator i = _drawOrder.begin(); i != _drawOrder.end(); i++)
		{
			Render2dComponent* renderComponent = (*i)->getComponent<Render2dComponent>();
			//Transform2dComponent* transformComponent = (*i)->getComponent<Transform2dComponent>();
			MatrixTransformComponent* transformComponent = (*i)->getComponent<MatrixTransformComponent>();
			if (!renderComponent) continue;

			//todo: optimize to catch transformed vertices in render item
			//_renderer.Transform.LocalPivot = transformComponent->LocalTransform.Pivot;
			//_renderer.Transform.GlobalPosition = transformComponent->WorldTransform.Position;
			//_renderer.Transform.Sin = transformComponent->WorldTransform.Sin;
			//_renderer.Transform.Cos = transformComponent->WorldTransform.Cos;
			//_renderer.Transform.Scale = transformComponent->WorldTransform.Scale;
			
			_renderer.Transform.Matrix = &(transformComponent->Matrix);

			for (int j = 0; j < renderComponent->MeshesCount; j++)
				renderComponent->RenderItems[j]->render(&_renderer);
		}
		_drawCalls = _renderer.finish();
	}

	int RenderSystem2d::getDrawCalls()
	{
		return _drawCalls;
	}

}
