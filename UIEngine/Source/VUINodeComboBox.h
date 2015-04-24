#pragma once

#include "UIEngineIncludes.hpp"
#include "VUINode.hpp"

class VUINodeComboBox :  public VPushButton
{
public:
	NODE_IMPEXP VUINodeComboBox(void);
	virtual ~VUINodeComboBox(void);

	NODE_IMPEXP static VUINodeComboBox* CreateInstance();

	NODE_IMPEXP virtual void OnVariableValueChanged(VisVariable_cl *pVar, const char * value);
 
	NODE_IMPEXP virtual bool Build(TiXmlElement *pNode, const char *szPath, bool bWrite) HKV_OVERRIDE;

	void SetItemList ( VArray<VString> szItemList );

protected:

	V_DECLARE_SERIAL_DLLEXP( VUINodeComboBox,  NODE_IMPEXP );
	NODE_IMPEXP virtual void Serialize( VArchive &ar );	

	IMPLEMENT_OBJ_CLASS(VUINodeComboBox);


	VArray<VString> m_ItemList;
	int m_iIndex;


	VIS_TransparencyType m_eTransp;
	VImageState::ImageStretchMode_e m_eStretcheMode;
};

