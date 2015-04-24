#include "UIEditorManagedPCH.h"
#include "EngineInstanceTextInput.hpp"

#include "../UIEngine/Source/VUINodeTextInput.h"


using namespace ManagedFramework;

namespace UIEditorManaged
{

  // create the native instance in the constructor
  UIEngineInstanceTextInput::UIEngineInstanceTextInput()
  {
	
  }

  UIEngineInstanceTextInput::~UIEngineInstanceTextInput()
  {
	 
  }

  void UIEngineInstanceTextInput::Initialize()
  {
	  m_pUIControlInstance = VUINodeTextInput::CreateInstance();

	  UIEngineInstanceShape2D::Initialize();
  }
}
