
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

namespace UIEditorPlugin
{   
    [Serializable]
    public class UIShapeDraggingButton : UIShapeBase
    {
        public UIShapeDraggingButton(string name)
            : base(name)
        {
            IconIndex = UIShapeBase.GetIconIndex("UIEditorPlugin.Icons.Button.png");

            UIProperties = UIManager.CreateCollection(this, "VUINodeDraggingButton");

            SizeX = 200;
            SizeY = 150;

            Order = 4; // Base render order
        }


        protected const string CAT_BUTTON = "Button Property";
        protected const int CATORDER_BUTTON = CATORDER_SHAPE2D_ETC + 1;
        public new const int LAST_CATEGORY_ORDER_ID = CATORDER_BUTTON;


        #region ISerializable members
        /// <summary>
        /// Called when deserializing
        /// </summary>
        /// <param name="info"></param>
        /// <param name="context"></param>
        protected UIShapeDraggingButton(SerializationInfo info, StreamingContext context)
            : base(info, context)
        {
          
     
        }


        public override void GetObjectData(SerializationInfo info, StreamingContext context)
        {
            base.GetObjectData(info, context);
        }

        #endregion



        private bool bActive = false;
        //PropertyTextState _textProperty = new PropertyTextState();
        //PropertyImageState _imageProperty = new PropertyImageState();
     
        /// <summary>
        /// Pseudo property that activates the effect
        /// </summary>
        [SortedCategory(CAT_BUTTON, CATORDER_BUTTON), PropertyOrder(3)]
        [Description("Activates or deactivates the postprocessing effect. There is at most one effect active at a time.")]
        public bool Active
        {
            get
            {
                return bActive;
            }
            set
            {
                bActive = value;
                //EditorManager.ActiveView.UpdateView(false);
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

        public UIEngineInstanceShape2D UIPushButtonInstance { get { return (UIEngineInstanceShape2D)_engineInstance; } }

        public override void CreateEngineInstance(bool bCreateChildren)
        {
            _engineInstance = new UIEngineInstanceShape2D("VUINodeDraggingButton");
            SetEngineInstanceBaseProperties(); // sets the position etc.

            if (Parent is UIShapeDialog)
            {
                ((UIShapeDialog)Parent).AddUIControlInstance(this);
            }

            UpdateProperties();
        }

        public override void RemoveEngineInstance(bool bRemoveChildren)
        {
            IEngineShapeInstance keepAlive = _engineInstance;
           
            if (Parent is UIShapeDialog)
            {
                ((UIShapeDialog)Parent).RemoveUIControlInstance(this);
            }

            _engineInstance = null;

            base.RemoveEngineInstance(bRemoveChildren);

         //   _engineInstance = keepAlive;
        }
    }

    /// <summary>
    /// Creator class. An instance of the creator is registerd in the plugin init function. Thus the creator shows
    /// up in the "Create" menu of the editor
    /// </summary>
    class UIShapeDraggingButtonCreator : CSharpFramework.IShapeCreatorPlugin
    {
        /// <summary>
        /// Constructor
        /// </summary>
        public UIShapeDraggingButtonCreator()
        {
            IconIndex = UIShapeBase.GetIconIndex("UIEditorPlugin.Icons.Button.png");
        }

        /// <summary>
        /// Get the name of the plugin, for instance the shape name. This name apears in the "create" menu
        /// </summary>
        /// <returns>creator name</returns>
        public override string GetPluginName()
        {
            return "DraggingButton";
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
            UIShapeDraggingButton shape = new UIShapeDraggingButton("DraggingButton");
            //   shape.Position = EditorManager.Scene.CurrentShapeSpawnPosition;
            return shape;
        }
    }
}
