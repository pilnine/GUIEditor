#include "UIEditorManagedPCH.h"
#include "EngineInstanceCheckBox.hpp"

#include "../UIEngine/Source/VUINodeCheckBox.h"


using namespace ManagedFramework;

namespace UIEditorManaged
{

  // create the native instance in the constructor
  UIEngineInstanceCheckBox::UIEngineInstanceCheckBox()
  {
	  UVTransform = new hkvMat3();
	  PosTransform = new hkvMat3();
  }

  UIEngineInstanceCheckBox::~UIEngineInstanceCheckBox()
  {
	  UVTransform = NULL;
	  PosTransform = NULL;
	  //V_SAFE_DELETE(UVTransform);
	  //V_SAFE_DELETE(PosTransform);
  }

  void UIEngineInstanceCheckBox::Initialize()
  {
	  m_pUIControlInstance = VUINodeCheckBox::CreateInstance();

	  //UIEngineInstanceShape2D::Initialize();	  
	
	  m_pUIControlInstance->SetSize(500,100);	   
	  m_pUIControlInstance->SetPosition(0,0);
	  m_pUIControlInstance->SetVisible(true);
  }
  
  void UIEngineInstanceCheckBox::SetUVTransform( float x, float y , float z )
  {
	UVTransform->setFromEulerAngles( z , y , x) ;
	((VUINodeCheckBox*)m_pUIControlInstance)->Image().SetTransformation( UVTransform,PosTransform );
  }

  void UIEngineInstanceCheckBox::SetPosTransform( float x, float y , float z )
  {
	PosTransform->setFromEulerAngles( z , y , x) ;
	((VUINodeCheckBox*)m_pUIControlInstance)->Image().SetTransformation( UVTransform,PosTransform );
  }

  void UIEngineInstanceCheckBox::SetTexureTransform(Vector3F^ uvTransform , Vector3F^ posTransform)
  {
	  SetUVTransform(uvTransform->Z , uvTransform->Y,uvTransform->X);
	  SetPosTransform(posTransform->Z , posTransform->Y,posTransform->X);
// 	  hkvMat3 UVTransform;
// 	  UVTransform.setFromEulerAngles( 0,0,0 );
// 
// 	  hkvMat3 PosTransform;
// 	  PosTransform.setFromEulerAngles( 0,0,0 );

	//  ((VUINodeCheckBox*)m_pUIControlInstance)->Image().SetTransformation( &UVTransform,&PosTransform );
  }

  void UIEngineInstanceCheckBox::SetStretchMode(int mode)
  {

  }

  void UIEngineInstanceCheckBox::SetTransparency(int type)
  {

  }
}
