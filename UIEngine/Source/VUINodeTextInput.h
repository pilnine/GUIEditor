#pragma once

#include "UIEngineIncludes.hpp"
#include "VUINode.hpp"

class VUINodeTextInput : public VTextControl
{
public:
	NODE_IMPEXP VUINodeTextInput(void);
	virtual ~VUINodeTextInput(void);

	NODE_IMPEXP static VUINodeTextInput* CreateInstance();

	NODE_IMPEXP virtual void OnVariableValueChanged(VisVariable_cl *pVar, const char * value);
	NODE_IMPEXP VTextStates &Text() {return m_Text;}

	NODE_IMPEXP virtual void OnPaint(VGraphicsInfo &Graphics, const VItemRenderInfo &parentState) HKV_OVERRIDE;


	NODE_IMPEXP virtual bool Build(TiXmlElement *pNode, const char *szPath, bool bWrite) HKV_OVERRIDE;

	NODE_IMPEXP virtual void SetText(const char *szText) HKV_OVERRIDE;


	NODE_IMPEXP virtual void SetCursorPosMultiLine(int iCharacterPos, bool bEnsureVisible=true);
	NODE_IMPEXP virtual void OnClick(VMenuEventDataObject *pEvent) HKV_OVERRIDE;



#ifdef SUPPORTS_SOFTKEYBOARD
	NODE_IMPEXP virtual void EnterText(const char* pUTF8Text);
	NODE_IMPEXP virtual void OnSpecialKey(unsigned int uiKey); 
#endif

protected:

	V_DECLARE_SERIAL_DLLEXP( VUINodeTextInput,  NODE_IMPEXP_DATA );
	NODE_IMPEXP virtual void Serialize( VArchive &ar );	

	IMPLEMENT_OBJ_CLASS(VUINodeTextInput);

	bool m_bMultiLine;
};

