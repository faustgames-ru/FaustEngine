#include "ContentMesh.h"
#include "ContentStatus.h"
#include "ContentBlock.h"
#include "BinaryReader.h"
#include "../core/Path.h"

namespace content
{
	ContentMesh* ContentMesh::create()
	{
		return new ContentMesh();
	}

	void ContentMesh::internalLoad(const LoadArgs& e)
	{
		resources::ContentManager* content = &resources::ContentManager::Default;
		if (!resources::ContentProvider::existContent(e.fullPath.c_str()))
		{
			// todo: handle error
			//ZomboLog::Default.e("Unable to find resource: " + fileName);
		}
		else
		{
			BinaryReader reader(content->loadString(e.fullPath.c_str()));
			std::string diffuseFile = reader.readString();
			std::string specularNormal = reader.readString();

			int indicesCount = reader.readInt();
			indices.resize(indicesCount);
			for (int i = 0; i < indicesCount; i++)
			{
				indices[i] = reader.readUshort();
			}

			int verticesCount = reader.readInt();
			vertices.resize(verticesCount);
			for (int i = 0; i < verticesCount; i++)
			{
				vertices[i].xyz = reader.readVector();
			}
			for (int i = 0; i < verticesCount; i++)
			{
				vertices[i].u = reader.readUshort();
				vertices[i].v = reader.readUshort();
			}

			std::string thisDir = core::Path::getFilePath(e.path);
			std::string thisFullDir = core::Path::getFilePath(e.fullPath);
			std::string diffusePath = thisFullDir + diffuseFile;
			graphics::Image2dData* imageData = content->loadUnregisteredTexture(diffusePath.c_str());
			if (imageData != nullptr)
			{
				// todo: configure loading;
				diffuse = new graphics::TextureImage2d(true, true);
				diffuse->create();
				diffuse->setData(imageData);
			}
			else
			{
				// todo: handle error
				//ZomboLog::Default.e("Unable to load png: " + fileName);
			}
		}
	}

	void ContentMesh::dispose()
	{
		delete this;
	}

	void ContentMesh::load(const LoadArgs& e)
	{
		internalLoad(e);
		e.status->invokeCallback();
	}

	ContentMesh::ContentMesh(): diffuse(nullptr)
	{
	}

	ContentMesh::~ContentMesh()
	{
	}
}
