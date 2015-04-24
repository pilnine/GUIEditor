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
using System.Windows.Forms;
//using UIEditorPlugin.Panels;
using CSharpFramework;
using CSharpFramework.View;
using CSharpFramework.Clipboard;
using CSharpFramework.Docking;
using CSharpFramework.Math;
using CSharpFramework.Contexts;
using CSharpFramework.Serialization;




namespace UIEditorPlugin
{
  public class EditorPlugin : IEditorPluginModule
  {
    /// <summary>
    /// Default constructor for the entity panel
    /// </summary>
    public EditorPlugin()
    {
      _name = "UIEditor";
      _version = 1;
    }

    //private DragAndDropPanel _DragAndDropPanel = null;


    IShapeCreatorPlugin[] creators;

    public static EditorPluginInfo EDITOR_PLUGIN_INFO = new EditorPluginInfo();

    public override bool InitPluginModule()
    {
   //   _DragAndDropPanel = new DragAndDropPanel(EditorManager.ApplicationLayout.DockingArea);
      //_DragAndDropPanel.ShowDockable();


        UIEditorManaged.ManagedModule.InitManagedModule();

        //EditorManager.GUI.
     // Editor.EnginePanel.View.DragDrop += itemDrop;
      //ManagedFramework.View.DragDrop += itemDrop;

      //EditorManager.MainForm.DragDrop += itemDrop;

      VisionViewBase.DropObjectCreator += new VisionViewBase.DropObjectCreatorDelegate(itemDrop);

      //VisionViewBase.MouseDown += new System.Windows.Forms.MouseEventHandler(this.VisionViewBase_MouseDown);
     
      //////////////////////////////////////////////////////////////////////////
    
      EDITOR_PLUGIN_INFO.NativePluginNames = new string[] { "UIEnginePlugin" };

      // we only have one type of shape in this plugin
      creators = new IShapeCreatorPlugin[]
                 {
                     new DialogShapeCreator(),
                     new PushButtonShapeCreator(),
                     new ImageShapeCreator(),
                     new GUIContextShapeCreator(),
                     new TextLabelShapeCreator(),
                     new CheckBoxShapeCreator(),
                     new SliderControlShapeCreator(),
                     new ListControlShapeCreator(),
                     new TextInputShapeCreator(),
                     new PushColorButtonShapeCreator(),
                     new PushStyleButtonShapeCreator(),
                     new ModelPreviewShapeCreator(),
                     new PieGraphShapeCreator(),        
                     new RotatorShapeCreator(),        
                     new NumericUpDownShapeCreator(),       
                     new UIShapeDraggingButtonCreator(),
                     
                 };

      // add them to the editor
      foreach (IShapeCreatorPlugin plugin in creators)
          EditorManager.ShapeCreatorPlugins.Add(plugin);
      /*
            // register tests (optional)
            TestSuite suite = new  TestSuite("Node Shape Plugin Tests");
            suite.Add( new TestFixture(typeof(Tests.EntityShapeTest)) );
            TestManager.AddTestSuite( suite );
      */
      return true;     
    }

    private void itemDrop(object creator, System.Windows.Forms.DragEventArgs e, ref bool bProcessed)
    {
        // 


        return;
    }

    public override bool DeInitPluginModule()
    {
        UIEditorManaged.ManagedModule.DeInitManagedModule();


      //Deregister the event handlers the panel uses.
//       _DragAndDropPanel.DeregisterEventHandler();
//       _DragAndDropPanel.Close();
//       _DragAndDropPanel.Dispose();
//       _DragAndDropPanel = null;
      return true;
    }

    /// <summary>
    /// Overridable that determines whether this is a standard plugin that is always loaded and initialized
    /// </summary>
    public override bool IsStandardPlugin
    {
      get
      {
        return true;
      }
    }
  }
}

