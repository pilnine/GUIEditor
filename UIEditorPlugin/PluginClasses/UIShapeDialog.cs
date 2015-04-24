
using System;
using System.Windows.Forms;
using System.Drawing;
using CSharpFramework;
using CSharpFramework.Math;
using CSharpFramework.Shapes;
using CSharpFramework.Actions;
using CSharpFramework.PropertyEditors;
using CSharpFramework.Helper;
using CSharpFramework.Serialization;
using System.Drawing.Design;
using System.Collections.Generic;
using UIEditorManaged;
using System.Runtime.Serialization;
using ManagedFramework;
using System.IO;
using System.ComponentModel;
using System.Xml;
using System.Reflection;

using System.Xml.Serialization;
using System.Collections;
using CSharpFramework.Dialogs;
using CSharpFramework.Scene;

using CSharpFramework.DynamicProperties;

namespace UIEditorPlugin
{
    /// <summary>
    /// DialogShape : This is the class that represents the shape in the editor. It has an engine instance that handles the
    /// native code. The engine instance code in located in the NodeManaged project (managed C++ class library)
    /// </summary>
    [Serializable]
    public class UIShapeDialog : UIShapeBase
    {
        /// <summary>
        /// The constructor of the node shape, just takes the node name
        /// </summary>
        /// <param name="name">Name of the shape in the shape tree</param>
        public UIShapeDialog(string name)
            : base(name)
        {
            UIProperties = UIManager.CreateCollection(this, "VUINodeDialog");

            SizeX = 1920;
            SizeY = 1080;  
        }

        public override int GetIconIndex()
        {
            return UIShapeBase.GetIconIndex("UIEditorPlugin.Icons.Dialog.png");
        }

        protected const string CAT_DIALOG = "DIALOG Property";
        protected const int CATORDER_DIALOG = CATORDER_SHAPE2D_ETC + 1;
 
        public new const int LAST_CATEGORY_ORDER_ID = CATORDER_DIALOG;

        #region ISerializable members
        /// <summary>
        /// Called when deserializing
        /// </summary>
        /// <param name="info"></param>
        /// <param name="context"></param>
        protected UIShapeDialog(SerializationInfo info, StreamingContext context)
            : base(info, context)
        {
            if (SerializationHelper.HasElement(info, "ShowDialog"))
                m_bShowDialog = info.GetBoolean("ShowDialog");
            if (SerializationHelper.HasElement(info, "Filename"))
                _filename = info.GetString("Filename");
        }

        public override void GetObjectData(SerializationInfo info, StreamingContext context)
        {
            base.GetObjectData(info, context);

            info.AddValue("ShowDialog", m_bShowDialog);
            info.AddValue("Filename", _filename);      
        }

     

        #endregion


        #region Properties

        bool m_bShowDialog = true;
        bool m_bFullScreen = false;
        string _filename = "";

        [SortedCategory(CAT_DIALOG, CATORDER_DIALOG), PropertyOrder(2)]
        public bool FullScreen
        {
            get { return m_bFullScreen; }
            set
            {
                if (m_bFullScreen == value) return;
                m_bFullScreen = value;

                UIDialogInstance.SetFullScreen(m_bFullScreen);
                UpdateLayout();

            }
        }

        /// <summary>
        /// Dialog Properties
        /// </summary>     

        [SortedCategory(CAT_DIALOG, CATORDER_DIALOG), PropertyOrder(1)]
        public bool ShowDialog
        {
            get { return m_bShowDialog; }
            set
            {
                if (m_bShowDialog == value) return;

                m_bShowDialog = value;
                UIDialogInstance.SetVisibleDialog(m_bShowDialog);               
            }
        }


        [SortedCategory(CAT_DIALOG, CATORDER_DIALOG), PropertyOrder(24)]
        [EditorAttribute(typeof(FilenameEditor), typeof(UITypeEditor)),
        RefreshProperties(System.ComponentModel.RefreshProperties.All),
        FileDialogFilter(new string[] { ".xml" })]
        public string Filename
        {
            get
            {
                return _filename;
            }
            set
            {
                if (_filename == value) return;
                _filename = value;
            }
        }

        #endregion // Properties

        /// <summary>
        /// Called when exporting the scene to engine archive. base implemenation calls function on engine object which in turn
        /// serializes itself
        /// </summary>
        /// <returns></returns>
        public override bool OnExport(CSharpFramework.Scene.SceneExportInfo info)
        {
            if ( Filename != "" )
            {
                string absoluteFileName = EditorManager.Project.MakeAbsolute( Filename );

                if (UIDialogInstance.BuildToXML(absoluteFileName) == false)
                {
                    EditorManager.ShowMessageBox("Cannot export dialog file");
                }
            }

            return base.OnExport(info);         
        }


