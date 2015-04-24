#pragma once

#include "UIEngineIncludes.hpp"
#include "VUINode.hpp"

class VUINodeListItem : public VListControlItem
{
public:
	NODE_IMPEXP VUINodeListItem();
	NODE_IMPEXP VUINodeListItem(const char* szText, VTextureObject* pThumbnail);
	NODE_IMPEXP virtual ~VUINodeListItem(void);
	

	NODE_IMPEXP static VUINodeListItem* CreateInstance();

	NODE_IMPEXP virtual void OnPaint(VGraphicsInfo &Graphics, const VItemRenderInfo &parentState) HKV_OVERRIDE;
	NODE_IMPEXP virtual bool Build(TiXmlElement *pNode, const char *szPath, bool bWrite) HKV_OVERRIDE;
	NODE_IMPEXP virtual void OnMeasureItem(const VRectanglef &area, hkvVec2 &vCurrentPos) HKV_OVERRIDE;


protected:

	VString m_szString;

	VArray<VWindowBase*> m_vControls;


	V_DECLARE_SERIAL_DLLEXP( VUINodeListItem,  NODE_IMPEXP );
	NODE_IMPEXP virtual void Serialize( VArchive &ar );	

	IMPLEMENT_OBJ_CLASS(VUINodeListItem)
};

