using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace UIEditorPlugin
{
    public partial class UIEditorLogPanel : Form
    {
        static UIEditorLogPanel staticPanel = new UIEditorLogPanel();
        public static UIEditorPlugin.UIEditorLogPanel StaticPanel
        {
            get { return staticPanel; }
            set { staticPanel = value; }
        }

        public UIEditorLogPanel()
        {
            StaticPanel.ShowDialog();
            InitializeComponent();
        }

        public void SetText1(string textLog)
        {
            this.textBox1.Text = textLog;
        }

        public void SetText2(string textLog)
        {
            this.textBox1.Text = textLog;
        }

        public void SetText3(string textLog)
        {
            this.textBox1.Text = textLog;
        }
    }
}
