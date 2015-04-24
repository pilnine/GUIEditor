#include "UIEditorManagedPCH.h"
#include "EngineInstanceImage.hpp"

#include "../UIEngine/Source/VUINodeImage.h"


using namespace ManagedFramework;

namespace UIEditorManaged
{

  // create the native instance in the constructor
  UIEngineInstanceImage::UIEngineInstanceImage()
  {
	  UVTransform = new hkvMat3();
	  PosTransform = new hkvMat3();
  }

  UIEngineInstanceImage::~UIEngineInstanceImage()
  {
	  UVTransform = NULL;
	  PosTransform = NULL;
	  //V_SAFE_DELETE(UVTransform);
	  //V_SAFE_DELETE(PosTransform);
  }

  void UIEngineInstanceImage::Initialize()
  {
	  m_pUIControlInstance = VUINodeImage::CreateInstance();

	  UIEngineInstanceShape2D::Initialize();
  }

  void UIEngineInstanceImage::SetUVTransform( float x, float y , float z )
  {
	UVTransform->setFromEulerAngles( z , y , x) ;
	((VUINodeImage*)m_pUIControlInstance)->Image().SetTransformation( UVTransform,PosTransform );
  }

  void UIEngineInstanceImage::SetPosTransform( float x, float y , float z )
  {
	PosTransform->setFromEulerAngles( z , y , x) ;
	((VUINodeImage*)m_pUIControlInstance)->Image().SetTransformation( UVTransform,PosTransform );
  } 
}
