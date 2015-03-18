#ifndef CONTENT_MANAGER_H
#define CONTENT_MANAGER_H

#include "resources_classes.h"

namespace resources
{
	class ContentManager : public llge::IContentManager
	{
	public:
		ContentManager();
		void cleanup();
		unsigned int registerTexture(const char *name);
		graphics::Image2dData * loadTexture(int id);
		void open();
		void close();

		virtual int API_CALL registerImage(char * name);
		virtual void API_CALL startLoad();
		virtual void API_CALL loadImage(int id, llge::ITextureImage2d *textureImage);
		virtual void API_CALL finishLoad();
		virtual void API_CALL dispose();

	private:
		std::vector<std::string> _files;
		graphics::Image2dData *_image;
		bool _isOpened;
		static const int ImageBufferSize = 16 * 1024 * 1024; //(2048x2048x32bpp)
		static const int ImageMaxHeight = 2048;
		static const int ImageMaxWidth = 2048;
	};
}

#endif /*CONTENT_MANAGER_H*/