        public override void CreateEngineInstance(bool bCreateChildren)
        {           
         //   base.CreateEngineInstance(bCreateChildren);
            _engineInstance = new UIEngineInstanceDialog();
            //PropertyString
            UIDialogInstance.CreateBasicDialogInstance("VUINodeDialog", this.ID, null);
            SetEngineInstanceBaseProperties(); // sets the position etc.       
            UpdateProperties();

            if (bCreateChildren)
            {
                foreach(ShapeBase shape in ChildCollection )
                {
                    shape.CreateEngineInstance(false);
                }
            }
            
            UIDialogInstance.ShowDialog(true);
            UIDialogInstance.SetVisibleDialog(m_bShowDialog);
        }


        /// <summary>
        /// Set all properties on the engine instance. The base implementation already sets position and orientation,
        /// so there is nothing else to do here
        /// </summary>
        public override void SetEngineInstanceBaseProperties()
        {
            base.SetEngineInstanceBaseProperties();

            if (!UIManager.Exists("VUINodeDialog"))
            {
                DynamicPropertyCollectionType newType = UIManager.CreateNewType("VUINodeDialog", DynamicPropertyCollectionType.DynamicPropertyCollectionFlags_e.None);
                UIManager.AddCollectionType(newType);
            } 
        }


     

        public override void GetRelevantProperties(List<PropertyInfo> targetList, PropertyRelevantQueryReason_e context)
        {
            base.GetRelevantProperties(targetList, context);
            if (context == PropertyRelevantQueryReason_e.PrefabExposeParameter)
                targetList.Add(GetType().GetProperty("UIProperties"));
        }

        public override void OnPostPrefabCreation(ShapeCollection allRootShapes, PrefabDesc prefab)
        {
            base.OnPostPrefabCreation(allRootShapes, prefab);
            if (_uiProperties != null)
            {        
                _uiProperties.OnPostPrefabCreation(allRootShapes, prefab);
            }
        }

        public UIEngineInstanceDialog UIDialogInstance { get { return _engineInstance as UIEngineInstanceDialog; } }

        public override void RenderShape(VisionViewBase view, ShapeRenderMode mode)
        {
            base.RenderShape(view, mode);

            //UIDialogInstance.RenderShape(view, mode);
            //EngineNode.RenderShape(c);
            //base.RenderShape(view, mode);
        }

        public override void OnAddedToScene()
        {
            base.OnAddedToScene();      
//             ModelGroup.OnModelGroupFileSaved += new EventHandler(ModelGroup_OnModelGroupFileSaved);
//             IScene.EngineInstancesChanged += new EngineInstancesChangedEventHandler(IScene_EngineInstancesChanged);
//             AddHint(HintFlags_e.OnlyUniformScale);
//             AddHint(HintFlags_e.NoScalingForLocalBoundingBox);
        }

        public override void OnRemoveFromScene()
        {
//             IScene.EngineInstancesChanged -= new EngineInstancesChangedEventHandler(IScene_EngineInstancesChanged);
//             ModelGroup.OnModelGroupFileSaved -= new EventHandler(ModelGroup_OnModelGroupFileSaved);
// 
//             // finally delete our engine instance here
//             if (this._engineInstance != null)
//             {
//                 // Important: We must call the base implementation here because the overload in this class prevents the deletion
//                 base.RemoveEngineInstance(false);
//             }

            base.OnRemoveFromScene();
        }


        public override void AddChild(ShapeBase child)
        {
            base.AddChild(child);

           if (child.Parent == this && child._engineInstance != null)
                AddUIControlInstance(child);
     //       UIDialogInstance.OnAttachToParent( child._engineInstance );
        }

        public void AddUIControlInstance(  ShapeBase child )
        {
            if (UIDialogInstance != null)
            {
                UIDialogInstance.AddControl(child._engineInstance);

            }
        }

        public void RemoveUIControlInstance(ShapeBase child)
        {
            if (UIDialogInstance != null)
                UIDialogInstance.RemoveControl(child._engineInstance);
        }

        public override void OnChildAdded(ShapeBase childShape, int iDepth)
         {
             base.OnChildAdded(childShape, iDepth);
         }
        //OnChildAdded(ShapeBase childShape, int iDepth);

        public override void OnSelected()
        {
            UIEditorMouseContext.UIEditorContext.SelectUIShape(this);
            UIEditorMouseContext.UIEditorContext.SetUIEditorMode(true);

            base.OnSelected();
        }

        public override void OnUnSelected()
        {
            UIEditorMouseContext.UIEditorContext.UnSelectUIShape(this);
            UIEditorMouseContext.UIEditorContext.SetUIEditorMode(false);

            base.OnUnSelected();
        }


        /// <summary>
        /// Remove shape from child collection
        /// </summary>
        /// <param name="child">child to remove</param>
        public override void RemoveChild(ShapeBase child)
        {
            base.RemoveChild(child);

            RemoveUIControlInstance(child);
        }

