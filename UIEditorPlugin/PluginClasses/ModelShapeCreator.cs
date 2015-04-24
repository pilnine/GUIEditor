// TKBMS v1.0 -----------------------------------------------------
//
// PLATFORM		: ALL
// PRODUCT		: VISION
// VISIBILITY		: PUBLIC
//
// ------------------------------------------------------TKBMS v1.0

using System;
using System.Collections.Generic;
using System.Text;
using CSharpFramework;
using CSharpFramework.Shapes;
using VisionEditorPlugin.Shapes;

namespace UIEditorPlugin.Classes
{
  /// <summary>
  /// Shape creator plugin implementation to create a entity with a model via drag-and-drop
  /// </summary>
  public class ModelShapeCreator : IShapeCreatorPlugin
  {
    private string _ModelPath = "";

    public ModelShapeCreator(string ModelPath)
    {
      _ModelPath = ModelPath;
    }

    public override ShapeBase CreateShapeInstance()
    {
      string ShapeName = System.IO.Path.GetFileNameWithoutExtension(_ModelPath);
      if (ShapeName == string.Empty)
        ShapeName = "Model";

      EntityShape ES = new EntityShape(ShapeName);
      ES.ModelFile = _ModelPath;

      return ES;
    }
  }
}

