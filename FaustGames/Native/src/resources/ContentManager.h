#ifndef CONTENT_MANAGER_H
#define CONTENT_MANAGER_H

#include "resources_classes.h"

namespace resources
{
	class ContentManager : public llge::IContentManager, public llge::ITextureBuffer2d
	{
	public:
		ContentManager();
		void cleanup();
		unsigned int registerTexture(const char *name);
		graphics::Image2dData * loadUnregisteredTexture(const char *name);
		graphics::Image2dData * loadTexture(int id);
		void open();
		void close();

		virtual void API_CALL setObbFile(char * obbFile);
		virtual int API_CALL registerImage(char * name);
		virtual void API_CALL startLoad();
		virtual void API_CALL loadImage(int id, llge::ITextureImage2d *textureImage);
		virtual llge::ITextureBuffer2d *API_CALL loadBuffer(int id);
		virtual void API_CALL finishLoad();
		virtual void API_CALL dispose();

		virtual llge::TextureImage2dFormat API_CALL getFormat();
		virtual int API_CALL getWidth();
		virtual int API_CALL getHeight();
		virtual IntPtr API_CALL getPixels();
		static ContentManager * Default;
	private:
		std::vector<std::string> _files;
		graphics::Image2dData *_image;
		bool _isOpened;
#ifdef __ANDROID__
		static const int ImageBufferSize = 2048 * 2048; //(2048x2048x32bpp)
		static const int ImageMaxHeight = 2048;
		static const int ImageMaxWidth = 2048;
#else
		static const int ImageBufferSize = 4*2048 * 2048; //(4096x4096x32bpp)
		static const int ImageMaxHeight = 2*2048;
		static const int ImageMaxWidth = 2*2048;
#endif
	};
}

#endif /*CONTENT_MANAGER_H*/

