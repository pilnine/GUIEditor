#include "UIEditorManagedPCH.h"
#include "EngineInstanceSliderControl.hpp"

#include "../UIEngine/Source/VUINodeSliderControl.h"


using namespace ManagedFramework;

namespace UIEditorManaged
{

  // create the native instance in the constructor
  UIEngineInstanceSliderControl::UIEngineInstanceSliderControl()
  {
	
  }

  UIEngineInstanceSliderControl::~UIEngineInstanceSliderControl()
  {
	 
  }

  void UIEngineInstanceSliderControl::Initialize()
  {
	  m_pUIControlInstance = VUINodeSliderControl::CreateInstance();

	  UIEngineInstanceShape2D::Initialize();
  }
}
