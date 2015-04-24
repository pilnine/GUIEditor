#pragma once

#include "UIEngineIncludes.hpp"
#include "VUINode.hpp"
#include "VUINodeListItem.h"

class VUINodeList : public VListControl
{
public:
	NODE_IMPEXP VUINodeList(void);
	virtual ~VUINodeList(void);

	NODE_IMPEXP static VUINodeList* CreateInstance();

	NODE_IMPEXP virtual void OnVariableValueChanged(VisVariable_cl *pVar, const char * value);


	NODE_IMPEXP virtual void OnPaint(VGraphicsInfo &Graphics, const VItemRenderInfo &parentState);


	NODE_IMPEXP virtual bool Build(TiXmlElement *pNode, const char *szPath, bool bWrite) HKV_OVERRIDE;

	NODE_IMPEXP virtual VUINodeListItem* AddListItem(const char* szText, VTextureObject* pThumbnail);

	NODE_IMPEXP virtual void SetScrollBar2( VSliderControl* pScrollbar );

protected:

	BOOL bShowScrollBar;

	V_DECLARE_SERIAL_DLLEXP( VUINodeList,  NODE_IMPEXP );
	NODE_IMPEXP virtual void Serialize( VArchive &ar );	

	IMPLEMENT_OBJ_CLASS(VUINodeList);

	VString ItemText;
};

