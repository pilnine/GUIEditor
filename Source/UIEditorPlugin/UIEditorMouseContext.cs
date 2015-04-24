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
using CSharpFramework.Actions;
using CSharpFramework.Shapes;
using UIEditorManaged;
using System.Drawing;




namespace UIEditorPlugin
{   
    public class UIEditorMouseContext : IMouseContext
    {
        public enum MOUSE_MODE
        {
            UI_NONE= 0,
            UI_MOVE,
            UI_SIZE,
            UI_COPY,          
            UI_MULTI_SELECT,
            UI_AREA_SELECT,
        };

        [Flags]
        public enum RESIZE_DIRECTION
        {       
            DIRECTION_NONE = 0,
            DIRECTION_TOP = 1,          
            DIRECTION_LEFT = 2 ,
            DIRECTION_RIGHT = 4,
            DIRECTION_BOTTOM = 8,
            DIRECTION_BOTTOM_RIGHT = 16,
            DIRECTION_BOTTOM_LEFT = 32,            
            DIRECTION_TOP_LEFT = 64,
            DIRECTION_TOP_RIGHT = 128,
        };
       
        MOUSE_MODE Dragmode = MOUSE_MODE.UI_NONE;
        Point m_MouseDownPoint;
        
        public static UIEditorMouseContext UIEditorContext = new UIEditorMouseContext();

        ShapeCollection m_UIShapeList = new ShapeCollection(); // 편집하고 있는 Shape 리스트

        ShapeCollection m_UICopyShapeList = new ShapeCollection(); // 복사된 Shape 리스트

        ShapeCollection m_selectedShapeList = new ShapeCollection(); // 선택된 Shape 리스트.. EditorManager.SelectedShapes 와는 별도로 관리하고, 수시로 동기화
        
        UIShapeDialog m_baseDialog = null;

        bool m_bCopying = false; // 복사중이므로, selectedShape 등의 처리는 복사도중에는 패스하고, 복사가 완료된 시점에서 처리

        public UIShapeBase CreateNewShape(UIShapeBase shape, UIShapeDialog parent, float PosX, float PosY)
        {
            UIShapeBase _newInst = (UIShapeBase)shape.Clone();
            _newInst.PosX = PosX;
            _newInst.PosY = PosY;

            EditorManager.Actions.Add(new AddShapeAction(_newInst, m_baseDialog, shape.ParentLayer, true));

            return _newInst;
        }



        public void SetUIEditorMode(bool bTrue)
        {
            if (bTrue)
            {
                EditorManager.ActiveView.CurrentContext = this;              
            }
            else
            {
           //     m_UIShapeList.Clear();
           //     m_selectedShapeList.Clear();
                EditorManager.ActiveView.SetDefaultContext();
            }
        }

        public void SelectUIShape(UIShapeBase shape)
        {
            if (m_bCopying == true)
                return;

            if ( m_selectedShapeList.Contains( shape ) == false )
                m_selectedShapeList.Add(shape); // 선택에 넣음

            if (shape == m_baseDialog || shape.Parent == m_baseDialog) // 작업 다이얼로그는 그대로
            {
                m_UIShapeList.Clear();

                foreach (ShapeBase child in m_baseDialog.ChildCollection)
                {
                    if (child is UIShapeBase)
                    {
                        m_UIShapeList.Add(child as UIShapeBase);
                    }
                }

                m_UIShapeList.Add(m_baseDialog); // 다이얼로그는 제일 마지막에 추가 ( 나중에 검색할때 가장 나중에 검색되도록 )                         
                
                return;
            }
            
            // 작업 다이얼로그가 변경됨
            if (shape is UIShapeDialog)
            {
                m_baseDialog = shape as UIShapeDialog;
            }
            else if (shape.Parent is UIShapeDialog)
            {                
                m_baseDialog = shape.Parent as UIShapeDialog;
            }
            else
            {
                return;
            }

            m_UIShapeList.Clear();
            foreach (ShapeBase child in m_baseDialog.ChildCollection)
            {
                if (child is UIShapeBase)
                {
                    m_UIShapeList.Add(child as UIShapeBase);
                }
            }

            m_UIShapeList.Add(m_baseDialog); // 다이얼로그는 제일 마지막에 추가 ( 나중에 검색할때 가장 나중에 검색되도록 )         
        }

