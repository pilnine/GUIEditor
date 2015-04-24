/*
 *
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Product and Trade Secret source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2013 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 *
 */

using System;
using System.Windows.Forms;
using System.Drawing;
using CSharpFramework;
using CSharpFramework.Math;
using CSharpFramework.Shapes;
using CSharpFramework.Actions;
using UIEditorManaged;
using System.Runtime.Serialization;
using ManagedFramework;
using System.IO;
using System.ComponentModel;
using System.Collections;
using CSharpFramework.DynamicProperties;
using CSharpFramework.Serialization;

namespace UIEditorPlugin
{
    public class GUIContextClassConverter : StringConverter
    {
        /// <summary>
        /// Overridden StringConverter function: We provide standard values
        /// </summary>
        /// <param name="context">The descriptor context</param>
        /// <returns>In our case : true</returns>
        public override bool GetStandardValuesSupported(ITypeDescriptorContext context)
        {
            return true;
        }

        /// <summary>
        /// Overridden StringConverter function: Returns the standard values that the dropdown should have
        /// </summary>
        /// <param name="context">The descriptor context</param>
        /// <returns>Collection of standard values</returns>
        public override StandardValuesCollection GetStandardValues(ITypeDescriptorContext context)
        {
            ArrayList className = new ArrayList();
            UIEditorMessageService.GetInstance().GetClassNames("VGUIMainContext", className);
            return new StandardValuesCollection(className);
        }

        /// <summary>
        /// Overridden StringConverter function: 
        /// We also allow non-standard values (i.e. when an entity class is missing/unknown)
        /// </summary>
        /// <param name="context">The descriptor context</param>
        /// <returns>In our case : false</returns>
        public override bool GetStandardValuesExclusive(ITypeDescriptorContext context)
        {
            return false;
        }
    }


  #region class GUIContextShape
	/// <summary>
	/// GUIContextShape : This is the class that represents the shape in the editor. It has an engine instance that handles the
	/// native code. The engine instance code in located in the NodeManaged project (managed C++ class library)
	/// </summary>
  [Serializable]
    public class GUIContextShape : ShapeBase
	{

    #region Constructor

    /// <summary>
    /// The constructor of the node shape, just takes the node name
    /// </summary>
    /// <param name="name">Name of the shape in the shape tree</param>
	public GUIContextShape(string name) : base(name)
	{
        GUIContextProperties = UIManager.CreateCollection(this, ContextClass.ToString());

        //GUIContextInstance
    }

    #endregion

    #region Icon

    /// <summary>
    /// Get an index once (executed the first time an instance of this shape is created)
    /// </summary>
    static int iIconIndex = EditorManager.GUI.ShapeTreeImages.AddBitmap(Path.Combine(EditorManager.AppDataDir, @"bitmaps\Shapes\Node.bmp"), Color.Magenta);

    /// <summary>
    /// Return the icon index in the image list for the shape tree view. For this, use a static variable for this class.
    /// </summary>
    public override int GetIconIndex()
    {
      return iIconIndex;
    }

    /// <summary>
    /// Gets the icon index for selected icons in the tree view. Simply pass the icon index
    /// </summary>
    /// <returns></returns>
    public override int GetSelectedIconIndex()
    {
      return iIconIndex;
    }

    protected const string CAT_GUI_CONTEXT = "UI Base";
    protected const int CATORDER_GUI_CONTEXT = Shape3D.LAST_CATEGORY_ORDER_ID + 10;   
    #endregion


    #region Properties

 

    public enum GUIContextClass_e
    {
        VDefaultGUIContext,
        VDynamicGUIContext,
    };

    GUIContextClass_e m_contextClass = GUIContextClass_e.VDefaultGUIContext;

    /// <summary>
    /// get or set entity class name
    /// </summary>
    [SortedCategory(CAT_GUI_CONTEXT, CATORDER_GUI_CONTEXT), PropertyOrder(5)]
    [Description("The engine type of the entity")]  //Causes a grid refresh after the class changes because we change the entity properties
    public GUIContextClass_e ContextClass
    {
        get { return m_contextClass; }
        set
        {
            if (m_contextClass != value)
            {
                m_contextClass = value;
                UpdateGUIContext();

                if (HasEngineInstance())
                {
                    ReCreateEngineInstance(false);
                }
            }
        }
    }

