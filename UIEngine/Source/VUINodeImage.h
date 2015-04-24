#pragma once

#include "UIEngineIncludes.hpp"
#include "VUINode.hpp"

class VUINodeImage :  public VImageControl
{
public:
	NODE_IMPEXP VUINodeImage(void);
	virtual ~VUINodeImage(void);

	NODE_IMPEXP static VUINodeImage* CreateInstance();

	NODE_IMPEXP virtual void OnVariableValueChanged(VisVariable_cl *pVar, const char * value);
 
	NODE_IMPEXP virtual void OnPaint(VGraphicsInfo &Graphics, const VItemRenderInfo &parentState) HKV_OVERRIDE;

	NODE_IMPEXP virtual bool Build(TiXmlElement *pNode, const char *szPath, bool bWrite) HKV_OVERRIDE;

	NODE_IMPEXP virtual void SetColor( VColorRef color );
	NODE_IMPEXP virtual VColorRef GetColor();


protected:

	V_DECLARE_SERIAL_DLLEXP( VUINodeImage,  NODE_IMPEXP );
	NODE_IMPEXP virtual void Serialize( VArchive &ar );	

	IMPLEMENT_OBJ_CLASS(VUINodeImage);


	VIS_TransparencyType m_eTransp;
	VImageState::ImageStretchMode_e m_eStretcheMode;


	//VCompiledEffectPtr m_ShaderEffect;
	//int m_eStretcheMode;
};

