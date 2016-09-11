#include "Mesh.h"
#include "../Entity.h"

namespace game
{
	Mesh::Mesh(): transform(core::Matrix3::identity), _contentMesh(nullptr)
	{
		renderOrder = RenderOrder::Default;
	}

	void Mesh::enqueueResources(const LoadArgs& e)
	{
		e.content->enqueueResource(e.value->asString())->setLoadedCallback(this, &Mesh::resourcesLoaded);
	}

	void Mesh::render(const RenderArgs& e)
	{
		MeshTransform t;
		t.position = owner->position;
		t.rotation = transform;
		Drawing::Default.drawMesh(t, _contentMesh);
	}

	void Mesh::resourcesLoaded(content::ContentStatus* status)
	{
		_contentMesh = status->asMesh();
	}
}
