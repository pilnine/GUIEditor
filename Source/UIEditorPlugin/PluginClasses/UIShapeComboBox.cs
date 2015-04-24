
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
    public class UIShapeComboBox : UIShapeBase
    {
        public UIShapeComboBox(string name)
            : base(name)
        {
            IconIndex = UIShapeBase.GetIconIndex("UIEditorPlugin.Icons.Button.png");      

            UIProperties = UIManager.CreateCollection(this, "VUINodeComboBox");

            SizeX = 400;
            SizeY = 100;

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
        protected UIShapeComboBox(SerializationInfo info, StreamingContext context)
            : base(info, context)
        {
            ControlClass = info.GetString("ControlClass");
     
        }


        public override void GetObjectData(SerializationInfo info, StreamingContext context)
        {
            base.GetObjectData(info, context);

            info.AddValue("ControlClass", ControlClass);
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

        private string controlClass = "VUINodeComboBox";
        [SortedCategory(CAT_BUTTON, CATORDER_BUTTON), PropertyOrder(4)]
        [TypeConverter(typeof(ButtonClassConverter))]
        [Description("Activates or deactivates the postprocessing effect. There is at most one effect active at a time.")]
        public string ControlClass
        {
            get { return controlClass; }
            set
            {
                if (controlClass != value)
                {
//                     if (ButtonClassConverter.IsClassName(controlClass))
//                     {
//                         ((UIShapeDialog)Parent).RemoveUIControlInstance(this);
//                         UIPushButtonInstance.SetClass(controlClass);
//                         ((UIShapeDialog)Parent).AddUIControlInstance(this);
// 
//                         controlClass = value;
//                     }
//                     else
//                     {
//                         controlClass = ButtonClassConverter.GetDefaultClassName();
//                     }
                }
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

        public UIEngineInstancePushButton UIPushButtonInstance { get { return (UIEngineInstancePushButton)_engineInstance; } }

        public override void CreateEngineInstance(bool bCreateChildren)
        {
            //   base.CreateEngineInstance(bCreateChildren);
            if (_engineInstance == null)
            {
                _engineInstance = new UIEngineInstanceShape2D("VUINodeComboBox");
                SetEngineInstanceBaseProperties(); // sets the position etc.

                if (Parent is UIShapeDialog)
                {
                    ((UIShapeDialog)Parent).AddUIControlInstance(this);
                }

                UpdateProperties();
            }
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
        }
    }

    /// <summary>
    /// Creator class. An instance of the creator is registerd in the plugin init function. Thus the creator shows
    /// up in the "Create" menu of the editor
    /// </summary>
    class ComboBoxShapeCreator : CSharpFramework.IShapeCreatorPlugin
    {
        /// <summary>
        /// Constructor
        /// </summary>
        public ComboBoxShapeCreator()
        {
            IconIndex = UIShapeBase.GetIconIndex("UIEditorPlugin.Icons.Button.png");
        }

        /// <summary>
        /// Get the name of the plugin, for instance the shape name. This name apears in the "create" menu
        /// </summary>
        /// <returns>creator name</returns>
        public override string GetPluginName()
        {
            return "ComboBox";
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
            UIShapeComboBox shape = new UIShapeComboBox("ComboBox");
            //   shape.Position = EditorManager.Scene.CurrentShapeSpawnPosition;
            return shape;
        }
    }
}
