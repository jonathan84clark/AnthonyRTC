using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace RTCProgrammer
{
    public partial class MainForm : Form
    {
        private SerialPort serialPort;

        public MainForm()
        {
            InitializeComponent();
        }

        private void Connect()
        {
            serialPort = new SerialPort();

            // Allow the user to set the appropriate properties.
            serialPort.PortName = "COM13";
            serialPort.BaudRate = 9600;
            serialPort.Parity = 0;
            serialPort.DataBits = 8;
            serialPort.StopBits = StopBits.One;
            serialPort.Open();
            Console.WriteLine("Connected");
        }

        private void SendTimeSetting()
        {
            DateTime date = DateTime.Now;
            byte[] buffer = new byte[16];
            // Set password
            buffer[0] = 145;
            buffer[1] = 198;
            buffer[2] = 1; // Indicate that we need to set the time
            buffer[3] = (byte)(date.Year % 100); // Year as first byte
            buffer[4] = (byte)date.Month;
            buffer[5] = (byte)date.Day;
            buffer[6] = (byte)date.DayOfWeek;
            buffer[7] = (byte)date.Hour;
            buffer[8] = (byte)date.Minute;
            buffer[9] = (byte)date.Second;
            serialPort.Write(buffer, 0, 16);

        }

        private void SendTestCommand(bool greenOn, bool greenOff, bool redOn, bool redOff)
        {
            byte[] buffer = new byte[16];
            // Set password
            buffer[0] = 145;
            buffer[1] = 198;
            if (greenOn)
            {
                buffer[3] = 1;
            }
            else if (!greenOn)
            {
                buffer[3] = 2;
            }
            if (redOn)
            {
                buffer[4] = 1;
            }
            else if (!redOn)
            {
                buffer[4] = 2;
            }
            buffer[5] = 255;

            serialPort.Write(buffer, 0, 16);
            //serialPort.Close();
            //serialPort.Handshake = SetPortHandshake(_serialPort.Handshake);
        }

        private void button1_Click(object sender, EventArgs e)
        {

        }

        private void grnOnBtn_Click(object sender, EventArgs e)
        {
            SendTestCommand(true, false, false,false);
        }

        private void grnOffBtn_Click(object sender, EventArgs e)
        {
            SendTestCommand(false, true, false, false);
        }

        private void redOn_Click(object sender, EventArgs e)
        {
            SendTestCommand(false, false, true, false);
        }

        private void redOff_Click(object sender, EventArgs e)
        {
            SendTestCommand(false, false, false, true);
        }

        private void connectBtn_Click(object sender, EventArgs e)
        {
            Connect();
        }

        private void disconnectBtn_Click(object sender, EventArgs e)
        {
            serialPort.Close();
        }

        private void updateTime_Click(object sender, EventArgs e)
        {
            SendTimeSetting();
        }
    }
}
