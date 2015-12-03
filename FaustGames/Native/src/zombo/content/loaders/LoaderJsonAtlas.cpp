#include "LoaderJsonAtlas.h"

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
		if (_json != nullptr)
		{
			cJSON_Delete(_json);
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
		_json = nullptr;

		cJSON * json = cJSON_Parse(jsonString);
		cJSON * jsonMeta = cJSON_GetObjectItem(json, "meta");
		cJSON * jsonMetaApp = cJSON_GetObjectItem(jsonMeta, "app");
		cJSON * jsonMetaVersion = cJSON_GetObjectItem(jsonMeta, "version");
		cJSON * jsonMetaImage = cJSON_GetObjectItem(jsonMeta, "image");
		cJSON * jsonMetaFormat = cJSON_GetObjectItem(jsonMeta, "format");
		cJSON * jsonMetaScale = cJSON_GetObjectItem(jsonMeta, "scale");
		cJSON * jsonMetaSize = cJSON_GetObjectItem(jsonMeta, "size");
		cJSON * jsonMetaSizeW = cJSON_GetObjectItem(jsonMetaSize, "w");
		cJSON * jsonMetaSizeH = cJSON_GetObjectItem(jsonMetaSize, "h");
		cJSON * jsonFrames = cJSON_GetObjectItem(json, "frames");

		meta.app = jsonMetaApp->valuestring;
		meta.version = jsonMetaVersion->valuestring;
		meta.image = jsonMetaImage->valuestring;
		meta.format = jsonMetaFormat->valuestring;
		meta.scale = jsonMetaScale->valuestring;

		meta.size.w = jsonMetaSizeH->valueint;
		meta.size.h = jsonMetaSizeW->valueint;

		cJSON *jsonFrame = jsonFrames->child;
		while (jsonFrame != nullptr)
		{
			JsonAtlasFrame *frame = new JsonAtlasFrame();
			cJSON * jsonFrameFilename = cJSON_GetObjectItem(jsonFrame, "filename");
			cJSON * jsonFrameFrame = cJSON_GetObjectItem(jsonFrame, "frame");
			cJSON * jsonFrameFrameX = cJSON_GetObjectItem(jsonFrameFrame, "x");
			cJSON * jsonFrameFrameY = cJSON_GetObjectItem(jsonFrameFrame, "y");
			cJSON * jsonFrameFrameW = cJSON_GetObjectItem(jsonFrameFrame, "w");
			cJSON * jsonFrameFrameH = cJSON_GetObjectItem(jsonFrameFrame, "h");
			cJSON * jsonFrameRotated = cJSON_GetObjectItem(jsonFrame, "rotated");
			cJSON * jsonFrameTrimmed = cJSON_GetObjectItem(jsonFrame, "trimmed");
			cJSON * jsonFrameSpriteSourceSize = cJSON_GetObjectItem(jsonFrame, "spriteSourceSize");
			cJSON * jsonFrameSpriteSourceSizeX = cJSON_GetObjectItem(jsonFrameSpriteSourceSize, "x");
			cJSON * jsonFrameSpriteSourceSizeY = cJSON_GetObjectItem(jsonFrameSpriteSourceSize, "y");
			cJSON * jsonFrameSpriteSourceSizeW = cJSON_GetObjectItem(jsonFrameSpriteSourceSize, "w");
			cJSON * jsonFrameSpriteSourceSizeH = cJSON_GetObjectItem(jsonFrameSpriteSourceSize, "h");
			cJSON * jsonFrameSourceSize = cJSON_GetObjectItem(jsonFrame, "sourceSize");
			cJSON * jsonFrameSourceSizeW = cJSON_GetObjectItem(jsonFrameSourceSize, "w");
			cJSON * jsonFrameSourceSizeH = cJSON_GetObjectItem(jsonFrameSourceSize, "h");
			cJSON * jsonFramePivot = cJSON_GetObjectItem(jsonFrame, "pivot");
			cJSON * jsonFramePivotX = cJSON_GetObjectItem(jsonFramePivot, "x");
			cJSON * jsonFramePivotY = cJSON_GetObjectItem(jsonFramePivot, "y");
			cJSON * jsonFrameVertices = cJSON_GetObjectItem(jsonFrame, "vertices");
			cJSON * jsonFrameVerticesUV = cJSON_GetObjectItem(jsonFrame, "verticesUV");
			cJSON * jsonFrameTriangles = cJSON_GetObjectItem(jsonFrame, "triangles");

			frame->filename = jsonFrameFilename->valuestring;
			frame->frame.pos.x = jsonFrameFrameX->valueint;
			frame->frame.pos.y = jsonFrameFrameY->valueint;
			frame->frame.size.w = jsonFrameFrameW->valueint;
			frame->frame.size.h = jsonFrameFrameH->valueint;

			frame->rotated = jsonFrameRotated->valueint != 0;
			frame->trimmed = jsonFrameTrimmed->valueint != 0;
			frame->spriteSourceSize.pos.x = jsonFrameSpriteSourceSizeX->valueint;
			frame->spriteSourceSize.pos.y = jsonFrameSpriteSourceSizeY->valueint;
			frame->spriteSourceSize.size.w = jsonFrameSpriteSourceSizeW->valueint;
			frame->spriteSourceSize.size.h = jsonFrameSpriteSourceSizeH->valueint;
			frame->sourceSize.w = jsonFrameSourceSizeW->valueint;
			frame->sourceSize.h = jsonFrameSourceSizeH->valueint;
			frame->pivot.x = static_cast<float>(jsonFramePivotX->valuedouble);
			frame->pivot.y = static_cast<float>(jsonFramePivotY->valuedouble);

			cJSON *jsonFrameVertex = jsonFrameVertices->child;
			while (jsonFrameVertex != nullptr)
			{
				cJSON *jsonFrameVertexX = jsonFrameVertex->child;
				cJSON *jsonFrameVertexY = jsonFrameVertexX->next;
				frame->vertices.push_back(JsonAtlasPointI(jsonFrameVertexX->valueint, jsonFrameVertexY->valueint));
				jsonFrameVertex = jsonFrameVertex->next;
			}

			cJSON *jsonFrameVertexUV = jsonFrameVerticesUV->child;
			while (jsonFrameVertexUV != nullptr)
			{
				cJSON *jsonFrameVertexU = jsonFrameVertexUV->child;
				cJSON *jsonFrameVertexV = jsonFrameVertexU->next;
				frame->verticesUV.push_back(JsonAtlasPointI(jsonFrameVertexU->valueint, jsonFrameVertexV->valueint));
				jsonFrameVertexUV = jsonFrameVertexUV->next;
			}

			cJSON *jsonFrameTriangle = jsonFrameTriangles->child;
			while (jsonFrameTriangle != nullptr)
			{
				cJSON *jsonFrameVertexA = jsonFrameTriangle->child;
				cJSON *jsonFrameVertexB = jsonFrameVertexA->next;
				cJSON *jsonFrameVertexC = jsonFrameVertexB->next;
				frame->triangles.push_back(JsonAtlasTriangle(
					jsonFrameVertexA->valueint, 
					jsonFrameVertexB->valueint, 
					jsonFrameVertexC->valueint));
				jsonFrameTriangle = jsonFrameTriangle->next;
			}

			frames.push_back(frame);
			jsonFrame = jsonFrame->next;
		}
		_json = json;
	}
}
