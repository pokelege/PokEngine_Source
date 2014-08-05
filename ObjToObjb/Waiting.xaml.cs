using System;
using System.Collections.Generic;
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
using System.Windows.Shapes;

namespace ObjToObjb
{
    /// <summary>
    /// Interaction logic for Window1.xaml
    /// </summary>
    public partial class Waiting : Window
    {
        Thread w;
        int part = 0;
        string message = "waiting...";
        int frame = 0;
        public Waiting(Thread toWait)
        {
            InitializeComponent();
            w = toWait;
            CompositionTarget.Rendering += checkState;
            la.Content = "waiting...";
        }

        private void checkState(object sender, EventArgs e)
        {
            frame++;
            if (frame == 30)
            {
                la.Content = message.Substring(0, part);
                if (part >= message.Length)
                {
                    part = 0;
                }
                part++;
                frame = 0;
            }

            if(!w.IsAlive)
            {
                MessageBox.Show("Success");
                this.Close();
            }
        }

        private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            if (w.IsAlive)
            {
                if (MessageBox.Show("Do you want to cancel?", "Cancel", MessageBoxButton.YesNo, MessageBoxImage.Question) == MessageBoxResult.Yes)
                {
                    w.Abort();
                }
                else
                {
                    e.Cancel = true;
                }
            }
            CompositionTarget.Rendering -= checkState;
        }
    }
}