        public void UnSelectUIShape(UIShapeBase shape)
        {
            //m_selectedShapeList.Remove(shape);
            //m_UIShapeList.Clear();

            if (m_bCopying == true)
                return;

            if (m_selectedShapeList.Contains(shape)) // 테스트
                m_selectedShapeList.Remove(shape);
            


     //       if (shape == m_baseDialog || shape.Parent == m_baseDialog)
            //{
            //    //if (m_selectedShapeList.Contains(shape)) // 테스트
            //        m_selectedShapeList.Remove(shape);
            //}
            
               


            //m_baseDialog = null;

//             m_UIShapeList.Remove(shape);
// 
//             foreach (ShapeBase child in shape.ChildCollection)
//             {
//                 if (child is UIShapeBase)
//                 {
//                     m_UIShapeList.Remove(child as UIShapeBase);
//                 }
//             }
        }

        public UIShapeBase GetUIShapeFromSelection(int x, int y)
        {

            float fX = 1.0f / UIEditorMessageService.GetInstance().GetResolutionScaleX() * (float)x;
            float fY = 1.0f / UIEditorMessageService.GetInstance().GetResolutionScaleY() * (float)y;

            foreach (UIShapeBase shape in m_selectedShapeList)
            {
                bool bResult = shape.Bound.IsInside(fX, fY);

                //bool bResult = ContainPoint(shape, fScaleX * (float)x, fScaleY * (float)y, fMargin);
                // && !(shape is UIShapeDialog)
                if (bResult == true && shape.Selected == true  )
                    return shape;
            }

            return null;
        }

        public UIShapeBase GetUIShape(int x, int y , bool includeDialog)
        {

            float fX = 1.0f / UIEditorMessageService.GetInstance().GetResolutionScaleX() * (float)x;
            float fY = 1.0f / UIEditorMessageService.GetInstance().GetResolutionScaleY() * (float)y;

            UIShapeBase findShape = null;

            foreach (UIShapeBase shape in m_UIShapeList)
            {
//                 if (includeDialog == false && shape is UIShapeDialog)
//                     continue;

                bool bResult = shape.Bound.IsInside(fX, fY);

                if (bResult == true)
                {
                    if (findShape == null )
                    {
                        findShape = shape;                        
                    }
                    else if (shape.ChildIndex > findShape.ChildIndex)
                    {
                        findShape = shape;
                    }
                    else if (shape.ChildIndex == findShape.ChildIndex && findShape is UIShapeDialog)
                    {
                        findShape = shape;
                    }
                }
            }

            return findShape;
        }

        public void MoveUIShapeDelta(UIShapeBase shape, float x, int y)
        {

            float fScaleX = 1.0f / UIEditorMessageService.GetInstance().GetResolutionScaleX();
            float fScaleY = 1.0f / UIEditorMessageService.GetInstance().GetResolutionScaleY();

            shape.PosX += (fScaleX * (float)x);
            shape.PosY += (fScaleX * (float)y);
        }

        public override void OnKeyDown(KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Escape) // Esc:Clears the selection
            {
                if (!View.Gizmo.Dragging)
                {
                    View.Gizmo.Clear();
                    View.UpdateView(false);

                    e.Handled = true;
                }
            }

            return;
        }

        public override void OnKeyUp(KeyEventArgs e)
        {
            return;
        }

