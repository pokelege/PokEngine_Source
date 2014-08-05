using Microsoft.Win32;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace ObjToObjb
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        OpenFileDialog Browse = new OpenFileDialog();
        Thread thread;
        public MainWindow()
        {
            InitializeComponent();
            CompositionTarget.Rendering += checkState;
        }

        private void checkState(object sender, EventArgs e)
        {
            StartB.IsEnabled = thread == null || !thread.IsAlive;
        }

        private void Start(object sender, RoutedEventArgs e)
        {
            if (!File.Exists(Filename.Text))
            {
                MessageBox.Show("File not found.");
                return;
            }
            FileInfo file = new FileInfo(Filename.Text);
            if (file.Length > 1048576)
            {
                if(MessageBox.Show("File is large. This might take a while.\nContinue?", "Large File", MessageBoxButton.YesNo, MessageBoxImage.Warning, MessageBoxResult.No) == MessageBoxResult.No)
                {
                    return;
                }
            }
            if (thread == null || !thread.IsAlive)
            {
                objbWriter writer = new objbWriter();
                writer.obj = Filename.Text;
                thread = new Thread(new ThreadStart(writer.writeObjb));
                thread.Start();
                Waiting waiting = new Waiting(thread);
                waiting.ShowDialog();
            }
            else
            {
                MessageBox.Show("There is a process");
            }
        }

        private void Import(object sender, RoutedEventArgs e)
        {
            Browse.ShowDialog();
            Filename.Text = Browse.FileName;
        }
    }
}
