#pragma once

#include "UIEngineIncludes.hpp"
#include "VUINode.hpp"

class VUINodeNumericUpDown : public VCheckBox
{
public:
	NODE_IMPEXP VUINodeNumericUpDown(void);
	virtual ~VUINodeNumericUpDown(void);

	NODE_IMPEXP virtual void OnVariableValueChanged(VisVariable_cl *pVar, const char * value);


	NODE_IMPEXP virtual bool Build(TiXmlElement *pNode, const char *szPath, bool bWrite) HKV_OVERRIDE;

	NODE_IMPEXP virtual void OnDragBegin(const hkvVec2 &vMousePos, int iButtonMask)  HKV_OVERRIDE;
	NODE_IMPEXP virtual void OnDragging(const hkvVec2 &vMouseDelta)  HKV_OVERRIDE;
	NODE_IMPEXP virtual void OnDragEnd(VWindowBase *pOver)  HKV_OVERRIDE;

	NODE_IMPEXP virtual void OnPaint(VGraphicsInfo &Graphics, const VItemRenderInfo &parentState) HKV_OVERRIDE;

	NODE_IMPEXP virtual void OnPointerDown(VMenuEventDataObject *pEvent);
	NODE_IMPEXP virtual void OnPointerUp(VMenuEventDataObject *pEvent);
	NODE_IMPEXP virtual void OnMouseLeave(VGUIUserInfo_t &user);

	NODE_IMPEXP virtual void OnTick(float dtime);
	NODE_IMPEXP virtual void OnSizeChanged();


	VPushButton *m_pButtonUp;
	VPushButton *m_pButtonDown;

	float m_Default;
	float m_tick;
	float m_Min;
	float m_Max;
	float m_fButtonLength;

protected:

	V_DECLARE_SERIAL_DLLEXP( VUINodeNumericUpDown,  NODE_IMPEXP );
	NODE_IMPEXP virtual void Serialize( VArchive &ar );	

	IMPLEMENT_OBJ_CLASS(VUINodeNumericUpDown);
};