        public override bool OnMouseDown(MouseEventArgs e, KeyModifier eKeyMod, int iOldX, int iOldY)
        {
            // 1. 현재 마우스 포지션의 UI 를 가지고 온다
            // 2. UI 가 있고, 커서가 경계에 겹칠 경우 - Select 해주고 마우스 커서 변경 , Move Mode 로 Position 변경            
            // 3. UI 가 있고, 커서가 경계 안에 있을 경우 - 사이즈 변경


            if (e.Button == MouseButtons.Left)
            {
                UIShapeBase selectedShape = null;
                if (View.Cursor == Cursors.Arrow ) // 화살표일 경우 선택 모드
                {
                    m_selectedShapeList.Clear();
                    selectedShape = this.GetUIShape(e.X, e.Y, false);
                }
                else if ( View.Cursor == Cursors.Cross )
                {
                    selectedShape = this.GetUIShape(e.X, e.Y, false);
                }
                else// 화살표가 아니면 선택된 아이템중에 선택
                {
                    selectedShape = this.GetUIShapeFromSelection(e.X, e.Y);
                }
                
               
                if (eKeyMod == KeyModifier.Shift) // Shift 누르면 복사
                {
                    m_bCopying = true;
                    Dragmode = MOUSE_MODE.UI_COPY;                    
                }             

                if (selectedShape != null)
                {
                    if (m_selectedShapeList.Contains(selectedShape ) == false )
                        m_selectedShapeList.Add(selectedShape);
                }

            }

//             if (EditorManager.SelectedShapes.Contains(m_selectedShape) == false )
//             {          
//                 EditorManager.SelectedShapes = new CSharpFramework.ShapeCollection();
//                 EditorManager.SelectedShapes.Add(m_selectedShape);
//             }

            if (m_selectedShapeList.Count >= 1 )
            {
                if (View.Cursor == Cursors.SizeAll)
                {
                    if (eKeyMod == KeyModifier.Shift && Dragmode == MOUSE_MODE.UI_COPY) // 시프트키 눌리고 , 셀렉티드 쉐입에 , Move Mode 이면 복사
                    {
                        m_bCopying = true;

                        foreach (UIShapeBase shape in m_selectedShapeList )
                        {
                            if (shape != null)
                            {
                                UIShapeBase copyShape = CreateNewShape(shape, m_baseDialog, shape.PosX, shape.PosY);
                                m_UICopyShapeList.Add(copyShape);                              
                            }
                        }

                        m_selectedShapeList.Clear();

                        m_selectedShapeList.AddRange(m_UICopyShapeList);
                        m_UIShapeList.AddRange(m_UICopyShapeList);

// 
//                         foreach (UIShapeBase shape in m_UICopyShapeList)
//                         {
//                             EditorManager.Actions.Add(new AddShapeAction(shape, m_baseDialog, shape.ParentLayer, true));
//                         }

// 
//                         EditorManager.SelectedShapes.Clear(); // 기존 선택된 UI 들 지우고
//                         EditorManager.SelectedShapes.AddRange(m_UICopyShapeList); // 복사된거만 선택


                        m_bCopying = false;

                        //UIShapeBase copyShape = CreateNewShape(m_selectedShape, m_baseDialog, m_selectedShape.PosX, m_selectedShape.PosY);

                        //m_selectedShape = copyShape;
                        //UIShapeBase copyShape = (UIShapeBase)m_selectedShape.Clone();

                        //m_UICopyShapeList.Add(copyShape);
                    }
                    else
                    {
                        Dragmode = MOUSE_MODE.UI_MOVE;   
                    }                   
                }
                else if ( View.Cursor ==  Cursors.Arrow )
                {
                    
                }
                else
                {
                    Dragmode = MOUSE_MODE.UI_SIZE;
                }
            }

            if (Dragmode == MOUSE_MODE.UI_COPY)
            {
                ShapeCollection collection = new CSharpFramework.ShapeCollection();
                collection.AddRange(m_UICopyShapeList);

                EditorManager.SelectedShapes = collection;                
            }
            else
            {
                ShapeCollection collection = new CSharpFramework.ShapeCollection();
                collection.AddRange(m_selectedShapeList);

                EditorManager.SelectedShapes = collection;                
            }

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
            if (Dragmode == MOUSE_MODE.UI_AREA_SELECT)
            {
                View.RenderRectangle2D(m_MouseDownPoint.X, m_MouseDownPoint.Y, e.X, e.Y, VisionColors.RGBA(255, 255, 255, 255), 1.0f);               
            }
            else if (eKeyMod == KeyModifier.Ctrl )
            {

            }
            else
            {
                if (m_selectedShapeList.Count > 0 )
                {
                    if (Dragmode == MOUSE_MODE.UI_MOVE || Dragmode == MOUSE_MODE.UI_COPY )
                    {
                        // this.MoveUIShape(m_selectedShape, e.X, e.Y);
                        foreach (UIShapeBase shape in m_selectedShapeList)
                        {
                            this.MoveUIShapeDelta(shape, e.X - iOldX, e.Y - iOldY);
                        }                        
                    }                    
                    else if (Dragmode == MOUSE_MODE.UI_SIZE)
                    {
                        foreach (UIShapeBase shape in m_selectedShapeList)
                        {                            
                            Rectangle2D newRect = new Rectangle2D();

                            float fDeltaX = (e.X - iOldX);
                            float fDeltaY = (e.Y - iOldY);

                            if (View.Cursor == Cursors.PanNW)
                            {
                                newRect.Add(shape.ScreenBound.X2, shape.ScreenBound.Y2);
                                newRect.Add(shape.ScreenBound.X1 + fDeltaX, shape.ScreenBound.Y1 + fDeltaY);

                                newRect.Validate();
                                shape.ScreenBound = newRect;
                            }
                            else if (View.Cursor == Cursors.PanSW)
                            {
                                newRect.Add(shape.ScreenBound.X2, shape.ScreenBound.Y1);
                                newRect.Add(shape.ScreenBound.X1 + fDeltaX, shape.ScreenBound.Y2 + fDeltaY);

                                newRect.Validate();
                                shape.ScreenBound = newRect;
                            }
                            else if (View.Cursor == Cursors.PanNE)
                            {
                                newRect.Add(shape.ScreenBound.X1, shape.ScreenBound.Y2);
                                newRect.Add(shape.ScreenBound.X2 + fDeltaX, shape.ScreenBound.Y1 + fDeltaY);

                                newRect.Validate();
                                shape.ScreenBound = newRect;
                            }
                            else if (View.Cursor == Cursors.PanSE)
                            {
                                newRect.Add(shape.ScreenBound.X1, shape.ScreenBound.Y1);
                                newRect.Add(shape.ScreenBound.X2 + fDeltaX, shape.ScreenBound.Y2 + fDeltaY);

                                newRect.Validate();
                                shape.ScreenBound = newRect;
                            }
                        }
                    }
                }
                else
                {

                }

                if (Dragmode == MOUSE_MODE.UI_NONE)
                {
                    UIShapeBase tempShape = this.GetUIShapeFromSelection(e.X, e.Y);
               

                    if (tempShape == null)
                    {
                        View.Cursor = Cursors.Default;
                    }                  
                    else
                    {
                        int margin = 15;
                        Rectangle2D mouseBound = new Rectangle2D();

                        mouseBound.Add(e.X - margin, e.Y - margin);
                        mouseBound.Add(e.X + margin, e.Y + margin);


                        float fX1 = tempShape.ScreenBound.X1;
                        float fY1 = tempShape.ScreenBound.Y1;
                        float fX2 = tempShape.ScreenBound.X2;
                        float fY2 = tempShape.ScreenBound.Y2;

                        if (mouseBound.IsInside(fX1, fY1))
                            View.Cursor = Cursors.PanNW;
                        else if (mouseBound.IsInside(fX1, fY2))
                            View.Cursor = Cursors.PanSW;
                        else if (mouseBound.IsInside(fX2, fY1))
                            View.Cursor = Cursors.PanNE;
                        else if (mouseBound.IsInside(fX2, fY2))
                            View.Cursor = Cursors.PanSE;
                        else if (tempShape is UIShapeDialog)      // 다이얼로그는 사이즈조정밖에 안됨
                        {
                            View.Cursor = Cursors.Arrow;                        
                        }
                        else
                        {
                            View.Cursor = Cursors.SizeAll;
                        }  
                    }
                }
            }

            return false;
        }


