using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using CSharpFramework;
using CSharpFramework.View;
using CSharpFramework.Math;
using CSharpFramework.Shapes;
using CSharpFramework.Actions;
using CSharpFramework.Scene;
using CSharpFramework.PropertyEditors;
using CSharpFramework.DynamicProperties;
using System.Runtime.Serialization;
using ManagedFramework;
using System.IO;
using System.ComponentModel;
using System.Drawing;
using System.Drawing.Design;
using System.Windows.Forms;
using CSharpFramework.Serialization;
using System.Collections.Specialized;

using UIEditorManaged;
using System.Xml;

namespace UIEditorPlugin
{

    /// <summary>
    /// Holds RTTI information for all engine entity classes. An instance of this class can be accessed via EditorManager.EngineManager.EntityClassManager
    /// </summary>
    public class UIClassManager : DynamicPropertyManager
    {
        public override DynamicPropertyCollectionType CreateNewType(string uniqueName, DynamicPropertyCollectionType.DynamicPropertyCollectionFlags_e flags)
        {
            return new UIPropertyCollectionType(uniqueName, flags);
        }

        bool _bActive = false;
        public virtual void Activate(bool bStatus)
        {
            if (_bActive == bStatus)
                return;
            _bActive = bStatus;
            if (_bActive)
                IScene.PropertyChanged += new CSharpFramework.PropertyChangedEventHandler(IScene_PropertyChanged);
            else
            {
                IScene.PropertyChanged -= new CSharpFramework.PropertyChangedEventHandler(IScene_PropertyChanged);              
            }
        }


        void IScene_PropertyChanged(object sender, PropertyChangedArgs e)
        {
            // since this is not only for components, but also for its base class, it should be moved to a more central place
            if (!(e._component is DynamicPropertyCollection))
                return;

            DynamicPropertyCollection comp = (DynamicPropertyCollection)e._component;
            DynamicProperty prop = comp.GetProperty(e._propertyName);
            comp.OnValueChanged(prop);
        }

    }


    /// <summary>
    /// Inherits from DynamicPropertyCollectionType to return entity specific instances
    /// </summary>
    [Serializable]
    public class UIPropertyCollectionType : DynamicPropertyCollectionType, ISerializable
    {
        public UIPropertyCollectionType(string uniqueName, DynamicPropertyCollectionFlags_e flags)
            : base(uniqueName, flags)
        {
        }

        protected UIPropertyCollectionType(SerializationInfo info, StreamingContext context)
            : base(info, context)
        {
        }

        public override DynamicPropertyCollection CreateInstance(ISceneObject forObject)
        {
            return new UIPropertyCollection(forObject, this);
        }
    }



    [Serializable]
    public class UIPropertyCollection : DynamicPropertyCollection, ISerializable
    {
        public UIPropertyCollection(ISceneObject owner, DynamicPropertyCollectionType collectionType)
            : base(owner, collectionType)
        {
        }

        protected UIPropertyCollection(SerializationInfo info, StreamingContext context)
            : base(info, context)
        {

        }

        public string AsEntityString(DynamicProperty p)
        {
            // see also EngineComponentManager::SetNativeComponentProperty:
            // note we are not checking p.PropertyType.IsEnum, which would be different
            if (p.PropertyType.NativeType.IsEnum)
            {
                int iAsInt = (int)p.Value;
                return iAsInt.ToString();
            }
            return SerializationHelper.GetEngineStringFromObject(p.Value);
        }

        public override void OnValueChanged(DynamicProperty prop)
        {
            base.OnValueChanged(prop);

            UIShapeBase entity = Owner as UIShapeBase;          

            if (entity.HasEngineInstance())
            {
                string varValue = AsEntityString(prop);
                entity.EngineInstanceUIShape.SetVariable(prop.PropertyType.Name, varValue);
                if (entity.ParentLayer != null)
                    entity.ParentLayer.Modified = true;
            }
        }
    }



    // Based on VWindowBase class
    [Serializable]
    public class UIShapeBase : ShapeBase
    {
        protected const string CAT_SHAPE2D = "UI Base";
        protected const string CAT_SHAPE2D_ETC = "ETC";

