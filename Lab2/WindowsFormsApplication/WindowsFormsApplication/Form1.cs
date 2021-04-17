using System;
using System.Data;
using System.Linq;
using System.Windows.Forms;
using System.IO.Ports;
using System.Drawing;
using System.Threading;
using System.Threading.Tasks;

namespace WindowsFormsApplication1
{
    public partial class Form1 : Form
    {
        private int algorithmCount = 0;
        private int algorithmNumber;

        public Form1()
        {
            InitializeComponent();
        }
        
        private void comboBox1_Click(object sender, EventArgs e)
        {
            comboBox1.Items.Clear();
            string[] ports = SerialPort.GetPortNames().OrderBy(a => a.Length > 3 && int.TryParse(a.Substring(3), out int num) ? num : 0).ToArray();
            comboBox1.Items.AddRange(ports);
        }

        private void buttonOpenPort_Click(object sender, EventArgs e)
        {
            if (!serialPort1.IsOpen)
                try
                {
                    serialPort1.PortName = comboBox1.Text;
                    serialPort1.Open();
                    buttonOpenPort.Text = "Close";
                    comboBox1.Enabled = false;
                    button1.Visible = true;
                    button2.Visible = true;
                }
                catch
                {
                    MessageBox.Show("Port " + comboBox1.Text + " is invalid!", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                }
            else
            {
                serialPort1.Close();
                buttonOpenPort.Text = "Open";
                comboBox1.Enabled = true;
                button1.Visible = false;
                button2.Visible = false;
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            byte[] b1 = new byte[1];
            b1[0] = 0xA1;
            serialPort1.Write(b1, 0, 1);
        }

        private void button2_Click(object sender, EventArgs e)
        {
            byte[] b1 = new byte[1];
            b1[0] = 0xB1;
            serialPort1.Write(b1, 0, 1);
        }

        private void clearAllLed()
        {
            panel1.BackColor = Color.SkyBlue;
            panel2.BackColor = Color.SkyBlue;
            panel3.BackColor = Color.SkyBlue;
            panel4.BackColor = Color.SkyBlue;
            panel6.BackColor = Color.SkyBlue;
            panel7.BackColor = Color.SkyBlue;
            panel8.BackColor = Color.SkyBlue;
            panel9.BackColor = Color.SkyBlue;
        }

        private void startTimer()
        {
            timer1.Start();
            //timer1.Interval = 400;
        }

        private async void timer1_Tick(object sender, EventArgs e)
        {
            Panel[] panels = {
                panel1,
                panel2,
                panel3,
                panel4,
                panel6,
                panel7,
                panel8,
                panel9,
            };
            clearAllLed();
            algorithmCount++;
            if (algorithmCount < 2)
            {
                if (algorithmNumber == 1)
                {

                  
                    for (int i = 0; i < 8; i++)
                    
                    {
                        if (i % 7 == 0)
                        panels[i].BackColor = Color.Red;
                        await Task.Delay(1100);
                        panels[i].BackColor = Color.SkyBlue;
                    }
                      for (int i = 0; i < 7; i++)
                        {
                        if (i % 5== 1)
                        panels[i].BackColor = Color.Red;
                        await Task.Delay(1100);
                        panels[i].BackColor = Color.SkyBlue;
                        }
                      for (int i = 2; i < 6; i++)
                        {
                        if (i % 3== 2)
                        panels[i].BackColor = Color.Red;
                        await Task.Delay(1100);
                        panels[i].BackColor = Color.SkyBlue;
                        }
                       for (int i = 3; i < 5; i++)
                        {
                       
                        panels[i].BackColor = Color.Red;
                        await Task.Delay(1100);
                        panels[i].BackColor = Color.SkyBlue;
                        }
                
}
                if (algorithmNumber == 2)
                {
                    for (int i = 7; i >= 0; i--)
                    {
                        panels[i].BackColor = Color.Red;
                        await Task.Delay(1100);
                        panels[i].BackColor = Color.SkyBlue;
                    }
                }
                timer1.Stop();
            } 
        }

        private void serialPort1_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            byte commandFromArduino = (byte)serialPort1.ReadByte();
            if (commandFromArduino == 0xB1)
            {
                algorithmNumber = 1;
                algorithmCount = 0;
                this.BeginInvoke(new ThreadStart(startTimer));
            }
            else if (commandFromArduino == 0xA1) 
            {
                algorithmNumber = 2;
                algorithmCount = 0;
                this.BeginInvoke(new ThreadStart(startTimer));
            }
        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }

        private void panel4_Paint(object sender, PaintEventArgs e)
        {

        }

        private void panel1_Paint(object sender, PaintEventArgs e)
        {

        }

        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {

        }
    }
}
