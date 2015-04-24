#pragma once

#include "VUINode.hpp"


/// \brief
///   Helper class providing various menu item colors.
class VAppMenuColors 
{
public:
	enum VDialogColors
	{
		COLOR_DIALOG_BG = 0,
		COLOR_TEXT,
		COLOR_LIST_CONTROL_BG,
		COLOR_LIST_ITEM_BG,
		COLOR_LIST_GROUP_BG,
		COLOR_LIST_ITEM_TEXT_NORMAL,
		COLOR_LIST_ITEM_TEXT_OVER,
		COLOR_LIST_ITEM_TEXT_SELECTED,

		COLOR_COUNT
	};

	//static void SetColor(VDialogColors item, const VColorRef& color) { VASSERT(item < COLOR_COUNT); m_colors[item] = color; }
	//static const VColorRef& GetColor(VDialogColors item) { VASSERT(item < COLOR_COUNT); return m_colors[item]; }

private:
	//static VColorRef m_colors[COLOR_COUNT];
};



class VUINodeColorButton : public VPushButton
{
public:
	NODE_IMPEXP VUINodeColorButton(void);
	virtual ~VUINodeColorButton(void);

	NODE_IMPEXP static VUINodeColorButton* CreateInstance();

	NODE_IMPEXP virtual void OnVariableValueChanged(VisVariable_cl *pVar, const char * value);

	NODE_IMPEXP virtual void OnPaint(VGraphicsInfo &Graphics, const VItemRenderInfo &parentState) HKV_OVERRIDE;

	NODE_IMPEXP virtual bool Build(TiXmlElement *pNode, const char *szPath, bool bWrite) HKV_OVERRIDE;

protected:

	V_DECLARE_SERIAL_DLLEXP( VUINodeColorButton,  NODE_IMPEXP );
	NODE_IMPEXP virtual void Serialize( VArchive &ar );		

	IMPLEMENT_OBJ_CLASS(VUINodeColorButton);

	VIS_TransparencyType m_eTransp;
	VImageState::ImageStretchMode_e m_eStretcheMode;
	VCompiledEffectPtr m_ShaderEffect;

private:
	float m_fBorderWidth;
	BOOL m_bUseGradient;
};