        public override bool OnMouseUp(MouseEventArgs e, KeyModifier eKeyMod, int iOldX, int iOldY)
        {
            if (Dragmode == MOUSE_MODE.UI_AREA_SELECT)
            {
                 // 범위내의 Shape 들을 전부  Select
            }


            if (Dragmode == MOUSE_MODE.UI_MOVE || Dragmode == MOUSE_MODE.UI_COPY )
            {
                foreach (UIShapeBase shape in m_selectedShapeList)
                {
                    EditorManager.Actions.Add(new SetPropertyAction(shape, "PosX", shape.PosX));
                    EditorManager.Actions.Add(new SetPropertyAction(shape, "PosY", shape.PosY));
                }
            }
            else if (Dragmode == MOUSE_MODE.UI_SIZE)
            {
                foreach (UIShapeBase shape in m_selectedShapeList)
                {
                    EditorManager.Actions.Add(new SetPropertyAction(shape, "SizeX", shape.SizeX));
                    EditorManager.Actions.Add(new SetPropertyAction(shape, "SizeY", shape.SizeY));
                }
            }
            else
            {
                //                 bool addSelection = false;
                //                 bool toggleSelection = false;
                // 
                //                 if ((eKeyMod & KeyModifier.Ctrl) > 0 && (eKeyMod & KeyModifier.Shift) > 0)
                //                     addSelection = true;
                // 
                //                 UIShapeBase hitShape = this.GetUIShape(e.X, e.Y);
                // 
                //                 if (addSelection)
                //                     View.Gizmo.AddShape(hitShape, false);
                //                 else if (toggleSelection)
                //                     View.Gizmo.AddShape(hitShape, true);
                //                 else
                //                     View.Gizmo.SetSingleShape(hitShape);

            }

          //  m_selectedShapeList.Clear();
            m_UICopyShapeList.Clear();
            Dragmode = MOUSE_MODE.UI_NONE;
            m_bCopying = false;
            //m_selectedShape = null;
            
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
            if ((eNewMod & KeyModifier.Ctrl) > 0) // Pick
            {
                View.Cursor = Cursors.Cross;
            }
            else
            {
                View.Cursor = Cursors.Default;
            }
        }


        public void DrawMultiSelectArea()
        {
          

//             base.RenderShape(view, mode);
// 
//             if (this.Selected == true)
//             {
//                 uint iColor = VisionColors.RGBA(0, 0, 255, 255);
// 
//                 float fScaleX = UIEditorMessageService.GetInstance().GetResolutionScaleX();
//                 float fScaleY = UIEditorMessageService.GetInstance().GetResolutionScaleY();
// 
//                 float fScreenPosX = PosX;
//                 float fScreenPosY = PosY;
// 
//                 if (Parent != null && Parent is UIShapeBase)
//                 {
//                     fScreenPosX += ((UIShapeBase)Parent).PosX;
//                     fScreenPosY += ((UIShapeBase)Parent).PosY;
//                 }
// 
//                 view.RenderRectangle2D(fScreenPosX * fScaleX, fScreenPosY * fScaleY, (fScreenPosX + SizeX) * fScaleX, (fScreenPosY + SizeY) * fScaleY, iColor, 1.0f);
//             }           
        }
    }

}
