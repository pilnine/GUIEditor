#pragma once

#include "UIEngineIncludes.hpp"
#include "VUINode.hpp"

class TabButton;

//class VUINodeTabDialog  : public VUINode_cl , public VDialog
class VUINodeTabDialog  : public VDialog , public IVisCallbackHandler_cl
{
public:
	NODE_IMPEXP VUINodeTabDialog(void);
	NODE_IMPEXP virtual ~VUINodeTabDialog(void);

	NODE_IMPEXP static VUINodeTabDialog* CreateInstance( VUINodeTabDialog* pNewDialog );


	void CommonInit();


	NODE_IMPEXP void BuildFromXML(VString szFileName);

	NODE_IMPEXP virtual bool BuildToXML( VString szFileName );

	NODE_IMPEXP virtual void OnHandleCallback(IVisCallbackDataObject_cl *pData);


	NODE_IMPEXP virtual void OnItemClicked(VMenuEventDataObject *pEvent);

	NODE_IMPEXP virtual void SetBackgroundTexture(VString filename);
	NODE_IMPEXP virtual void SetBackgroundColor(VColorRef color);


	NODE_IMPEXP virtual void SetFullScreen(bool bFullScreen);

	NODE_IMPEXP virtual void SetFadeIn(bool bFadeIn , float fFadeInPos , float fFadeInTime );
	NODE_IMPEXP virtual void SetFadeOut(bool bFadeOut , VColorRef iBackFadeColor , float fBackFadePos , float fFadeOutTime);

	NODE_IMPEXP virtual void OnVariableValueChanged(VisVariable_cl *pVar, const char * value);

	NODE_IMPEXP virtual void OnPaint(VGraphicsInfo &Graphics, const VItemRenderInfo &parentState);


	bool m_bVisible;

	NODE_IMPEXP VOVERRIDE void Serialize( VArchive &ar );
	NODE_IMPEXP VOVERRIDE void OnSerialized( VArchive &ar );


protected:


	//serialization
	V_DECLARE_SERIAL_DLLEXP( VUINodeTabDialog,  NODE_IMPEXP );
	V_DECLARE_VARTABLE(VDialog, NODE_IMPEXP);

	
	//EFFECTS_IMPEXP VOVERRIDE void Serialize( VArchive &ar );

	IMPLEMENT_OBJ_CLASS(VUINodeTabDialog)
};

