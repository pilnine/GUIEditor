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
  public ref class UIEngineInstancePushButton : public UIEngineInstanceShape2D
  {
  public:     

    UIEngineInstancePushButton();

	VOVERRIDE void Initialize() override; 
	VOVERRIDE void SetClass( String^ className );
	VOVERRIDE void ReplaceButtonControl( String^ className );
  };
}