    int _resolutionX = 1920;
    int _resolutionY = 1080;

    [SortedCategory(CAT_GUI_CONTEXT, CATORDER_GUI_CONTEXT), PropertyOrder(1)]
    [Description("The engine type of the entity")]  //Causes a grid refresh after the class changes because we change the entity properties
    public int ResolutionX
    {
        get { return _resolutionX; }
        set
        {
            if (_resolutionX != value)
            {
                _resolutionX = value;           
   
               
            }
        }
    }

    [SortedCategory(CAT_GUI_CONTEXT, CATORDER_GUI_CONTEXT), PropertyOrder(2)]
    [Description("The engine type of the entity")]  //Causes a grid refresh after the class changes because we change the entity properties
    public int ResolutionY
    {
        get { return _resolutionY; }
        set
        {
            if (_resolutionY != value)
            {
                _resolutionY = value;
            }
        }
    }

    float _editScale = 0.5f;

    [SortedCategory(CAT_SHAPE, CATORDER_SHAPE), PropertyOrder(3),
    Description("Gets or sets the visible status of the Screen resolution")]
    public float EditScale
    {
        get { return _editScale; }
        set
        {
            _editScale = value;

            if (HasEngineInstance())
            {
                EngineNode.SetScaling(_editScale, _editScale, 1.0f);
            }

     //      UIEditorMessageService.GetInstance().GetResolutionScaleX();
     //      UIEditorMessageService.GetInstance().GetResolutionScaleY();

        }
    }


    [SortedCategory(CAT_SHAPE, CATORDER_SHAPE), PropertyOrder(10),
    Description("Gets or sets the visible status of the Screen resolution"), DefaultValue(true)]
    public bool Visible
    {
        get { return _bVisible; }
        set
        {
            _bVisible = value;
        //    UpdateVisibleStateRecursive();
        }
    }



    public DynamicPropertyCollection _guiContextProperties;

    [Browsable(true)]  
    [SortedCategory(CAT_GUI_CONTEXT, CATORDER_GUI_CONTEXT), PropertyOrder(40), TypeConverter(typeof(DynamicPropertyConverter))]
    public DynamicPropertyCollection GUIContextProperties
    {
        get { return _guiContextProperties; }
        set { _guiContextProperties = value; }
    }


    public DynamicPropertyManager uiManager;
    public virtual DynamicPropertyManager UIManager
    {
        get
        {
            if (uiManager == null)
            {
                //UIEditorMessageService::GetUIRTTIClasses(
                uiManager = new DynamicPropertyManager();
                UIEditorMessageService.GetInstance().GetGUIContextRTTIClasses(uiManager);

            //    ((UIClassManager)uiManager).Activate(true);
            }

            return uiManager;
        }
    }

    public void UpdateGUIContext()
    {
        return;
    }


    public override void RenderShape(VisionViewBase view, ShapeRenderMode mode)
    {
        base.RenderShape(view, mode);

        if (this.Visible == true)
        {
            uint iColor = VisionColors.RGBA(0, 255, 255, 255);

            view.RenderRectangle2D(0, 0, ResolutionX * _editScale, ResolutionY * _editScale, iColor, 1.0f);
        }   

    }


    #endregion

    #region Engine Instance

    /// <summary>
    /// Function to create the engine instance of the shape. The engine instance is of type EngineInstanceNode
    /// and located in the managed code library.
    /// </summary>
    /// <param name="bCreateChildren">relevant for the base class to create instances for children</param>
    public override void CreateEngineInstance(bool bCreateChildren)
    {
      base.CreateEngineInstance (bCreateChildren);
      _engineInstance = new GUIContextInstance();
      SetEngineInstanceBaseProperties(); // sets the position etc.
    }

    /// <summary>
    /// Removes the engine instance
    /// </summary>
    /// <param name="bRemoveChildren">relevant for the base class to cleanup its children</param>
    public override void RemoveEngineInstance(bool bRemoveChildren)
    {
      base.RemoveEngineInstance(bRemoveChildren);

      // nothing else to do here (_engineInstance already destroyed in base.RemoveEngineInstance
    }

