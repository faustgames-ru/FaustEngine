#include "LoaderJsonAtlas.h"
#include "../serialization/ZomboValue.h"
#include "../serialization/ZomboObject.h"
#include "../serialization/ZomboArray.h"
#include "../serialization/ZomboSerializer.h"

namespace zombo
{
	JsonAtlasTriangle::JsonAtlasTriangle()
	{
	}

	JsonAtlasTriangle::JsonAtlasTriangle(int a, int b, int c)
	{
		indices[0] = a;
		indices[1] = b;
		indices[2] = c;
	}

	JsonAtlasPointI::JsonAtlasPointI(): x(0), y(0)
	{
	}

	JsonAtlasPointI::JsonAtlasPointI(int valueX, int valueY)
	{
		x = valueX;
		y = valueY;
	}

	JsonAtlas::~JsonAtlas()
	{
		for (uint i = 0; i < frames.size(); i++)
		{
			delete frames[i];
		}
	}

	class ZomboContentAtlasPage* JsonAtlas::createContentAtlasPage()
	{
		ZomboContentAtlasPage* page = ZomboContentAtlasPage::create();
		page->texture.fileName = meta.image;
		for (uint i = 0; i < frames.size(); i++)
		{
			JsonAtlasFrame * frame = frames[i];
			ZomboContentImage * image = ZomboContentImage::create();


			image->name = frame->filename;
			if (frame->vertices.size() != frame->verticesUV.size())
			{
				// handle error
			}
			else
			{
				core::Vector2 center(static_cast<float>(frame->sourceSize.w) * frame->pivot.x, static_cast<float>(frame->sourceSize.h) * frame->pivot.y);
				for (uint j = 0; j < frame->vertices.size(); j++)
				{
					ZomboImageVertex v;
					// todo: handle rotation
					v.xy.setX((frame->vertices[j].x - center.getX()) * scale);
					v.xy.setY((frame->vertices[j].y - center.getY()) * scale);
					v.u = frame->verticesUV[j].x * USHRT_MAX / meta.size.w;
					v.v = frame->verticesUV[j].y * USHRT_MAX / meta.size.h;
					image->vertices.push_back(v);
				}
				for (uint j = 0; j < frame->triangles.size(); j++)
				{
					image->indices.push_back(frame->triangles[j].indices[0]);
					image->indices.push_back(frame->triangles[j].indices[1]);
					image->indices.push_back(frame->triangles[j].indices[2]);
				}
			}

			page->images.push_back(image);			
		}
		return page;
	}

	JsonAtlas::JsonAtlas(const char* jsonString, float verticesScale)
	{
		scale = verticesScale;
		ZomboValue *zomboRootValue = ZomboSerializer::deserializeFromJson(jsonString);
		ZomboObject *zomboRoot = zomboRootValue->asObject();
		ZomboObject *zomboMeta = (*zomboRoot)["meta"]->asObject();
		ZomboObject *zomboMetaSize = (*zomboMeta)["size"]->asObject();
		ZomboArray * zomboFrames = (*zomboRoot)["frames"]->asArray();


		meta.app = (*zomboMeta)["app"]->asString();
		meta.version = (*zomboMeta)["version"]->asString();
		meta.image = (*zomboMeta)["image"]->asString();
		meta.format = (*zomboMeta)["format"]->asString();
		meta.scale = (*zomboMeta)["scale"]->asString();

		meta.size.w = (*zomboMetaSize)["w"]->asInt();
		meta.size.h = (*zomboMetaSize)["h"]->asInt();
		
		frames.resize(zomboFrames->size());
		for (int i = 0; i < frames.size(); i++)
		{
			ZomboObject* zomboFrame = (*zomboFrames)[i]->asObject();
			ZomboObject* zomboFrameFrame = (*zomboFrame)["frame"]->asObject();
			ZomboObject* zomboFrameSpriteSourceSize = (*zomboFrame)["spriteSourceSize"]->asObject();
			ZomboObject* zomboFrameSourceSize = (*zomboFrame)["sourceSize"]->asObject();
			ZomboObject* zomboFramePivot = (*zomboFrame)["pivot"]->asObject();
			ZomboArray* zomboFrameVertices = (*zomboFrame)["vertices"]->asArray();
			ZomboArray* zomboFrameVerticesUV = (*zomboFrame)["verticesUV"]->asArray();
			ZomboArray* zomboFrameTriangles = (*zomboFrame)["triangles"]->asArray();

			JsonAtlasFrame *frame = new JsonAtlasFrame();
			frames[i] = frame;

			frame->filename = (*zomboFrame)["filename"]->asString();
			frame->frame.pos.x = (*zomboFrameFrame)["x"]->asInt();
			frame->frame.pos.y = (*zomboFrameFrame)["y"]->asInt();
			frame->frame.size.w = (*zomboFrameFrame)["w"]->asInt();
			frame->frame.size.h = (*zomboFrameFrame)["h"]->asInt();

			frame->rotated = (*zomboFrame)["rotated"]->asBool();
			frame->trimmed = (*zomboFrame)["trimmed"]->asBool();
			frame->spriteSourceSize.pos.x = (*zomboFrameSpriteSourceSize)["x"]->asInt();
			frame->spriteSourceSize.pos.y = (*zomboFrameSpriteSourceSize)["y"]->asInt();
			frame->spriteSourceSize.size.w = (*zomboFrameSpriteSourceSize)["w"]->asInt();
			frame->spriteSourceSize.size.h = (*zomboFrameSpriteSourceSize)["h"]->asInt();
			frame->sourceSize.w = (*zomboFrameSourceSize)["w"]->asInt();
			frame->sourceSize.h = (*zomboFrameSourceSize)["h"]->asInt();
			frame->pivot.x = static_cast<float>((*zomboFramePivot)["x"]->asFloat());
			frame->pivot.y = static_cast<float>((*zomboFramePivot)["y"]->asFloat());

			frame->vertices.resize(zomboFrameVertices->size());
			for (uint j = 0; j < frame->vertices.size(); j++)
			{
				ZomboArray* v = (*zomboFrameVertices)[j]->asArray();
				frame->vertices[j].x = (*v)[0]->asInt();
				frame->vertices[j].y = (*v)[1]->asInt();
			}

			frame->verticesUV.resize(zomboFrameVerticesUV->size());
			for (uint j = 0; j < frame->verticesUV.size(); j++)
			{
				ZomboArray* v = (*zomboFrameVerticesUV)[j]->asArray();
				frame->verticesUV[j].x = (*v)[0]->asInt();
				frame->verticesUV[j].y = (*v)[1]->asInt();
			}

			frame->triangles.resize(zomboFrameTriangles->size());
			for (uint j = 0; j < frame->triangles.size(); j++)
			{
				ZomboArray* v = (*zomboFrameTriangles)[j]->asArray();
				frame->triangles[j].indices[0] = (*v)[0]->asInt();
				frame->triangles[j].indices[1] = (*v)[1]->asInt();
				frame->triangles[j].indices[2] = (*v)[2]->asInt();
			}
		}
		zomboRootValue->dispose();
	}	
}
