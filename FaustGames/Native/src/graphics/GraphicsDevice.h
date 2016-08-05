#ifndef GRAPHICSDEVICE_H
#define GRAPHICSDEVICE_H

#include "graphics_classes.h"
#include "RenderState.h"
#include "TextureRenderTarget2d.h"
#include "VertexBuffer.h"

namespace graphics
{
	class PostProcessTargetManager
	{
	public:
		bool filter;
		PostProcessTargetManager();
		TextureRenderTarget2d *pop();
		void push(TextureRenderTarget2d *);
		void setViewport(int width, int height);
		bool isAvaliable();
	private:
		void addProcessRenderTarget();
		std::vector<TextureRenderTarget2d *> _stack;
		std::vector<TextureRenderTarget2d *> _all;
		int _width;
		int _height;
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
		int getMagFilter();
		int getMinFilter();
		GraphicsConfig();
	};
	
	class GraphicsDevice
	{
	public:
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
		void readPixels(void * pixels);
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