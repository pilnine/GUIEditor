#pragma once

#include "VUINode.hpp"

class VUINodePushButton : public VPushButton
{
public:
	NODE_IMPEXP VUINodePushButton(void);
	virtual ~VUINodePushButton(void);

	NODE_IMPEXP static VUINodePushButton* CreateInstance();

	NODE_IMPEXP virtual void OnMouseLeave(VGUIUserInfo_t &user) HKV_OVERRIDE 
	{
		VDlgControlBase::OnMouseLeave(user); SetSelected(false);
	}

	NODE_IMPEXP virtual void OnVariableValueChanged(VisVariable_cl *pVar, const char * value);

	NODE_IMPEXP void SetID(int iID) { m_iID = iID;}

	NODE_IMPEXP void SetTexture( VString szTextureFile );

protected:

	V_DECLARE_SERIAL_DLLEXP( VUINodePushButton,  NODE_IMPEXP );
	NODE_IMPEXP virtual void Serialize( VArchive &ar );	


	NODE_IMPEXP virtual bool Build(TiXmlElement *pNode, const char *szPath, bool bWrite) HKV_OVERRIDE;

	IMPLEMENT_OBJ_CLASS(VUINodePushButton);



	VIS_TransparencyType m_eTransp;
	VImageState::ImageStretchMode_e m_eStretcheMode;
	VCompiledEffectPtr m_ShaderEffect;



};