        /// <summary>
        /// Calls SetVisible(FinalVisibleState) on the engine instanceof this shape and all children
        /// </summary>
        public override void UpdateVisibleStateRecursive()
        {
            if (HasEngineInstance())
            {
              //  _engineInstance.SetVisible(FinalVisibleState);
                UIDialogInstance.SetVisibleDialog(FinalVisibleState);
            }
            
        }




        #region relevant Operation

        /// <summary>
        /// Return relevant operations for heightfield shape
        /// </summary>
        public override ArrayList RelevantOperations
        {
            get
            {
                ArrayList op = base.RelevantOperations;
                if (op == null) op = new ArrayList(4);
                op.Add(@"Load XML");
                op.Add(@"Save XML");
                op.Add(@"Rebuild UI Shape tree from XML");
                return op;
            }
        }

        public override void PerformRelevantOperation(string name, int iShapeIndex, int iShapeCount)
        {
            base.PerformRelevantOperation(name, iShapeIndex, iShapeCount);
            if (name == "Load XML")
            {
                OpenFileDlg dlg = new OpenFileDlg();
                dlg.Caption = "Opening a model group file";
                dlg.Description = "Please select a model group file and click OK to continue";
                dlg.Text = "Opening an model group file";
                dlg.InitialDirectory = EditorManager.Project.ProjectDir;
                dlg.FileName = _filename;
                dlg.Filter = new string[] { ".xml" };
                if (dlg.ShowDialog() == DialogResult.OK)
                {
                    _filename = EditorManager.Project.MakeRelative(dlg.FileName);

                    if (UIDialogInstance.BuildFromXML(_filename) == false)
                    {
                        EditorManager.ShowMessageBox("Wrong Dialog resource file.");
                        _filename = "";
                    }
                }             

                UpdateLayout();
                //UpdateShape
            }
            else if (name == "Save XML")
            {
                CreateFileDlg dlg = new CreateFileDlg();
                dlg.Caption = "Opening a GUI Dialog file";
                dlg.Description = "Please select a GUI Dialog file and click OK to continue";
                dlg.Text = "Opening a GUI Dialog file";
                dlg.InitialDirectory = EditorManager.Project.ProjectDir;
                dlg.FileName = _filename;
                dlg.Filter = new string[] { ".xml" };
                dlg.AllowOverwrite = true;
                if (dlg.ShowDialog() == DialogResult.OK)
                {
                    _filename = EditorManager.Project.MakeRelative(dlg.FileName);

                    if (UIDialogInstance.BuildToXML(dlg.FileName) == false)
                    {
                        EditorManager.ShowMessageBox("Write failed.");
                        _filename = "";
                    }
                }
             

                UpdateLayout();
            }
            else if (name == "Rebuild UI Shape tree from XML")
            {
                OpenFileDlg dlg = new OpenFileDlg();
                dlg.Caption = "Opening a model group file";
                dlg.Description = "Please select a model group file and click OK to continue";
                dlg.Text = "Opening an model group file";
                dlg.InitialDirectory = EditorManager.Project.ProjectDir;
                dlg.FileName = _filename;
                dlg.Filter = new string[] { ".xml" };
                if (dlg.ShowDialog() == DialogResult.OK)
                {
                    _filename = EditorManager.Project.MakeRelative(dlg.FileName);

                    if (UIDialogInstance.BuildFromXML(_filename) == false)
                    {
                        EditorManager.ShowMessageBox("Wrong Dialog resource file.");
                        _filename = "";
                    }
                }

                ShapeCollection Collection = ChildCollection;
                foreach (ShapeBase shape in Collection)
                {
                    RemoveChild(shape);
                }


                int iItemCount = UIDialogInstance.GetChildUIInstanceCount();

                this.Bound = UIDialogInstance.GetBound();
               
                UpdateLayout();
            }            
        }


        #endregion


    }

    /// <summary>
    /// Creator class. An instance of the creator is registerd in the plugin init function. Thus the creator shows
    /// up in the "Create" menu of the editor
    /// </summary>
    class DialogShapeCreator : CSharpFramework.IShapeCreatorPlugin
    {
        /// <summary>
        /// Constructor
        /// </summary>
        public DialogShapeCreator()
        {
            IconIndex = UIShapeBase.GetIconIndex("UIEditorPlugin.Icons.Dialog.png");
            CategoryIconIndex = IconIndex;
        }



        /// <summary>
        /// Get the name of the plugin, for instance the shape name. This name apears in the "create" menu
        /// </summary>
        /// <returns>creator name</returns>
        public override string GetPluginName()
        {
            return "Dialog";
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
            UIShapeDialog shape = new UIShapeDialog("Dialog");
            //   shape.Position = EditorManager.Scene.CurrentShapeSpawnPosition;
            return shape;
        }

    }
}
