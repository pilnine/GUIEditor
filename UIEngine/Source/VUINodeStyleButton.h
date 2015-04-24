#pragma once

#include "VUINode.hpp"


// One texture
// Color Variation for Disable , Normal , Select
// Button Texture Alpha Flag



class VUINodeStyleButton : public VPushButton
{
public:
	NODE_IMPEXP VUINodeStyleButton(void);
	virtual ~VUINodeStyleButton(void);

	NODE_IMPEXP static VUINodeStyleButton* CreateInstance();

	NODE_IMPEXP virtual void OnVariableValueChanged(VisVariable_cl *pVar, const char * value);

protected:

	V_DECLARE_SERIAL_DLLEXP( VUINodeStyleButton,  NODE_IMPEXP );
	NODE_IMPEXP virtual void Serialize( VArchive &ar );	


	NODE_IMPEXP virtual bool Build(TiXmlElement *pNode, const char *szPath, bool bWrite) HKV_OVERRIDE;

	IMPLEMENT_OBJ_CLASS(VUINodeStyleButton);



	VIS_TransparencyType m_eTransp;
	VImageState::ImageStretchMode_e m_eStretcheMode;
	VCompiledEffectPtr m_ShaderEffect;



};

