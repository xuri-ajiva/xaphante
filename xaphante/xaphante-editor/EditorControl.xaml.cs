using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Windows.Threading;
using System.Xml;
using ICSharpCode.AvalonEdit.CodeCompletion;
using ICSharpCode.AvalonEdit.Document;
using ICSharpCode.AvalonEdit.Editing;
using ICSharpCode.AvalonEdit.Highlighting;
using ICSharpCode.AvalonEdit.Search;

namespace xaphante_editor
{
    /// <summary>
    /// Interaction logic for EditorControl.xaml
    /// </summary>
    public partial class EditorControl : UserControl
    {
        private CompletionWindow completionWindow;

        public EditorControl()
        {
            // Load our custom highlighting definition
            IHighlightingDefinition customHighlighting;
            using (Stream s = typeof(EditorControl).Assembly.GetManifestResourceStream("xaphante_editor.Glsl.xml"))
            {
                if (s == null)
                    throw new InvalidOperationException("Could not find embedded resource");

                using (XmlReader reader = new XmlTextReader(s))
                {
                    customHighlighting = ICSharpCode.AvalonEdit.Highlighting.Xshd.HighlightingLoader.Load(reader, HighlightingManager.Instance);
                }
            }

            // and register it in the HighlightingManager
            HighlightingManager.Instance.RegisterHighlighting("glsl", new string[] { ".ftag", ".vert" }, customHighlighting);
            var myctx = HighlightingManager.Instance.GetDefinition("glsl");
            foreach (var rule in myctx.MainRuleSet.Rules)
            {
                var st = rule.Regex.ToString();
                st = st.Replace("\\b(", "");
                st = st.Replace("\\b", "");
                st = st.Replace("\\", "");
                st = st.Replace("?>", "");
                st = st.Replace(")\\b", "");

                if (st[0] == '(')
                    st = st.Remove(0, 1);

                if (st[^1] == ')')
                    st = st.Remove(st.Length - 1, 1);

                var ctr = st.Split("|");
                foreach (string s in ctr)
                {
                    this.data.Add(new MyCompletionData(s));
                }
            }

            InitializeComponent();

            SetValue(TextOptions.TextFormattingModeProperty, TextFormattingMode.Display);

            //textEditor.TextArea.SelectionBorder = null;

            //textEditor.SyntaxHighlighting = HighlightingManager.Instance.GetDefinition("C#");
            //textEditor.SyntaxHighlighting = customHighlighting;
            // initial highlighting now set by XAML

            //textEditor.TextArea.TextEntering += textEditor_TextArea_TextEntering;
            //textEditor.TextArea.TextEntered  += textEditor_TextArea_TextEntered;
            SearchPanel.Install(this.textEditor);
        }

        ObservableCollection<ICompletionData> data = new ObservableCollection<ICompletionData>();
        

        private void TextEditor_OnKeyDown(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.LeftCtrl)
            {
                this.completionWindow = new CompletionWindow(this.textEditor.TextArea);

                foreach (var completionData in this.data)
                {
                    this.completionWindow.CompletionList.CompletionData.Add(completionData);
                }

                this.completionWindow.Show();
                this.completionWindow.Closed += delegate {
                    this.completionWindow = null;
                };
            }
            else if (e.Key == Key.Enter)
            {
                if (this.completionWindow != null)
                {
                    this.completionWindow.CompletionList.RequestInsertion(e);
                    e.Handled = true;
                }
            }
        }
    }

    /// <summary>
    /// Implements AvalonEdit ICompletionData interface to provide the entries in the completion drop down.
    /// </summary>
    public class MyCompletionData : ICompletionData
    {
        public MyCompletionData(string text)
        {
            this.Text = text;
        }

        public ImageSource Image { get { return null; } }

        public string Text { get; private set; }

        // Use this property if you want to show a fancy UIElement in the drop down list.
        public object Content { get { return this.Text; } }

        public object Description { get { return "Description for " + this.Text; } }

        public double Priority { get { return 0; } }

        public void Complete(TextArea textArea, ISegment completionSegment, EventArgs insertionRequestEventArgs)
        {
            textArea.Document.Replace(completionSegment, this.Text);
        }
    }
}
