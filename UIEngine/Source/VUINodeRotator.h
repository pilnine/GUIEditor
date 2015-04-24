#pragma once

#include "UIEngineIncludes.hpp"
#include "VUINode.hpp"

class VUINodeRotator : public VPushButton
{
public:
	NODE_IMPEXP VUINodeRotator(void);
	virtual ~VUINodeRotator(void);

	NODE_IMPEXP static VUINodeRotator* CreateInstance();
	NODE_IMPEXP virtual void OnVariableValueChanged(VisVariable_cl *pVar, const char * value);


	NODE_IMPEXP virtual bool Build(TiXmlElement *pNode, const char *szPath, bool bWrite) HKV_OVERRIDE;

	// toggle check status
	NODE_IMPEXP virtual void OnClick(VMenuEventDataObject *pEvent) HKV_OVERRIDE;
	NODE_IMPEXP virtual void OnPointerDown(VMenuEventDataObject *pEvent) HKV_OVERRIDE;
	NODE_IMPEXP virtual void OnPointerUp(VMenuEventDataObject *pEvent) HKV_OVERRIDE;

	NODE_IMPEXP virtual void OnDragBegin(const hkvVec2 &vMousePos, int iButtonMask)  HKV_OVERRIDE;
	NODE_IMPEXP virtual void OnDragging(const hkvVec2 &vMouseDelta)  HKV_OVERRIDE;
	NODE_IMPEXP virtual void OnDragEnd(VWindowBase *pOver)  HKV_OVERRIDE;

	NODE_IMPEXP virtual void RotateImage(float fAngle);

	 NODE_IMPEXP virtual void OnPaint(VGraphicsInfo &Graphics, const VItemRenderInfo &parentState) HKV_OVERRIDE;

	 float GetAngle() const { return m_fAngle; }
	 void SetAngle(float val) { m_fAngle = val; }

	 float AngleDelta() const { return m_fAngleDelta; }
	 void AngleDelta(float val) { m_fAngleDelta = val; }


protected:

	float GetAngleBetweenVectors( hkvVec2 v1);

	float m_fAngle;
	float m_fAngleDelta;
	
	
	float m_fPivotAngle;
	hkvVec2 m_vPivotPoint;
	hkvVec2 m_vEndPoint;

	V_DECLARE_SERIAL_DLLEXP( VUINodeRotator,  NODE_IMPEXP );
	NODE_IMPEXP virtual void Serialize( VArchive &ar );	

	IMPLEMENT_OBJ_CLASS(VUINodeRotator);
};

