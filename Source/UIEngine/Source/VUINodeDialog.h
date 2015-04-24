#pragma once

#include "UIEngineIncludes.hpp"
#include "VUINode.hpp"



NODE_IMPEXP enum MessageBoxResult
{
	MessageBoxResult_YES,
	MessageBoxResult_NO,
	MessageBoxResult_OK,
};

//class VUINodeDialog  : public VUINode_cl , public VDialog
class VUINodeDialog  : public VDialog , public IVisCallbackHandler_cl
{
public:

	enum VDialogState
	{
		STATE_NONE = 0,
		STATE_FADE_IN,
		STATE_FADE_OUT,
		STATE_HIDE,
		STATE_OPEN,
		STATE_BACK
	};


	NODE_IMPEXP VUINodeDialog(void);
	NODE_IMPEXP virtual ~VUINodeDialog(void);

	NODE_IMPEXP static VUINodeDialog* CreateInstance( VUINodeDialog* pNewDialog );


	void CommonInit();

	NODE_IMPEXP virtual void DoLayout();
	NODE_IMPEXP virtual void BuildFromXML(VString szFileName);

	NODE_IMPEXP virtual bool BuildToXML( VString szFileName );

	NODE_IMPEXP virtual void OnHandleCallback(IVisCallbackDataObject_cl *pData);


	NODE_IMPEXP virtual void OnItemClicked(VMenuEventDataObject *pEvent);

	NODE_IMPEXP virtual void OnMessageBoxClicked( MessageBoxResult result ) {}

	NODE_IMPEXP virtual void SetBackgroundTexture(VString filename);
	NODE_IMPEXP virtual void SetBackgroundColor(VColorRef color);

	NODE_IMPEXP virtual void SetFullScreen(bool bFullScreen);

	NODE_IMPEXP virtual void SetFadeIn(bool bFadeIn , float fFadeInPos , float fFadeInTime );
	NODE_IMPEXP virtual void SetFadeOut(bool bFadeOut , VColorRef iBackFadeColor , float fBackFadePos , float fFadeOutTime);

	NODE_IMPEXP virtual void OnVariableValueChanged(VisVariable_cl *pVar, const char * value);

	NODE_IMPEXP virtual void OnPaint(VGraphicsInfo &Graphics, const VItemRenderInfo &parentState);
	NODE_IMPEXP virtual void OnTick(float dtime);

	NODE_IMPEXP virtual void Reset();
	NODE_IMPEXP virtual void DoFadeIn();
	NODE_IMPEXP virtual void DoFadeOut(const VDialogState& followUpState);


	bool m_bVisible;

	NODE_IMPEXP VOVERRIDE void Serialize( VArchive &ar );
	NODE_IMPEXP VOVERRIDE void OnSerialized( VArchive &ar );

	NODE_IMPEXP VOVERRIDE VDlgControlBase* GetControl(VString szControlID);

	VisCallback_cl m_OnFadeoutFinished;

	NODE_IMPEXP VOVERRIDE void AddControl(VDlgControlBase *pItem);

	NODE_IMPEXP VOVERRIDE void SetStartPosition();

	NODE_IMPEXP VOVERRIDE void SetParam(VArray<VString> param){m_nParam = param; }
	NODE_IMPEXP VOVERRIDE VArray<VString> GetParam(){return m_nParam; }
	
	void UpdateAnimation();

	int m_iStartDirection; // 0 - None , 1 - From Up , 2 - From Down , 3 - From Left , 4 - From Right

	hkvVec2 m_vOriginalPos;
	hkvVec2 m_vTempPos;

	float m_fMovingTime;

protected:

	//serialization
	V_DECLARE_SERIAL_DLLEXP( VUINodeDialog,  NODE_IMPEXP );
	V_DECLARE_VARTABLE(VDialog, NODE_IMPEXP);

	
	//EFFECTS_IMPEXP VOVERRIDE void Serialize( VArchive &ar );

	IMPLEMENT_OBJ_CLASS(VUINodeDialog)

	bool m_bFadeOut;
	VColorRef m_bFadeOutColor;
	float m_fFadeOutPos; // 0.0f ~ 1.0f;
	float m_fFadeOutSpeed; // 1.0f -> 1 second
	VArray<VString> m_nParam;

	VDialogState m_state;
	VDialogState m_followUpState;
	float m_fTimer;

	float m_fFadeTime; // = 0.15f;

	float m_fPositionOffsetX;
	float m_fPositionOffsetY;

};

