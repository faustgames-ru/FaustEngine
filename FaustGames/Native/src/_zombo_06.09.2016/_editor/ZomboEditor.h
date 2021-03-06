#ifndef ZOMBO_EDITOR_H
#define ZOMBO_EDITOR_H

#include "../zombo_classes.h"
#include "../../CommonSources/include/zombo.h"
#include "EditorMode.h"
#include "../../fonts/OutlineVectorFont.h"
#include "../../fonts/BitmapFont.h"
#include "../content/ZomboContentBlock.h"
#include "../content/ZomboContentScene.h"
#include "../content/ZomboContentGame.h"
#include "toolbox/ZomboToolBox.h"

namespace zombo
{
	class ZomboEditorSharedData
	{
	public:
		static ZomboEditorSharedData Default;
		ZomboEditorSharedData();
	};
	class ZomboEditorConstants
	{
	public:
		static const int BuildInFontSize = 64;
		static std::string fpsPattern;
		static std::string fpsAvrLabel;
		static std::string fpsMinLabel;
		static std::string fpsMaxLabel;
		static std::string LoadingScreenFont;
		static std::string LogDisplayerFont;
		static std::string StatisticsDisplayerFont;
		static std::string PointRingImage;
		static std::string PointBoxImage;
		static std::string GameFile;
	};

	class ZomboEditor : public IZomboEditor
	{
	public:
		ZomboEditor();
		static ZomboEditor Default;
		String getEditorModeInternal() const;
		void setEditorModeInternal(String mode);
		ZomboContentBlock internalContent;
		ZomboContentBlock gameContent;

		virtual void API_CALL setRootPath(String rootPath) OVERRIDE;
		virtual IntPtr API_CALL getMode() OVERRIDE;
		virtual void API_CALL setMode(String modeName) OVERRIDE;
		virtual bool API_CALL isUndoAvaliable() OVERRIDE;
		virtual bool API_CALL isRedoAvaliable() OVERRIDE;
		virtual void API_CALL undo() OVERRIDE;
		virtual void API_CALL redo() OVERRIDE;
		virtual void API_CALL init() OVERRIDE;
		virtual void API_CALL update(float ellapsedTime) OVERRIDE;
		virtual void API_CALL render() OVERRIDE;
		virtual void API_CALL release() OVERRIDE;
		virtual void API_CALL finish() OVERRIDE;
	private:
		void renderBackground();
		void renderBackgroundAsSkybox();
		void renderGui();
		void internalUpdate();
		void contentLoaded();

		void addBackgroundEdgeVertex(uint i);

		std::string _actualModeName;
		IEditorMode *_mode;
		std::string _rootPath;
		core::MatrixContainer _guiMatrix;
		core::MatrixContainer _backgroundMatrix;
		bool _needCallContetnLoaded;
		ZomboContentGame *_game;
		ZomboContentScene *_scene;
		std::vector<ZomboContentPlatform *> _platforms;
		ZomboContentImage* _background;
		std::vector<RenderVertex> _backgroundVertices;
		std::vector<ushort> _backgroundIndices;
		std::vector<ColorVertex> _backgroundEdgesVertices;
		SFloat _backgrounAlpha;
		bool _renderBackgroundEdges;
	};
}
#endif /* ZOMBO_EDITOR_H */