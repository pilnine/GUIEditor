
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
using CSharpFramework.UndoRedo;
using CSharpFramework.Serialization;

namespace UIEditorPlugin
{
    /// <summary>
    /// ImageShape : This is the class that represents the shape in the editor. It has an engine instance that handles the
    /// native code. The engine instance code in located in the NodeManaged project (managed C++ class library)
    /// </summary>
    [Serializable]
    public class UIShapeListControl : UIShapeBase //, ImageState
    {
        /// <summary>
        /// The constructor of the node shape, just takes the node name
        /// </summary>
        /// <param name="name">Name of the shape in the shape tree</param>
        public UIShapeListControl(string name)
            : base(name)
        {
            IconIndex = UIShapeBase.GetIconIndex("UIEditorPlugin.Icons.Text.png");
            UIProperties = UIManager.CreateCollection(this, "VUINodeList");
        }

        protected const string CAT_LIST = "List Property";
        protected const int CATORDER_LIST = CATORDER_SHAPE2D_ETC + 1;
        public new const int LAST_CATEGORY_ORDER_ID = CATORDER_LIST;

        #region ISerializable members
        /// <summary>
        /// Called when deserializing
        /// </summary>
        /// <param name="info"></param>
        /// <param name="context"></param>
        protected UIShapeListControl(SerializationInfo info, StreamingContext context)
            : base(info, context)
        {
            if (SerializationHelper.HasElement(info, "ItemCount"))
                _itemCount = info.GetInt32("ItemCount");
     
        }


        public override void GetObjectData(SerializationInfo info, StreamingContext context)
        {
            base.GetObjectData(info, context);

            info.AddValue("ItemCount", _itemCount);
        }

        #endregion

        int _itemCount;

        [SortedCategory(CAT_LIST, CATORDER_LIST), PropertyOrder(1)]
        public int ItemCount
        {
            get { return _itemCount; }
            set
            {
                if (_itemCount == value) return;

                _itemCount = value;

                if ( UIListInstance != null )
                    UIListInstance.AddTestItem(_itemCount);
            }
        }



        /// <summary>
        /// Called when exporting the scene to engine archive. base implemenation calls function on engine object which in turn
        /// serializes itself
        /// </summary>
        /// <returns></returns>
        public override bool OnExport(CSharpFramework.Scene.SceneExportInfo info)
        {
            return base.OnExport(info);
        }

        public UIEngineInstanceListControl UIListInstance { get { return (UIEngineInstanceListControl)_engineInstance; } }


        public override void CreateEngineInstance(bool bCreateChildren)
        {
            //base.CreateEngineInstance(bCreateChildren);
            if (_engineInstance == null)
            {
                _engineInstance = new UIEngineInstanceListControl();
                SetEngineInstanceBaseProperties(); // sets the position etc.

                if ( Parent is UIShapeDialog )
                {
                    ((UIShapeDialog)Parent).AddUIControlInstance(this);
                }

                UpdateProperties();
            }
        }

        public override void UpdateProperties()
        {
            base.UpdateProperties();

            UIListInstance.AddTestItem(_itemCount);
        }

        public void AddUIControlInstance(ShapeBase child)
        {
            if (UIListInstance != null)
            {
                if (child is UIShapeSliderControl)
                {
                    // 
                 
                    UIListInstance.AddScrollbarControl(child._engineInstance);
                }

            }
        }
    }

    /// <summary>
    /// Creator class. An instance of the creator is registerd in the plugin init function. Thus the creator shows
    /// up in the "Create" menu of the editor
    /// </summary>
    class ListControlShapeCreator : CSharpFramework.IShapeCreatorPlugin
    {
        /// <summary>
        /// Constructor
        /// </summary>
        public ListControlShapeCreator()
        {
            IconIndex = UIShapeBase.GetIconIndex("UIEditorPlugin.Icons.Text.png");
        }

        /// <summary>
        /// Get the name of the plugin, for instance the shape name. This name apears in the "create" menu
        /// </summary>
        /// <returns>creator name</returns>
        public override string GetPluginName()
        {
            return "ListControl";
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
            return "Creates a node instance that can be linked to other nodes.\n" +
              "This shape is just a sample for custom vForge plugins.";
        }

        public override ShapeBase CreateShapeInstance()
        {
            UIShapeListControl shape = new UIShapeListControl("ListControl");
            //   shape.Position = EditorManager.Scene.CurrentShapeSpawnPosition;
            return shape;
        }
    }
}