    /// <summary>
    /// Helper field to access the engine instance as casted class to perform specfic operations on it
    /// </summary>
    GUIContextInstance EngineNode { get { return (GUIContextInstance)_engineInstance; } }
    
    /// <summary>
    /// Set all properties on the engine instance. The base implementation already sets position and orientation,
    /// so there is nothing else to do here
    /// </summary>
    public override void SetEngineInstanceBaseProperties()
    {
      base.SetEngineInstanceBaseProperties ();
      // set other properties here : EngineNode.SetXYZ()

      EngineNode.SetEditViewScale(EditScale);
    }

    #endregion

    #region Serialization and Export

    /// <summary>
    /// Called when deserializing
    /// </summary>
    /// <param name="info"></param>
    /// <param name="context"></param>
    protected GUIContextShape(SerializationInfo info, StreamingContext context) : base(info, context)
    {
        if (SerializationHelper.HasElement(info, "ResolutionX"))
            _resolutionX = info.GetInt32("ResolutionX");
        if (SerializationHelper.HasElement(info, "ResolutionY"))
            _resolutionY = info.GetInt32("ResolutionY");
         if (SerializationHelper.HasElement(info, "ContextClass"))
             m_contextClass = (GUIContextClass_e)info.GetValue("ContextClass", typeof(GUIContextClass_e));
     
      //  GUIContextClass_e ContextClass

      //..nothing to do
    }

    /// <summary>
    /// Called when serializing
    /// </summary>
    /// <param name="info"></param>
    /// <param name="context"></param>
    public override void GetObjectData(System.Runtime.Serialization.SerializationInfo info, System.Runtime.Serialization.StreamingContext context)
    {
        base.GetObjectData(info, context);

        info.AddValue("ResolutionX", _resolutionX);
        info.AddValue("ResolutionY", _resolutionY);
        info.AddValue("ContextClass", m_contextClass);      
    }



    /// <summary>
    /// Called during export to collect native plugin information. In this case, return the global instance that applies for all shpes in this plugin
    /// </summary>
    /// <returns></returns>
    public override CSharpFramework.Serialization.EditorPluginInfo GetPluginInformation()
    {
      return EditorPlugin.EDITOR_PLUGIN_INFO;
    }
  
 

    /// <summary>
    /// Called when exporting the scene to engine archive. base implemenation calls function on engine object which in turn
    /// serializes itself
    /// </summary>
    /// <returns></returns>
    public override bool OnExport(CSharpFramework.Scene.SceneExportInfo info)
    {
      return base.OnExport (info);
    }

    #endregion
  }
  #endregion

  #region Shape Creator Plugin


  /// <summary>
  /// Creator class. An instance of the creator is registerd in the plugin init function. Thus the creator shows
  /// up in the "Create" menu of the editor
  /// </summary>
  class GUIContextShapeCreator : CSharpFramework.IShapeCreatorPlugin
  {

    /// <summary>
    /// Constructor
    /// </summary>
    public GUIContextShapeCreator()
    {
      IconIndex = EditorManager.GUI.ShapeTreeImages.AddBitmap(Path.Combine(EditorManager.AppDataDir, @"bitmaps\Shapes\Node.bmp"), Color.Magenta);
    }

    /// <summary>
    /// Get the name of the plugin, for instance the shape name. This name apears in the "create" menu
    /// </summary>
    /// <returns>creator name</returns>
    public override string GetPluginName()
    {
      return "GUI Context";
    }

    /// <summary>
    /// Get the plugin category name to sort the plugin name. This is useful to group creators. A null string can
    /// be returned to put the creator in the root
    /// </summary>
    /// <returns></returns>
    public override string GetPluginCategory()
    {
      return "UI";
    }

    /// <summary>
    /// Returns a short description text
    /// </summary>
    /// <returns></returns>
    public override string GetPluginDescription()
    {
      return "Creates a node instance that can be linked to other nodes.\n"+
        "This shape is just a sample for custom vForge plugins.";
    }

    public override ShapeBase CreateShapeInstance()
    {
      GUIContextShape shape = new GUIContextShape("GUIContext");    
      return shape;
    }
  
  }

  #endregion 
}