        protected const int CATORDER_SHAPE2D = Shape3D.LAST_CATEGORY_ORDER_ID + 10;
        protected const int CATORDER_SHAPE2D_ETC = Shape3D.LAST_CATEGORY_ORDER_ID + 11;

        public event CSharpFramework.PropertyChangedEventHandler PropertyChanged = null;

        public UIShapeBase(string name)
            : base(name)
        {
            
            return;
        }

        public UIShapeBase()
            :base("UI")
        {

        }


        void OnPropertyChanged(object sender, PropertyChangedArgs e)
        {
            if (EngineInstanceUIShape == null)
                return;

            if (e._propertyName == "PosX" || e._propertyName == "PosY")
            {
                EngineInstanceUIShape.SetPosition(PosX, PosY, 0.0f);
            }
            else if (e._propertyName == "SizeX" || e._propertyName == "SizeY")
            {
                EngineInstanceUIShape.SetSize(SizeX, SizeY);
            }
            else if (e._propertyName == "UIKey")
            {
                EngineInstanceUIShape.SetUIKey(UIKey);
            }
            else if (e._propertyName == "Order")
            {
                EngineInstanceUIShape.SetOrder(Order);
            }
            else 
            {
                if (e._component is bool)
                    UIEditorMessageService.GetInstance().OnPropertyChanged(e._propertyName, (bool)e._component);
                else if (e._component is int)
                    UIEditorMessageService.GetInstance().OnPropertyChanged(e._propertyName, (int)e._component);
                else if (e._component is float)
                    UIEditorMessageService.GetInstance().OnPropertyChanged(e._propertyName, (float)e._component);
                else if (e._component is string)
                    UIEditorMessageService.GetInstance().OnPropertyChanged(e._propertyName, (string)e._component);
            }
        }


        public override int GetIconIndex()
        {
            return EditorManager.GUI.ShapeTreeImages.AddBitmap(Path.Combine(EditorManager.AppDataDir, @"bitmaps\Shapes\Node.png"), Color.Magenta);
        }

        /// <summary>
        /// Get the sky class manager
        /// </summary>
        public DynamicPropertyManager uiManager;
        public virtual DynamicPropertyManager UIManager
        {
            get
            {
                if (uiManager == null)
                {
                    //UIEditorMessageService::GetUIRTTIClasses(
                    uiManager = new UIClassManager();
                    UIEditorMessageService.GetInstance().GetUIRTTIClasses(uiManager);

                    ((UIClassManager)uiManager).Activate(true);
                }

                return uiManager;
            }
        }

        #region ISerializable members
        /// <summary>
        /// Called when deserializing
        /// </summary>
        /// <param name="info"></param>
        /// <param name="context"></param>
        /// 
        private CloneableBase _oldEntityProperties;


        protected UIShapeBase(SerializationInfo info, StreamingContext context)
            : base(info, context)
        {
          

            m_PosX = info.GetSingle("PosX");
            m_PosY = info.GetSingle("PosY");
            m_SizeX = info.GetSingle("SizeX");
            m_SizeY = info.GetSingle("SizeY");
            m_Order = info.GetInt32("Order");
            m_TabOrder = info.GetInt32("TabOrder");
            m_ClientBorder = info.GetSingle("ClientBorder"); 
            m_UseCaching = info.GetBoolean("UseCaching");
            m_TooltipText = info.GetString("TooltipText");
            m_Script = info.GetString("Script");

            object entityProps = (object)info.GetValue("_uiProperties", typeof(object));
            _uiProperties = entityProps as DynamicPropertyCollection;

            if (_uiProperties == null)
            {
                _oldEntityProperties = entityProps as CloneableBase;                
                //_entityProperties = EditorManager.EngineManager.EntityClassManager.CreateCollection(this,_entityClass);
            }

            if (SerializationHelper.HasElement(info, "UIKey"))
                m_UIKey = info.GetString("UIKey");

        }

        public override void OnDeserialization()
        {
            base.OnDeserialization();

            //      _uiProperties.CopyValuesFromOldCollection(_oldEntityProperties);

            _uiProperties = UIManager.CreateMigratedCollection(_uiProperties);
            if (_uiProperties != null)
                _uiProperties.Owner = this;
        }



