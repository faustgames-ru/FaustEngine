#ifndef ZOMBO_EDITOR_H
#define ZOMBO_EDITOR_H

#include "../zombo_classes.h"
#include "../../CommonSources/include/zombo.h"

namespace zombo
{
	class IEditorMode: public IBaseObject
	{
	public:
		virtual void API_CALL activated() =0;
		virtual void API_CALL deactivated() = 0;
		virtual void API_CALL update() = 0;
	};

	class ZomboEditor : public IZomboEditor
	{
	public:
		virtual void API_CALL setMode(String modeName) OVERRIDE;
		virtual void API_CALL update() OVERRIDE;
		virtual void API_CALL render() OVERRIDE;
		virtual void API_CALL release() OVERRIDE;
	private:
	};
}

#endif /* ZOMBO_EDITOR_H */