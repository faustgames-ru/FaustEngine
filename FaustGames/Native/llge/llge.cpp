// llge.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"

using namespace graphics;

namespace llge
{
	class RenderSystem : public IRenderSystem
	{
	public:
		GraphicsDevice * _graphicsDevice;
		RenderSystem();
		virtual ~RenderSystem();
		virtual void API_CALL render();
		virtual void API_CALL dispose();
	};

	class Factory : public IFactory
	{
	public:
		virtual IRenderSystem * API_CALL createRenderSystem();
	};

	RenderSystem::RenderSystem()
	{
		_graphicsDevice = new GraphicsDevice();
	}

	RenderSystem::~RenderSystem()
	{
		delete _graphicsDevice;
	}

	void API_CALL RenderSystem::render()
	{
		_graphicsDevice->setClearState(0x7784aa, 1.0f);
		_graphicsDevice->clear();
	}

	void API_CALL RenderSystem::dispose()
	{
		delete this;
	}
	
	IRenderSystem * API_CALL Factory::createRenderSystem()
	{
		return new RenderSystem();
	}

	extern "C" DLLEXPORT IFactory * API_CALL createFactory()
	{
		return new Factory();
	}
}