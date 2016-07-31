#ifndef CONTENT_CLASSES_H
#define CONTENT_CLASSES_H

#include "llge.h"
#include "content_platform.h"
#include "content_references.h"

namespace content
{
	struct ContentValueType
	{
		enum e
		{
			None,
			Int,
			Bool,
			Float,
			String,
			Array,
			Object
		};
	};

	class ContentValue;
	class ContentObject;
	class ContentArray;
	class ContentSerializer;

	struct ImageVertex
	{
		core::Vector2 xy;
		ushort u;
		ushort v;
	};

	struct MeshVertex
	{
		core::Vector3 xyz;
		ushort u;
		ushort v;
	};


	class ContentConstants
	{
	public:
		static const ushort t0 = 1;
		static const ushort t05 = 32768;
		static const ushort t1 = 65535;
	};
	
	struct ContentType
	{
		enum e
		{
			None,
			Json,
			Mesh,
			Image,
			FrameAnimation,
			BitmapFont
		};
	};
	
	class ContentStatus;
	class ContentBlock;

	struct LoadArgs
	{
		std::string path;
		std::string fullPath;
		ContentStatus* status;
		ContentBlock* owner;
	};

	class IContent: public IBaseObject
	{
	public:
		virtual void dispose() = 0;
		virtual void load(const LoadArgs& e) = 0;
	};

	class ContentJson;
	class ContentImage;
	class ContentFrameAnimation;
	class ContentBitmapFont;
}

#endif /*CONTENT_CLASSES_H*/
