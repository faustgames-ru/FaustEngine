#ifndef LOADER_JSON_ATLAS_H
#define LOADER_JSON_ATLAS_H

#include "../../zombo_classes.h"
#include "../../cjson/cJSON.h"

namespace zombo
{
	class JsonAtlasTriangle
	{
	public:
		int indices[3];
		JsonAtlasTriangle();
		JsonAtlasTriangle(int a, int b, int c);
	};

	class JsonAtlasPointF
	{
	public:
		float x;
		float y;
	};

	class JsonAtlasPointI
	{
	public:
		int x;
		int y;
		JsonAtlasPointI();
		JsonAtlasPointI(int valueX, int valueY);
	};

	class JsonAtlasSize
	{
	public:
		int w;
		int h;
	};

	class JsonAtlasRect
	{
	public:
		JsonAtlasPointI pos;
		JsonAtlasSize size;
	};

	class JsonAtlasFrame
	{
	public:
		char* filename;
		JsonAtlasRect frame;
		bool rotated;
		bool trimmed;
		JsonAtlasRect spriteSourceSize;
		JsonAtlasSize sourceSize;
		JsonAtlasPointF pivot;
		std::vector<JsonAtlasPointI> vertices;
		std::vector<JsonAtlasPointI> verticesUV;
		std::vector<JsonAtlasTriangle> triangles;
	};

	class JsonAtlasMeta
	{
	public:
		char * app;
		char * version;
		char * image;
		char * format;
		JsonAtlasSize size;
		char * scale;
	};

	class JsonAtlas
	{
	public:
		std::vector<JsonAtlasFrame *> frames;
		JsonAtlasMeta meta;
		JsonAtlas(const char *jsonString);
		~JsonAtlas();
	private:
		cJSON * _json;
	};
}

#endif /*LOADER_JSON_ATLAS_H*/