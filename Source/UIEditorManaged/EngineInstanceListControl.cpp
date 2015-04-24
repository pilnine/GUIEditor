#include "UIEditorManagedPCH.h"
#include "EngineInstanceListControl.hpp"

#include "../UIEngine/Source/VUINodeList.h"


using namespace ManagedFramework;

namespace UIEditorManaged
{

  // create the native instance in the constructor
  UIEngineInstanceListControl::UIEngineInstanceListControl()
  {
	
  }

  UIEngineInstanceListControl::~UIEngineInstanceListControl()
  {
	 
  }

  void UIEngineInstanceListControl::Initialize()
  {
	  m_pUIControlInstance = VUINodeList::CreateInstance();

	  UIEngineInstanceShape2D::Initialize();
  }

  void UIEngineInstanceListControl::AddTestItem(int iCount) 
  {
	  for ( int i = 0 ; i < iCount ; i++)
	  {
		  VString szText;
		  szText.Format("TEXT - %d" , i);
		  ((VUINodeList*)m_pUIControlInstance)->AddListItem(szText,NULL);
	  }

	  return;
  }


  void UIEngineInstanceListControl::AddScrollbarControl( IEngineShapeInstance ^control )
  {
	  ((VUINodeList*)m_pUIControlInstance)->SetScrollBar( (VSliderControl*)(((UIEngineInstanceShape2D^)control)->m_pUIControlInstance) );	
  }
}
