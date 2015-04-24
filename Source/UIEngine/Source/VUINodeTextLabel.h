#pragma once

#include "UIEngineIncludes.hpp"
#include "VUINode.hpp"

class VUINodeTextLabel : public VTextLabel
{
public:
	NODE_IMPEXP VUINodeTextLabel(void);
	virtual ~VUINodeTextLabel(void);

	NODE_IMPEXP static VUINodeTextLabel* CreateInstance();
	NODE_IMPEXP virtual void OnVariableValueChanged(VisVariable_cl *pVar, const char * value);

	NODE_IMPEXP virtual bool Build(TiXmlElement *pNode, const char *szPath, bool bWrite) HKV_OVERRIDE;

protected:

	V_DECLARE_SERIAL_DLLEXP( VUINodeTextLabel,  NODE_IMPEXP );
	NODE_IMPEXP virtual void Serialize( VArchive &ar );	

	IMPLEMENT_OBJ_CLASS(VUINodeTextLabel);
};

