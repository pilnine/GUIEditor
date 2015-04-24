/// \file VGUIContext.hpp

#ifndef VDefault_GUI_CONTEXT_HPP_INCLUDED
#define VDefault_GUI_CONTEXT_HPP_INCLUDED

#include <Vision/Runtime/EnginePlugins/VisionEnginePlugin/GUI/VGUIIncludes.hpp>
//#include <Vision/Runtime/EnginePlugins/VisionEnginePlugin/GUI/VGUIContext.hpp>



/// \brief
///   Resolution aware GUI context
class VDefaultGUIContext : public VGUIMainContext , public VisTypedEngineObject_cl
{
protected:
  VRectanglef rReferenceResolution;


  float m_fScaleX;
  float m_fScaleY;
  
public:
  VDialogCollection m_ClosedDialogs;
// 
//   void SetScale(float fScale)
//   {
// 	  m_fScaleX = fScale;
// 	  m_fScaleY = fScale;
//   }

  void SetScale(hkvVec2 vScale)
  {
	  m_fScaleX = vScale.x;
	  m_fScaleY = vScale.y;

	  m_ClientRect.Set(0.f,0.f, (float)Vision::Video.GetXRes() / m_fScaleX , (float)Vision::Video.GetYRes() / m_fScaleY);
  }

  float GetScaleX() const { return m_fScaleX; }
  float GetScaleY() const { return m_fScaleY; }

  hkvVec2 GetScale() { return hkvVec2(m_fScaleX,m_fScaleY);}

  /// \brief
  ///   Gets the current mouse position (clamped to client rect)
  inline const hkvVec2 GetCurrentScaledMousePos(const VGUIUserInfo_t *pUser=NULL) const 
  {
	  hkvVec2 vec(0,0);
	  int i = (pUser!=NULL) ? pUser->m_iID : 0;	

	  vec.x = m_UserStates[i].m_vMousePos.x * m_fScaleX;
		vec.y = m_UserStates[i].m_vMousePos.y * m_fScaleY;

	  return vec ;
  }

  /// \brief
  ///   Indicates whether constex is currently in dragging mode
  inline VWindowBasePtr GetDraggingItem(const VGUIUserInfo_t *pUser=NULL) const 
  {
	  int i = (pUser!=NULL) ? pUser->m_iID : 0;
	  return m_UserStates[i].m_spDragItem;
  }

 NODE_IMPEXP hkvVec2 GetGUIPositionFromScreenPos( hkvVec2 vPos );
 NODE_IMPEXP hkvVec2 GetScreenPositionFromGUIPos( hkvVec2 vPos );



  NODE_IMPEXP VDefaultGUIContext(void);

  /// \brief
  ///   Constructor of the GUI context instance
  NODE_IMPEXP VDefaultGUIContext(VGUIManager *pManager, float fReferenceResolutionX = 2048.0f, float fReferenceResolutionY = 1536.0f) :
  VGUIMainContext(pManager), rReferenceResolution(0, 0, fReferenceResolutionX, fReferenceResolutionY)
  {


#ifdef WIN32
	  m_fScaleX  = 1.0f;
	  m_fScaleY  = 1.0f;
#else
	  m_fScaleX  = 1.0f;
	  m_fScaleY  = 1.0f;
#endif

	  m_vDragSpeed.setZero();

	  m_ClientRect.Set(0.f,0.f, (float)Vision::Video.GetXRes(), (float)Vision::Video.GetYRes());

	  float fValue = Vision::Video.GetDeviceDpi() / 96.0f * 10.0f;
	  this->SetDragThreshold(fValue);
  };

  /// \brief
  ///   Destructor
  virtual ~VDefaultGUIContext() {};


  virtual void UpdateCursorPos(VGUIUserInfo_t &userState);

  virtual void OnRender();

   NODE_IMPEXP virtual void OnTickFunction(float fTimeDelta);

  NODE_IMPEXP virtual void OnVariableValueChanged(VisVariable_cl *pVar, const char * value);

  NODE_IMPEXP virtual void ShowDIalogByString(VString szDialog);

  NODE_IMPEXP virtual void CloseDialog(VDialog* pDialog);
  

  // serialisation:
  V_DECLARE_SERIAL_DLLEXP( VDefaultGUIContext,  NODE_IMPEXP );
  NODE_IMPEXP virtual void Serialize( VArchive &ar );
  IMPLEMENT_OBJ_CLASS(VDefaultGUIContext);  

  hkvVec2 m_vDragSpeed;
};

typedef VSmartPtr<VDefaultGUIContext> VDefaultGUIContextPtr;

#endif