        public override void GetObjectData(SerializationInfo info, StreamingContext context)
        {
            base.GetObjectData(info, context);
            info.AddValue("PosX", PosX);
            info.AddValue("PosY", PosY);
            info.AddValue("SizeX", SizeX);
            info.AddValue("SizeY", SizeY);

            info.AddValue("Order", Order);
            info.AddValue("TabOrder", TabOrder);
            info.AddValue("ClientBorder", ClientBorder);
            info.AddValue("UseCaching", UseCaching);
            info.AddValue("TooltipText", TooltipText);
            info.AddValue("Script", Script);

            info.AddValue("_uiProperties", _uiProperties);
            info.AddValue("UIKey", m_UIKey);         
        }
      
        #endregion

        #region Properties

        string m_className;

        float m_PosX = 0;
        float m_PosY = 0;
        float m_SizeX = 500;
        float m_SizeY = 500;      
        int m_Order;
        int m_TabOrder;
        float m_ClientBorder;
        bool m_UseCaching;
        string m_TooltipText;
        string m_Script;
        string m_UIKey;
     
        [SortedCategory(CAT_SHAPE2D, CATORDER_SHAPE2D), PropertyOrder(1)]
        public string ClassName
        {
            get { return m_className; }
            set { m_className = value; }
        }



        [SortedCategory(CAT_SHAPE2D, CATORDER_SHAPE2D), PropertyOrder(5)]
        public float PosX
        {
            get { return m_PosX; }
            set
            {
                if (m_PosX != value)
                {
                    if (value < 0 || value > 100000)
                        return;

                    m_PosX = value;
                    if (PropertyChanged != null) PropertyChanged(this, new PropertyChangedArgs(PosX, "PosX"));
                }
            }
        }

      
        [SortedCategory(CAT_SHAPE2D, CATORDER_SHAPE2D), PropertyOrder(6)]
        public float PosY
        {
            get { return m_PosY; }
            set
            {
                if (m_PosY != value)
                {
                    if (value < 0 || value > 100000)
                        return;

                    m_PosY = value;
                    if (PropertyChanged != null) PropertyChanged(this, new PropertyChangedArgs(PosY, "PosY"));
                }
            }
        }

      
        [SortedCategory(CAT_SHAPE2D, CATORDER_SHAPE2D), PropertyOrder(7)]
        public float SizeX
        {
            get { return m_SizeX; }
            set
            {
                if (m_SizeX != value)
                {
                    if (value < 0 || value > 100000)
                        return;

                    m_SizeX = value;
                    if (PropertyChanged != null) PropertyChanged(this, new PropertyChangedArgs(SizeX, "SizeX"));
                }
            }
        }

      
        [SortedCategory(CAT_SHAPE2D, CATORDER_SHAPE2D), PropertyOrder(8)]
        public float SizeY
        {
            get { return m_SizeY; }
            set
            {
                if (m_SizeY != value || value > 100000)
                {
                    if (value < 0)
                        return;

                    m_SizeY = value;
                    if (PropertyChanged != null) PropertyChanged(this, new PropertyChangedArgs(SizeY, "SizeY"));
                }
            }
        }

        [SortedCategory(CAT_SHAPE2D, CATORDER_SHAPE2D), PropertyOrder(9)]
        public int Order
        {
            get { return m_Order; }
            set
            {
                if (m_Order != value)
                {
                    m_Order = value;
                    if (PropertyChanged != null) PropertyChanged(this, new PropertyChangedArgs(Order, "Order"));
                }
            }
        }

        [SortedCategory(CAT_SHAPE2D, CATORDER_SHAPE2D), PropertyOrder(10)]
        public int TabOrder
        {
            get { return m_TabOrder; }
            set
            {
                if (m_TabOrder != value)
                {
                    m_TabOrder = value;
                    if (PropertyChanged != null) PropertyChanged(this, new PropertyChangedArgs(TabOrder, "TabOrder"));
                }
            }
        }

        [SortedCategory(CAT_SHAPE2D, CATORDER_SHAPE2D), PropertyOrder(11)]
        public float ClientBorder
        {
            get { return m_ClientBorder; }
            set
            {
                if (m_ClientBorder != value)
                {
                    m_ClientBorder = value;
                    if (PropertyChanged != null) PropertyChanged(this, new PropertyChangedArgs(ClientBorder, "ClientBorder"));
                }
            }
        }

