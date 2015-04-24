// TKBMS v1.0 -----------------------------------------------------
//
// PLATFORM       : ALL
// PRODUCT        : VISION 
// VISIBILITY     : PUBLIC
//
// ------------------------------------------------------TKBMS v1.0

#ifndef vPieGraphControls_h__
#define vPieGraphControls_h__

#include "UIEngineIncludes.hpp"
#include "VUINode.hpp"

///////////////////////////////////////////////////////////////////////////////////////////////////


class VUINodePieGraph : public VDlgControlBase
{
public:
	NODE_IMPEXP VUINodePieGraph(void);
	virtual ~VUINodePieGraph(void);

	NODE_IMPEXP static VUINodePieGraph* CreateInstance();

	virtual void OnPaint(VGraphicsInfo &Graphics, const VItemRenderInfo &parentState) HKV_OVERRIDE;
	virtual bool Build(TiXmlElement *pNode, const char *szPath, bool bWrite) HKV_OVERRIDE;
	virtual void OnBuildFinished() HKV_OVERRIDE;
	virtual VCursor *GetMouseOverCursor(VGUIUserInfo_t &user) HKV_OVERRIDE {return m_Image.m_States[GetCurrentState()].GetCursor();}

	/// \brief Returns the VImageStates object for this image.
	VImageStates& Image() {return m_Image;}

	void SetCount(int iValueCount);
	void SetValue(int iIndex , float fValue);
	void SetValueColor(int iIndex , VColorRef color);


	V_DECLARE_SERIAL_DLLEXP( VUINodePieGraph,  NODE_IMPEXP );
	NODE_IMPEXP virtual void Serialize( VArchive &ar );	

	IMPLEMENT_OBJ_CLASS(VUINodePieGraph);

protected:

	VImageStates m_Image;  ///< image
	int m_iValueCount;
	VArray<float> m_Values;
	VTextureObject *m_pTex;
	float m_fMaxValue;

	VColorRef m_ColorBackground;
	VColorRef m_ColorInside;
	VColorRef m_ColorOutside;
};


#endif // vCommonControls_h__
