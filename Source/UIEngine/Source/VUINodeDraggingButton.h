#pragma once

#include "VUINode.hpp"
#include "VUINodePushButton.h"
#include "VUINodeStyleButton.h"


// One texture
// Color Variation for Disable , Normal , Select
// Button Texture Alpha Flag



class VUINodeDraggingButton : public VUINodePushButton
{
public:
	NODE_IMPEXP VUINodeDraggingButton(void);
	virtual ~VUINodeDraggingButton(void);

	NODE_IMPEXP static VUINodeDraggingButton* CreateInstance();

	NODE_IMPEXP virtual void OnVariableValueChanged(VisVariable_cl *pVar, const char * value);

	NODE_IMPEXP virtual void OnDragBegin(const hkvVec2 &vMousePos, int iButtonMask)  HKV_OVERRIDE;
	NODE_IMPEXP virtual void OnDragging(const hkvVec2 &vMouseDelta)  HKV_OVERRIDE;
	NODE_IMPEXP virtual void OnDragEnd(VWindowBase *pOver)  HKV_OVERRIDE;

	NODE_IMPEXP virtual void RecoverPosition(); 

	NODE_IMPEXP virtual hkvVec2 GetDeltaPos()
	{
		hkvVec2 vScaledPos (VUINodeMananger_cl::GlobalManager()->GetGUIContext()->GetScaleX() * m_vDeltaPos.x , 
			VUINodeMananger_cl::GlobalManager()->GetGUIContext()->GetScaleY() * m_vDeltaPos.y );

		return vScaledPos;
	}

	NODE_IMPEXP virtual void SetDragRange(VRectanglef rect);

protected:

	V_DECLARE_SERIAL_DLLEXP( VUINodeDraggingButton,  NODE_IMPEXP );
//	NODE_IMPEXP virtual void Serialize( VArchive &ar );	


	NODE_IMPEXP virtual bool Build(TiXmlElement *pNode, const char *szPath, bool bWrite) HKV_OVERRIDE;

	IMPLEMENT_OBJ_CLASS(VUINodeDraggingButton);

	hkvVec2 m_vPivotPoint;
	hkvVec2 m_vInitPos;

	hkvVec2 m_vDeltaPos;

	float m_fTempPositionX;
	float m_fTempPositionY;

	VRectanglef m_fDragRange;


};

