#pragma once

#include "UIEngineIncludes.hpp"
#include "VUINode.hpp"

class VUINodeModelPreviewControl : public VModelPreviewControl
{
public:
	NODE_IMPEXP VUINodeModelPreviewControl(void);
	virtual ~VUINodeModelPreviewControl(void);

	NODE_IMPEXP static VUINodeModelPreviewControl* CreateInstance();

	NODE_IMPEXP virtual void CreateDefaultModelView();

	NODE_IMPEXP virtual void OnVariableValueChanged(VisVariable_cl *pVar, const char * value);

	NODE_IMPEXP virtual bool Build(TiXmlElement *pNode, const char *szPath, bool bWrite) HKV_OVERRIDE;

protected:

	V_DECLARE_SERIAL_DLLEXP( VUINodeModelPreviewControl,  NODE_IMPEXP );
	NODE_IMPEXP virtual void Serialize( VArchive &ar );	

	IMPLEMENT_OBJ_CLASS(VUINodeModelPreviewControl);

	VString AnimationName; ///< Name of the animation that should be started
};

