#include "UIEditorManagedPCH.h"
#include "EngineInstanceTextLabel.hpp"

#include "../UIEngine/Source/VUINodeTextLabel.h"


using namespace ManagedFramework;

namespace UIEditorManaged
{

  // create the native instance in the constructor
  UIEngineInstanceTextLabel::UIEngineInstanceTextLabel()
  {
	
  }

  UIEngineInstanceTextLabel::~UIEngineInstanceTextLabel()
  {
	 
  }

  void UIEngineInstanceTextLabel::Initialize()
  {
	  m_pUIControlInstance = VUINodeTextLabel::CreateInstance();

	  UIEngineInstanceShape2D::Initialize();
  }
}