        [SortedCategory(CAT_SHAPE2D, CATORDER_SHAPE2D), PropertyOrder(12)]
        public bool UseCaching
        {
            get { return m_UseCaching; }
            set
            {
                if (m_UseCaching != value)
                {
                    m_UseCaching = value;
                    if (PropertyChanged != null) PropertyChanged(this, new PropertyChangedArgs(UseCaching, "UseCaching"));
                }
            }
        }

        [SortedCategory(CAT_SHAPE2D, CATORDER_SHAPE2D), PropertyOrder(13)]
        public string TooltipText
        {
            get { return m_TooltipText; }
            set
            {
                if (m_TooltipText != value)
                {
                    m_TooltipText = value;
                    if (PropertyChanged != null) PropertyChanged(this, new PropertyChangedArgs(TooltipText, "TooltipText"));
                }
            }
        }

        [SortedCategory(CAT_SHAPE2D, CATORDER_SHAPE2D), PropertyOrder(14)]
        public string Script
        {
            get { return m_Script; }
            set
            {
                if (m_Script != value)
                {
                    m_Script = Script;
                    if (PropertyChanged != null) PropertyChanged(this, new PropertyChangedArgs(Script, "Script"));
                }
            }
        }
         
        [SortedCategory(CAT_SHAPE2D, CATORDER_SHAPE2D), PropertyOrder(15)]
        public string UIKey
        {
            get { return m_UIKey; }
            set
            {
                if (m_UIKey != value)
                {
                    m_UIKey = value;
                    if (PropertyChanged != null) PropertyChanged(this, new PropertyChangedArgs(UIKey, "UIKey"));
                }
            }
        }

        public DynamicPropertyCollection _uiProperties;

        [Browsable(true)]
        [PrefabRelevant(false)]
        [SortedCategory(CAT_SHAPE2D_ETC, CATORDER_SHAPE2D_ETC), PropertyOrder(40), TypeConverter(typeof(DynamicPropertyConverter))]      
        public DynamicPropertyCollection UIProperties
        {
            get { return _uiProperties; }
            set { _uiProperties = value; }
        }

#endregion //  Properties

        public override void CreateEngineInstance(bool bCreateChildren)
        {
            base.CreateEngineInstance(bCreateChildren);
            _engineInstance = new UIEngineInstanceShape2D();
            SetEngineInstanceBaseProperties(); // sets the position etc.
        }

        [PrefabRelevant(true)]
        [Browsable(false)]
        public string PropertyString
        {
            get
            {
                if (_uiProperties == null)
                    return null;
                return _uiProperties.GetPropertyParameterString(",");
            }

            set
            {
                if (_uiProperties != null && !string.IsNullOrEmpty(value))
                    _uiProperties.ParseParameterString(value, ",");
            }
        }

