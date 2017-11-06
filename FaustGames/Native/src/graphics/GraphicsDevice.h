#ifndef GRAPHICSDEVICE_H
#define GRAPHICSDEVICE_H

#include "graphics_classes.h"
#include "RenderState.h"
#include "TextureRenderTarget2d.h"
#include "TextureRenderTargetDepth2d.h"
#include "VertexBuffer.h"

namespace graphics
{

	class IPostProcessTarget : public IBaseObject
	{
	public:
		virtual IRenderTarget* getRenderTarget() = 0;
		virtual Texture* getColor() = 0;
		virtual Texture* getDepth() = 0;
		virtual int getWidth() = 0;
		virtual int getHeight() = 0;
		virtual void API_CALL create(int width, int height) = 0;
		virtual void API_CALL cleanup() = 0;
	};
	
	class PostProcessTarget : public IPostProcessTarget
	{
	public:
		PostProcessTarget(TextureRenderTarget2d* target);
		virtual IRenderTarget* getRenderTarget() OVERRIDE;
		virtual Texture* getColor() OVERRIDE;
		virtual Texture* getDepth() OVERRIDE;
		virtual int getWidth() OVERRIDE;
		virtual int getHeight() OVERRIDE;
		virtual void API_CALL create(int width, int height) OVERRIDE;
		virtual void API_CALL cleanup() OVERRIDE;
	private:
		TextureRenderTarget2d* _target;
	};
		
	class PostProcessDepthTarget : public IPostProcessTarget
	{
	public:
		PostProcessDepthTarget(TextureRenderTargetDepth2d* target);
		virtual IRenderTarget* getRenderTarget() OVERRIDE;
		virtual Texture* getColor() OVERRIDE;
		virtual Texture* getDepth() OVERRIDE;
		virtual int getWidth() OVERRIDE;
		virtual int getHeight() OVERRIDE;
		virtual void API_CALL create(int width, int height) OVERRIDE;
		virtual void API_CALL cleanup() OVERRIDE;
	private:
		TextureRenderTargetDepth2d* _target;
	};

	class IPostProcessTargetConstructor: public IBaseObject
	{
	public:
		virtual IPostProcessTarget* createPostProcessTarget(bool filter) = 0;
	};

	class PostProcessTargetConstructor : public IPostProcessTargetConstructor
	{
	public:
		static PostProcessTargetConstructor Default;
		virtual IPostProcessTarget* createPostProcessTarget(bool filter) OVERRIDE;
	};

	class PostProcessTargetDepthConstructor : public IPostProcessTargetConstructor
	{
	public:
		static PostProcessTargetDepthConstructor Default;
		virtual IPostProcessTarget* createPostProcessTarget(bool filter) OVERRIDE;
	};

	class PostProcessTargetManager
	{
	public:
		bool filter;
		PostProcessTargetManager();
		void setConstructor(IPostProcessTargetConstructor* constructor);
		IPostProcessTarget *pop();
		void push(IPostProcessTarget *);
		void setViewport(int width, int height);
		bool isAvaliable();
	private:
		void addProcessRenderTarget();
		std::vector<IPostProcessTarget *> _stack;
		std::vector<IPostProcessTarget *> _all;
		int _width;
		int _height;
		IPostProcessTargetConstructor* _constructor;
	};

	struct Extensions
	{
		enum e
		{
			OES_depth_texture = 0x0,
			Count = 0x1,
		};
		static std::string names[Count];
	};
	
	class GraphicsConfig
	{
	public:
		FilterMode::e filterMode;
		bool generateMipmaps;
		bool earlyDepthPath;
		bool enableFog;
		bool earlyFragmentTestsShaderCode;
		int bloomDownsample;
		int mipmapsLevel;
		bool refraction;
		float refractionScale;
		float refractionVelocityX;
		float refractionVelocityY;
		bool vignetting;
		float vignettingR0;
		float vignettingR1;
		float vignettingR2;
		uint vignettingColor0;
		uint vignettingColor1;
		uint vignettingColor2;
		float ellapsedTime;
		float postEffectsScale;
		bool useRgbTransforms;
		bool useColorCorrection;
		float colorCorrectionRotation;
		float colorCorrectionScaleX;
		float colorCorrectionScaleY;
		float colorCorrectionOffsetY;
		int getMagFilter();
		int getMinFilter();
		GraphicsConfig();
	};
	
	class GraphicsDevice
	{
	public:
		static bool extensions[Extensions::Count];
		GraphicsConfig config;
		static GraphicsDevice Default;
		RenderState renderState;
		IRenderTarget * actualRenderTarget;
		GraphicsDevice();
		~GraphicsDevice();
		void setClearState(unsigned int color, float depth);
		void setViewport(int x, int y, int width, int height);
		void setRenderTarget(IRenderTarget *renderTarget);
		void clear();
		void clearDepth();
		void clearStencil();
		void startStencilMask(int mask);
		void startStencilTest(int mask);
		void finishStencil();

		void resetState();
		void drawVertexBuffer(VertexFormat *vertexFormat, GLuint vertexBuffer, unsigned short *indexBuffer, int primitivesCount);
		void drawEdgesPrimitives(VertexFormat *vertexFormat, void *vertexBuffer, unsigned short *indexBuffer, int primitivesCount);
		void drawEdges(VertexFormat *vertexFormat, void *vertexBuffer, int primitivesCount);
		void drawTriangles(VertexFormat *vertexFormat, void *vertexBuffer, int primitivesCount);
		void drawPrimitives(VertexFormat *vertexFormat, void *vertexBuffer, unsigned short *indexBuffer, int primitivesCount);
		void resetSamplersState();
		void resetRenderState();
        void grabDefaultRenderTarget();
        void setActiveTexture(uint value);
		int getPixelsWidth();
		int getPixelsHeight();
		void readPixels(void * pixels, bool inverse);
		static void create();
		PostProcessTargetManager PostProcessTargets;
		PostProcessTargetManager PostProcessScaledTargets1;
		PostProcessTargetManager PostProcessScaledTargets2;
		PostProcessTargetManager PostProcessScaledTargets3;

		void resetDrawCallsCounter();
		int getDrawCallsCounterValue();
	protected:
	private:
		int _viewportX;
		int _viewportY;
		int _viewportWidth;
		int _viewportHeight;
        static int _primaryFbo;
        float _depthState;
        uint _colorState;
        int _activeTextureState;
		int _drawCalls;
	};
}

#endif /*GRAPHICSDEVICE_H*/