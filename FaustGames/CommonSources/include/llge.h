#ifndef LLGE_H
#define LLGE_H

#define API_CALL __stdcall
#define DLLEXPORT __declspec( dllexport )

namespace llge
{
	class IRenderSystem
	{
	public:		
		virtual void API_CALL render() = 0;
		virtual void API_CALL dispose() = 0;
	};

	class IFactory
	{
	public:
		virtual IRenderSystem * API_CALL createRenderSystem() = 0;
	};

	extern "C" DLLEXPORT IFactory * API_CALL createFactory();
}

#endif /*LLGE_H*/