        public virtual void UpdateProperties()
        {
            StringDictionary propertyDict = _uiProperties.GetPropertyDict(SerializationHelper.ENGINEVECTOR_SEPARATOR);

            EngineInstanceUIShape.UpdateProperties(propertyDict);
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

        public UIEngineInstanceShape2D EngineInstanceUIShape { get { return (UIEngineInstanceShape2D)_engineInstance; } }

        /// <summary>
        /// Helper field to access the engine instance as casted class to perform specfic operations on it
        /// </summary>        
        [BrowsableAttribute(false)]
        public UIEngineInstanceShape2D EngineNode { get { return (UIEngineInstanceShape2D)_engineInstance; } }

        [Browsable(false)]
        public CSharpFramework.Math.Rectangle2D Bound
        {
            get 
            {
                Rectangle2D bound = new Rectangle2D();

                float fScreenPosX = PosX;
                float fScreenPosY = PosY;

                if (Parent != null && Parent is UIShapeBase)
                {
                    fScreenPosX += ((UIShapeBase)Parent).PosX;
                    fScreenPosY += ((UIShapeBase)Parent).PosY;
                }

                bound.Add(fScreenPosX, fScreenPosY);
                bound.Add(fScreenPosX + SizeX, fScreenPosY + SizeY);                 
                return bound;
            }
            set
            {
                value.Validate();

                PosX = value.X1;
                PosY = value.Y1;

                SizeX = value.X2 - value.X1;
                SizeY = value.Y2 - value.Y1;
            }
        }

        [Browsable(false)]
        public CSharpFramework.Math.Rectangle2D ScreenBound
        {
            get
            {
                float fScaleX = UIEditorMessageService.GetInstance().GetResolutionScaleX();
                float fScaleY = UIEditorMessageService.GetInstance().GetResolutionScaleY();

                Rectangle2D ScreenBound = new Rectangle2D();

                float fScreenPosX = PosX;
                float fScreenPosY = PosY;

                if (Parent != null && Parent is UIShapeBase)
                {
                    fScreenPosX += ((UIShapeBase)Parent).PosX;
                    fScreenPosY += ((UIShapeBase)Parent).PosY;
                }

                ScreenBound.Add(fScreenPosX * fScaleX, fScreenPosY * fScaleY);
                ScreenBound.Add((fScreenPosX + SizeX) * fScaleX, (fScreenPosY + SizeY) * fScaleY);

                return ScreenBound;
            }
            set
            {
                float fScaleX = 1.0f / UIEditorMessageService.GetInstance().GetResolutionScaleX();
                float fScaleY = 1.0f / UIEditorMessageService.GetInstance().GetResolutionScaleY();

                float fScreenPosX = value.X1;
                float fScreenPosY = value.Y1;

                if (Parent != null && Parent is UIShapeBase)
                {
                    fScreenPosX -= ((UIShapeBase)Parent).PosX;
                    fScreenPosY -= ((UIShapeBase)Parent).PosY;
                }



                PosX = fScreenPosX *fScaleX;
                PosY = fScreenPosY *fScaleY;

                SizeX = (value.X2 - value.X1) * fScaleX;
                SizeY = (value.Y2 - value.Y1) * fScaleY;
            }
        }

        /// <summary>
        /// overridden clone function
        /// </summary>
        /// <returns></returns>
        public override ShapeBase Clone()
        {
            // perform deep copy of the dynamic properties
            UIShapeBase clonedShape = base.Clone() as UIShapeBase;
            clonedShape.UIProperties = (DynamicPropertyCollection)UIProperties.Clone();
            clonedShape.UIProperties.Owner = clonedShape;
//             clonedShape._snappoints = StaticMeshShape.SNAPPOINTLIST_CACHE_INVALID; // force update next time;
// 
//             clonedShape._hotSpotLightGridOfs = null;
//             clonedShape._cachedBoneList = null; // invalidate cache (and re-cache via children)
//             clonedShape._copySource = this;
//             while (clonedShape._copySource._copySource != null)
//                 clonedShape._copySource = clonedShape._copySource._copySource;
//             ValidateCopySource();

            return clonedShape;
        }

        public Rectangle2D GetRectangle()
        {
            Rectangle2D rectangle = new Rectangle2D(PosX , PosY , PosX + SizeX , PosY + SizeY );

            return rectangle;
        }

        // for UI Editing mode
        bool _selectedInUIEditor = false;
        [Browsable(false)]
        public bool SelectedInUIEditor
        {
            get { return _selectedInUIEditor; }
            set { _selectedInUIEditor = value; }
        }
        
        /// <summary>
        /// Overridden render function: Let the engine instance render itself and render a box
        /// </summary>
        /// <param name="view"></param>
        /// <param name="mode"></param>
        public override void RenderShape(VisionViewBase view, ShapeRenderMode mode)
        {
        //    EngineNode.RenderShape(view, mode);
            base.RenderShape(view, mode);

            if (this.Selected == true)
            {
                uint iColor = VisionColors.RGBA(0, 0, 255, 255);

                float fScaleX = UIEditorMessageService.GetInstance().GetResolutionScaleX();
                float fScaleY = UIEditorMessageService.GetInstance().GetResolutionScaleY();

                float fScreenPosX = PosX;
                float fScreenPosY = PosY;

                if (Parent != null && Parent is UIShapeBase)
                {
                    fScreenPosX += ((UIShapeBase)Parent).PosX;
                    fScreenPosY += ((UIShapeBase)Parent).PosY;
                }

                view.RenderRectangle2D(fScreenPosX * fScaleX, fScreenPosY * fScaleY, (fScreenPosX + SizeX) * fScaleX, (fScreenPosY + SizeY) * fScaleY, iColor, 1.0f);                
            }           
        }


        public override void OnAddedToScene()
        {
            base.OnAddedToScene();

     //       IconIndex = EditorManager.GUI.ShapeTreeImages.AddBitmap(Path.Combine(EditorManager.AppDataDir, @"bitmaps\Shapes\Node.png"), Color.Magenta);
            this.PropertyChanged += new CSharpFramework.PropertyChangedEventHandler(OnPropertyChanged);

            //   EditorScene.ShapeChanged += new ShapeChangedEventHandler(visionView.OnShapeChanged);
            EditorManager.ShapeSelectionChanged += new ShapeSelectionChangedEventHandler(UIEditor_ShapeSelectionChanged);           
        }


        public override void OnRemoveFromScene()
        {
            this.PropertyChanged -= new CSharpFramework.PropertyChangedEventHandler(OnPropertyChanged);
            EditorManager.ShapeSelectionChanged -= new ShapeSelectionChangedEventHandler(UIEditor_ShapeSelectionChanged);

            // finally delete our engine instance here
            if (this._engineInstance != null)
            {
                // Important: We must call the base implementation here because the overload in this class prevents the deletion
                base.RemoveEngineInstance(false);
            }

            UIEditorMouseContext.UIEditorContext.UnSelectUIShape(this);
            UIEditorMouseContext.UIEditorContext.SetUIEditorMode(false);
            base.OnRemoveFromScene();
        }



        /// <summary>
        /// Set all properties on the engine instance. The base implementation already sets position and orientation,
        /// so there is nothing else to do here
        /// </summary>
        public override void SetEngineInstanceBaseProperties()
        {
            base.SetEngineInstanceBaseProperties();                        
           
            UpdateLayout();
            EngineInstanceUIShape.SetUIKey(m_UIKey);
            EngineInstanceUIShape.SetOrder(Order);
        //    EngineInstanceUIShape.UpdateEntityProperties();
        }

        public void UpdateLayout()
        {
            EngineInstanceUIShape.SetPosition(PosX, PosY, 0.0f);
            EngineInstanceUIShape.SetSize(SizeX, SizeY);
        }

        public void LoadLayout()
        {
            EngineInstanceUIShape.SetPosition(PosX, PosY, 0.0f);
            EngineInstanceUIShape.SetSize(SizeX, SizeY);
        }


        public void UIEditor_ShapeSelectionChanged(object sender, ShapeSelectionChangedArgs e)
        {
            if (e.NewSelection.Contains(this) == true)
            {
                base.OnSelected();
                OnSelected();
            }
            else if (this.Selected == true)
            {
                base.OnUnSelected();
                OnUnSelected();
            }
        }

        public override void OnSelected()
        {
            UIEditorMouseContext.UIEditorContext.SelectUIShape(this);
            UIEditorMouseContext.UIEditorContext.SetUIEditorMode(true);


        }

        public override void OnUnSelected()
        {
            UIEditorMouseContext.UIEditorContext.UnSelectUIShape(this);
            UIEditorMouseContext.UIEditorContext.SetUIEditorMode(false);
        }

        


        /// <summary>
        /// Called during export to collect native plugin information. In this case, return the global instance that applies for all shpes in this plugin
        /// </summary>
        /// <returns></returns>
        public override CSharpFramework.Serialization.EditorPluginInfo GetPluginInformation()
        {
            return EditorPlugin.EDITOR_PLUGIN_INFO;
        }

        public static int GetIconIndex(string iconName)
        {
            Stream iconStream = System.Reflection.Assembly.GetExecutingAssembly().GetManifestResourceStream(iconName);

            if (iconStream != null)
            {
                Bitmap image = new Bitmap(iconStream);
                return EditorManager.GUI.ShapeTreeImages.AddBitmap(image, iconName, Color.Magenta);
            }

            return EditorManager.GUI.ShapeTreeImages.AddBitmap(Path.Combine(EditorManager.AppDataDir, @"bitmaps\Shapes\Node.bmp"), Color.Magenta);
        }
    }
}
