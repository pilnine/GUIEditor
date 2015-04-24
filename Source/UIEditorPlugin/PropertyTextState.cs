using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using System.ComponentModel;
using System.Drawing.Design;

using CSharpFramework;
using CSharpFramework.Shapes;
using CSharpFramework.View;
using CSharpFramework.Scene;
using CSharpFramework.PropertyEditors;
using CSharpFramework.UndoRedo;

namespace UIEditorPlugin
{
    [DelegatePropertyChangesToParent]
    public class PropertyTextState : ICloneable
    {
        protected const string CAT_APPEARANCE = "Image Properties";
        protected const int CATORDER_APPEARANCE = Shape3D.LAST_CATEGORY_ORDER_ID + 5;

        Color _textColor = System.Drawing.Color.White;
        string _text = "<TEXT>";
        string _font = "<Default>";
        int _textSize = 10;

      
        public PropertyTextState()
        {
    
        }

        public object Clone()
        {
            PropertyTextState copy = (PropertyTextState)this.MemberwiseClone();          
            return copy;
        }

        [SortedCategory(CAT_APPEARANCE, CATORDER_APPEARANCE), Description("Text") , PropertyOrder(6)]
        public string Text
        {
            get { return _text; }
            set { _text = value; }
        }

        [SortedCategory(CAT_APPEARANCE, CATORDER_APPEARANCE), Description("Text"), PropertyOrder(7)]
        public int TextSize
        {
            get { return _textSize; }
            set { _textSize = value; }
        }


        [SortedCategory(CAT_APPEARANCE, CATORDER_APPEARANCE),Description("Font color"),EditorAttribute(typeof(StandardColorDropDownEditor),
            typeof(UITypeEditor)), PropertyOrder(9)]
        [PropertyLiveUpdate("LightColorLiveUpdate")]
        public Color TextColor
        {
            get { return _textColor; }
            set
            {
                if (_textColor == value) return;
                _textColor = value;
            }
        }

        [SortedCategory(CAT_APPEARANCE, CATORDER_APPEARANCE), Description("Font"), PropertyOrder(10)]
        public string Font
        {
            get { return _font; }
            set { _font = value; }
        }
    }
}
