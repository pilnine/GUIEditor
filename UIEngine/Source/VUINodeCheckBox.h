#pragma once

#include "UIEngineIncludes.hpp"
#include "VUINode.hpp"

class VUINodeCheckBox : public VCheckBox
{
public:
	NODE_IMPEXP VUINodeCheckBox(void);
	virtual ~VUINodeCheckBox(void);

	NODE_IMPEXP static VUINodeCheckBox* CreateInstance();
	NODE_IMPEXP virtual void OnVariableValueChanged(VisVariable_cl *pVar, const char * value);


	NODE_IMPEXP virtual bool Build(TiXmlElement *pNode, const char *szPath, bool bWrite) HKV_OVERRIDE;

	// toggle check status
	NODE_IMPEXP virtual void OnClick(VMenuEventDataObject *pEvent) HKV_OVERRIDE;	

protected:

	V_DECLARE_SERIAL_DLLEXP( VUINodeCheckBox,  NODE_IMPEXP );
	NODE_IMPEXP virtual void Serialize( VArchive &ar );	

	IMPLEMENT_OBJ_CLASS(VUINodeCheckBox);
};

