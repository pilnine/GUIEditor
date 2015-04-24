#pragma once

using namespace CSharpFramework;
using namespace CSharpFramework::Math;
using namespace CSharpFramework::Shapes;
using namespace CSharpFramework::Scene;


#include "EngineInstanceShape2D.hpp"


namespace UIEditorManaged
{
  /// <summary>
  /// EngineInstanceNode : class that manages the native class
  /// </summary>

  public ref class UIEngineInstanceListControl : public UIEngineInstanceShape2D
  {
  public:     

    UIEngineInstanceListControl();
	virtual ~UIEngineInstanceListControl();


	VOVERRIDE void Initialize() override; 	

	VOVERRIDE void AddScrollbarControl( IEngineShapeInstance ^control );
	VOVERRIDE void AddTestItem(int iCount);
	
	//VOVERRIDE VUINodeTextLabel* GetTextLabel() { return static_cast<VUINodeTextLabel*>( m_pUIControlInstance ); }	
  };
}
