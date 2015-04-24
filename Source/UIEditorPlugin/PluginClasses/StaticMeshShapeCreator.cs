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
  /// Shape creator plugin implementation to create a static mesh via drag-and-drop
  /// </summary>
  public class StaticMeshShapeCreator : IShapeCreatorPlugin
  {
    private string _MeshPath = "";

    public StaticMeshShapeCreator(string ModelPath)
    {
      _MeshPath = ModelPath;
    }

    public override ShapeBase CreateShapeInstance()
    {
      string ShapeName = System.IO.Path.GetFileNameWithoutExtension(_MeshPath);
      if (ShapeName == string.Empty)
        ShapeName = "Model";

      StaticMeshShape SMS = new StaticMeshShape(ShapeName);
      SMS.MeshFileName = _MeshPath;

      return SMS;
    }
  }
}

