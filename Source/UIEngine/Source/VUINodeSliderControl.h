#pragma once

#include "UIEngineIncludes.hpp"
#include "VUINode.hpp"

class VUINodeSliderControl : public VSliderControl
{
public:
	NODE_IMPEXP VUINodeSliderControl(void);
	virtual ~VUINodeSliderControl(void);

	NODE_IMPEXP static VUINodeSliderControl* CreateInstance();

	NODE_IMPEXP virtual void OnVariableValueChanged(VisVariable_cl *pVar, const char * value);


	NODE_IMPEXP virtual bool Build(TiXmlElement *pNode, const char *szPath, bool bWrite) HKV_OVERRIDE;

protected:

	V_DECLARE_SERIAL_DLLEXP( VUINodeSliderControl,  NODE_IMPEXP );
	NODE_IMPEXP virtual void Serialize( VArchive &ar );	

	IMPLEMENT_OBJ_CLASS(VUINodeSliderControl);

	//BOOL m_bVertical;
};

