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
using CSharpFramework.Math;
using System.Runtime.Serialization;

namespace UIEditorPlugin
{
//     public enum ImageStretchMode_e
//     {
//         TEXTURE_SIZE = 0,  ///< Use actual pixel size of the texture
//         STRETCHED = 1,     ///< Stretch the image to fit into the screen rectangle
//         BORDER = 2         ///< Render texture in border mode
//     };

  

    [DelegatePropertyChangesToParent]
    public class PropertyImageState : ICloneable , ISerializable
    {
        protected const string CAT_APPEARANCE = "Image Properties";
        protected const int CATORDER_APPEARANCE = Shape3D.LAST_CATEGORY_ORDER_ID + 3;
     
//         string _texture = "Default";
//         Vector3F _uvRotationTransform = new Vector3F();
//         Vector3F _uvPostionTransform = new Vector3F();

    //    TransparencyType _transparency = TransparencyType.Opaque;
   //     ImageStretchMode_e _stretchMode = ImageStretchMode_e.TEXTURE_SIZE;

 
      

        public PropertyImageState()
        {
//             m_eStretchMode = STRETCHED;
//             m_bDefined = false;
//             m_iColor = V_RGBA_WHITE;
//             texCoord.Set(0, 0, 1, 1);
//             m_pUVTransform = m_pPosTransform = NULL;
//             m_eTranspType = VIS_TRANSP_ALPHA;
//             m_iAdditionalStateFlags = 0;    
        }
        #region ISerializable members
        protected PropertyImageState(SerializationInfo info, StreamingContext context)         
        {
            //Image = (PropertyImageState)info.GetValue("Image", typeof(PropertyImageState));     

            //..nothing to do
        }

        public void GetObjectData(SerializationInfo info, StreamingContext context)
        {
        
//             info.AddValue("PosX", PosX);
//             info.AddValue("PosY", PosY);
//             info.AddValue("SizeX", SizeX);
//             info.AddValue("SizeY", SizeY);
        }

        #endregion

        public object Clone()
        {
            PropertyImageState copy = (PropertyImageState)this.MemberwiseClone();          
            return copy;
        }     
    }
}
