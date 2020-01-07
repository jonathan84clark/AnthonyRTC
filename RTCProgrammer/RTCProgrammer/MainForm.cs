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
            //show list of valid com ports
            foreach (string s in SerialPort.GetPortNames())
            {
                portsComboBox.Items.Add(s);
            }
            disconnectBtn.Enabled = false;
            updateTime.Enabled = false;
            clearEEPromBtn.Enabled = false;
            addTimeBtn.Enabled = false;
            grnOnBtn.Enabled = false;
            grnOffBtn.Enabled = false;
            redOff.Enabled = false;
            redOn.Enabled = false;
        }

        private void TermainalWrite(string inString)
        {
            consoleBox.Items.Add(inString);
        }
        
        /// <summary>
        /// Connects to the designated serial port
        /// </summary>
        private void Connect()
        {
            try
            {
                serialPort = new SerialPort();
                // Allow the user to set the appropriate properties.
                serialPort.PortName = portsComboBox.SelectedItem.ToString();
                serialPort.BaudRate = 9600;
                serialPort.Parity = 0;
                serialPort.DataBits = 8;
                serialPort.StopBits = StopBits.One;
                serialPort.Open();
                connectBtn.Enabled = false;
                disconnectBtn.Enabled = true;
                disconnectBtn.Enabled = true;
                updateTime.Enabled = true;
                clearEEPromBtn.Enabled = true;
                addTimeBtn.Enabled = true;
                grnOnBtn.Enabled = true;
                grnOffBtn.Enabled = true;
                redOff.Enabled = true;
                redOn.Enabled = true;
                TermainalWrite("Connected to port: " + serialPort.PortName);
            }
            catch (Exception ex)
            {
                TermainalWrite(ex.ToString());
            }
        }

        private void AddEventTime(int color, int startHourVal, int startMinuteVal, int endHourVal, int endMinuteVal)
        {
            byte[] buffer = new byte[16];
            buffer[0] = 145;
            buffer[1] = 198;
            buffer[2] = 2;
            buffer[3] = 0xFF;
            buffer[4] = (byte)color;
            buffer[5] = (byte)startHourVal;
            buffer[6] = (byte)startMinuteVal;
            buffer[7] = (byte)endHourVal;
            buffer[8] = (byte)endMinuteVal;
            serialPort.Write(buffer, 0, 16);
        }

        private void ClearEEProm()
        {
            byte[] buffer = new byte[16];
            buffer[0] = 145;
            buffer[1] = 198;
            buffer[2] = 3;
            buffer[3] = 0;
            buffer[4] = 0;
            buffer[5] = 0;
            buffer[6] = 0;
            buffer[7] = 0;
            buffer[8] = 0;
            serialPort.Write(buffer, 0, 16);
            TermainalWrite("Clear EEProm");
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
        }

        private void button1_Click(object sender, EventArgs e)
        {

        }

        private void grnOnBtn_Click(object sender, EventArgs e)
        {
            TermainalWrite("Enable Green LED");
            SendTestCommand(true, false, false,false);
        }

        private void grnOffBtn_Click(object sender, EventArgs e)
        {
            TermainalWrite("Disable Green LED");
            SendTestCommand(false, true, false, false);
        }

        private void redOn_Click(object sender, EventArgs e)
        {
            TermainalWrite("Enable Red LED");
            SendTestCommand(false, false, true, false);
        }

        private void redOff_Click(object sender, EventArgs e)
        {
            TermainalWrite("Disable Red LED");
            SendTestCommand(false, false, false, true);
        }

        private void connectBtn_Click(object sender, EventArgs e)
        {
            Connect();
        }

        /// <summary>
        /// Disconnects the application from the serial port
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void disconnectBtn_Click(object sender, EventArgs e)
        {
            connectBtn.Enabled = true;
            disconnectBtn.Enabled = false;
            disconnectBtn.Enabled = false;
            updateTime.Enabled = false;
            clearEEPromBtn.Enabled = false;
            addTimeBtn.Enabled = false;
            grnOnBtn.Enabled = false;
            grnOffBtn.Enabled = false;
            redOff.Enabled = false;
            redOn.Enabled = false;
            serialPort.Close();
        }

        private void updateTime_Click(object sender, EventArgs e)
        {
            SendTimeSetting();
        }

        private void addTimeBtn_Click(object sender, EventArgs e)
        {
            int color = ledColor.SelectedIndex + 1;
            int startHourVal = (int)startHour.Value;
            int startMinuteVal = (int)startMinute.Value;
            int endHourVal = (int)endHour.Value;
            int endMinuteVal = (int)endMinute.Value;
            if (color > 0)
            {
                AddEventTime(color, startHourVal, startMinuteVal, endHourVal, endMinuteVal);
                TermainalWrite("Time Sent");
            }
            else
            {
                TermainalWrite("Invalid Time");
            }
            //AddEventTime();
        }

        private void clearEEPromBtn_Click(object sender, EventArgs e)
        {
            ClearEEProm();
        }

        private void MainForm_Load(object sender, EventArgs e)
        {

        }

        private void startHour_SelectedIndexChanged(object sender, EventArgs e)
        {

        }
    }
}
