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

  public ref class UIEngineInstanceTextLabel : public UIEngineInstanceShape2D
  {
  public:     

    UIEngineInstanceTextLabel();
	virtual ~UIEngineInstanceTextLabel();


	VOVERRIDE void Initialize() override; 	
	
//	VOVERRIDE VUINodeTextLabel* GetTextLabel() { return static_cast<VUINodeTextLabel*>( m_pUIControlInstance ); }	
  };
}
