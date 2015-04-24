using System;
using System.Collections.Generic;
using System.Text;
//using UIEditorPlugin.Panels;
using CSharpFramework;
using System.Windows.Forms;
using CSharpFramework.View;
using CSharpFramework.Clipboard;
using CSharpFramework.Docking;
using CSharpFramework.Math;
using CSharpFramework.Contexts;


namespace UIEditorPlugin
{
    public class UIEditorMouseContext : IMouseContext
    {
        public override bool OnMouseUp(MouseEventArgs e, KeyModifier eKeyMod, int iOldX, int iOldY)
        {
            return false;
        }


        public override bool OnMouseDown(MouseEventArgs e, KeyModifier eKeyMod, int iOldX, int iOldY)
        {
            return false;
        }

        /// <summary>
        /// Overridable that is called when the mouse cursor moves in the view
        /// </summary>
        /// <param name="e">Mouse event arguments directly passed through</param>
        /// <param name="eKeyMod">Key modifier bitmask (Shift, Alt, ...)</param>
        /// <param name="iOldX">Previous screen x position</param>
        /// <param name="iOldY">Previous screen y position</param>
        /// <returns>return true to redraw the view immediately</returns>
        public override bool OnMouseMove(MouseEventArgs e, KeyModifier eKeyMod, int iOldX, int iOldY)
        {
            return false;
        }

        /// <summary>
        /// Overridable that is called when the mouse wheel is used
        /// </summary>
        /// <param name="e">Mouse event arguments directly passed through</param>
        /// <param name="eKeyMod">Key modifier bitmask (Shift, Alt, ...)</param>
        /// <param name="iOldX">Previous screen x position</param>
        /// <param name="iOldY">Previous screen y position</param>
        /// <returns>return true to redraw the view immediately</returns>
        public override bool OnMouseWheel(MouseEventArgs e, KeyModifier eKeyMod, int iOldX, int iOldY)
        {
            return false;
        }

        /// <summary>
        /// Overridable that is called when modifier keys change state, e.g. Shift/Alt/Ctrl key.
        /// </summary>
        /// <param name="eOldMod">Old modifier bitmask</param>
        /// <param name="eNewMod">New (current) modifier bitmask</param>
        public override void OnKeyModifierChanged(KeyModifier eOldMod, KeyModifier eNewMod)
        {

        }
    }

}
