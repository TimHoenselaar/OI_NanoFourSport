using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Windows.Forms;
//using LiveCharts;
//using LiveCharts.Wpf;
using System.IO.Ports;
using System.Net;
using System.Net.Sockets;
using System.Windows.Media;
//using LiveCharts.Geared;
using System.Threading;
using System.Threading.Tasks;
using System.Globalization;
using System.Windows;

namespace MyoVisualizedApp
{
    public partial class Form1 : Form
    {
        //udp stuff
        UDPServer server = new UDPServer();
        List<Sample> runData = new List<Sample>();
        const float upperThreshold = 120;
        const float lowerThreshold = 120;
        bool aboveUpperThresholdValue = false;

        string SelectedAxis1, SelectedAxis2, SelectedAxis3 = ""; // nothing as default
        SerialCom serCom = new SerialCom();

        bool liveData = false;

        int stepCounter = 0;

        public char SplitChar = ' ';
        public char SplitCharLoaded = ',';
        public int displayTimer = 4000; //amount of milliseconds that should be displayed (4000 = 4sec)
        public float roll, pitch, yaw, gyro_x, gyro_y, gyro_z, accel_x, accel_y, accel_z;
        public Int32 oldTime = 100;

        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            SelectedAxis1 = comboBox1.Text;
            dataGraph.Series["Series1"].Points.Clear();
            dataGraph.Series["Series1"].LegendText = comboBox1.Text;
        }
        private void comboBox2_SelectedIndexChanged(object sender, EventArgs e)
        {
            SelectedAxis2 = comboBox2.Text;
            dataGraph.Series["Series2"].Points.Clear();
            dataGraph.Series["Series2"].LegendText = comboBox2.Text;
        }

        private void comboBox3_SelectedIndexChanged(object sender, EventArgs e)
        {
            SelectedAxis3 = comboBox3.Text;
            dataGraph.Series["Series3"].Points.Clear();
            dataGraph.Series["Series3"].LegendText = comboBox3.Text;
        }

        public int EMG0, EMG1, EMG2, EMG3, EMG4, EMG5, EMG6, EMG7, accel_x_arduino, accel_y_arduino, accel_z_arduino, stepDetect, muscleTension;

        int simTimer = 0;

        public Form1()
        {
            InitializeComponent();

            getAvailablePorts();
        }

        void getAvailablePorts()
        {
            String[] ports = SerialPort.GetPortNames();
            comboBox1.Items.AddRange(ports);
        }

        private void btnLive_Click(object sender, EventArgs e)
        {
            //adding udp connection
            server.Start();
            //starting simulation
            startTime.Start();
            liveData = true;
        }

        private void startTime_Tick(object sender, EventArgs e)
        {
            if (liveData)
            {
                //Read from Server
                try { 
                string DataLineLive = server.receivedMessage;
                string[] singleLine = DataLineLive.Split(SplitChar);
                float.TryParse(singleLine[0], NumberStyles.Any, CultureInfo.InvariantCulture, out roll);
                float.TryParse(singleLine[1], NumberStyles.Any, CultureInfo.InvariantCulture, out pitch);
                float.TryParse(singleLine[2], NumberStyles.Any, CultureInfo.InvariantCulture, out yaw);
                float.TryParse(singleLine[3], NumberStyles.Any, CultureInfo.InvariantCulture, out gyro_x);
                float.TryParse(singleLine[4], NumberStyles.Any, CultureInfo.InvariantCulture, out gyro_y);
                float.TryParse(singleLine[5], NumberStyles.Any, CultureInfo.InvariantCulture, out gyro_z);
                float.TryParse(singleLine[6], NumberStyles.Any, CultureInfo.InvariantCulture, out accel_x);
                float.TryParse(singleLine[7], NumberStyles.Any, CultureInfo.InvariantCulture, out accel_y);
                float.TryParse(singleLine[8], NumberStyles.Any, CultureInfo.InvariantCulture, out accel_z);
                int.TryParse(singleLine[9], out EMG0);
                int.TryParse(singleLine[10], out EMG1);
                int.TryParse(singleLine[11], out EMG2);
                int.TryParse(singleLine[12], out EMG3);
                int.TryParse(singleLine[13], out EMG4);
                int.TryParse(singleLine[14], out EMG5);
                int.TryParse(singleLine[15], out EMG6);
                int.TryParse(singleLine[16], out EMG7);
                int.TryParse(singleLine[17], out stepDetect);
                int.TryParse(singleLine[18], out muscleTension);
                int.TryParse(singleLine[19], out accel_x_arduino);
                int.TryParse(singleLine[20], out accel_y_arduino);
                int.TryParse(singleLine[20], out accel_z_arduino);
                Int32.TryParse(singleLine[21], out time);

                Sample sampleData = new Sample(roll, pitch, yaw, gyro_x, gyro_y, gyro_z, accel_x, accel_y, accel_z, EMG0, EMG1, EMG2, EMG3, EMG4, EMG5, EMG6, EMG7, stepDetect, muscleTension, time);

                dataGraph.ChartAreas[0].AxisX.Minimum = sampleData.time - displayTimer;
                dataGraph.ChartAreas[1].AxisX.Minimum = sampleData.time - displayTimer;
                dataGraph.ChartAreas[2].AxisX.Minimum = sampleData.time - displayTimer;

                if (sampleData.time < oldTime)
                {
                    dataGraph.Series["Series1"].Points.Clear();
                    dataGraph.Series["Series2"].Points.Clear();
                    dataGraph.Series["Series3"].Points.Clear();
                }
                oldTime = sampleData.time;
                simTimer += 10;
                //label1.Text = simTimer+"";
                if (simTimer > 4000)
                {

                    //resetGraphYScale();
                    simTimer = 0;               
                }
                
                //selecting the right graph for the right axis
                switch (SelectedAxis1)
                {
                    case "Roll":
                        dataGraph.Series["Series1"].Points.AddXY(sampleData.time, sampleData.roll);
                        break;
                    case "Pitch":
                        dataGraph.Series["Series1"].Points.AddXY(sampleData.time, sampleData.pitch);
                        break;
                    case "Yaw":
                        dataGraph.Series["Series1"].Points.AddXY(sampleData.time, sampleData.yaw);
                        break;
                    case "Gyro_X":
                        dataGraph.Series["Series1"].Points.AddXY(sampleData.time, sampleData.gyro_x);
                        break;
                    case "Gyro_Y":
                        dataGraph.Series["Series1"].Points.AddXY(sampleData.time, sampleData.gyro_y);
                        break;
                    case "Gyro_Z":
                        dataGraph.Series["Series1"].Points.AddXY(sampleData.time, sampleData.gyro_z);
                        break;
                    case "Accel_X":
                        dataGraph.Series["Series1"].Points.AddXY(sampleData.time, sampleData.accel_x);
                        break;
                    case "Accel_Y":
                        dataGraph.Series["Series1"].Points.AddXY(sampleData.time, sampleData.accel_y);
                        break;
                    case "Accel_Z":
                        dataGraph.Series["Series1"].Points.AddXY(sampleData.time, sampleData.accel_z);
                        break;
                    case "EMG_0":
                        dataGraph.Series["Series1"].Points.AddXY(sampleData.time, sampleData.EMG0);
                        break;
                    case "EMG_1":
                        dataGraph.Series["Series1"].Points.AddXY(sampleData.time, sampleData.EMG1);
                        break;
                    case "EMG_2":
                        dataGraph.Series["Series1"].Points.AddXY(sampleData.time, sampleData.EMG2);
                        break;
                    case "EMG_3":
                        dataGraph.Series["Series1"].Points.AddXY(sampleData.time, sampleData.EMG3);
                        break;
                    case "EMG_4":
                        dataGraph.Series["Series1"].Points.AddXY(sampleData.time, sampleData.EMG4);
                        break;
                    case "EMG_5":
                        dataGraph.Series["Series1"].Points.AddXY(sampleData.time, sampleData.EMG5);
                        break;
                    case "EMG_6":
                        dataGraph.Series["Series1"].Points.AddXY(sampleData.time, sampleData.EMG6);
                        break;
                    case "EMG_7":
                        dataGraph.Series["Series1"].Points.AddXY(sampleData.time, sampleData.EMG7);
                        break;
                    case "accel_x_arduino":
                        dataGraph.Series["Series1"].Points.AddXY(sampleData.time, sampleData.accel_x_arduino);
                        break;
                    case "accel_y_arduino":
                        dataGraph.Series["Series1"].Points.AddXY(sampleData.time, sampleData.accel_y_arduino);
                        break;
                    case "accel_z_arduino":
                        dataGraph.Series["Series1"].Points.AddXY(sampleData.time, sampleData.accel_z_arduino);
                        break;
                    case "StepDetect":
                        dataGraph.Series["Series1"].Points.AddXY(sampleData.time, sampleData.stepDetect);
                        break;
                    case "MuscleTension":
                        dataGraph.Series["Series1"].Points.AddXY(sampleData.time, sampleData.muscleTension);
                        break;
                    default:
                        break;
                }
                switch (SelectedAxis2)
                {
                    case "Roll":
                        dataGraph.Series["Series2"].Points.AddXY(sampleData.time, sampleData.roll);
                        break;
                    case "Pitch":
                        dataGraph.Series["Series2"].Points.AddXY(sampleData.time, sampleData.pitch);
                        break;
                    case "Yaw":
                        dataGraph.Series["Series2"].Points.AddXY(sampleData.time, sampleData.yaw);
                        break;
                    case "Gyro_X":
                        dataGraph.Series["Series2"].Points.AddXY(sampleData.time, sampleData.gyro_x);
                        break;
                    case "Gyro_Y":
                        dataGraph.Series["Series2"].Points.AddXY(sampleData.time, sampleData.gyro_y);
                        break;
                    case "Gyro_Z":
                        dataGraph.Series["Series2"].Points.AddXY(sampleData.time, sampleData.gyro_z);
                        break;
                    case "Accel_X":
                        dataGraph.Series["Series2"].Points.AddXY(sampleData.time, sampleData.accel_x);
                        break;
                    case "Accel_Y":
                        dataGraph.Series["Series2"].Points.AddXY(sampleData.time, sampleData.accel_y);
                        break;
                    case "Accel_Z":
                        dataGraph.Series["Series2"].Points.AddXY(sampleData.time, sampleData.accel_z);
                        break;
                    case "EMG_0":
                        dataGraph.Series["Series2"].Points.AddXY(sampleData.time, sampleData.EMG0);
                        break;
                    case "EMG_1":
                        dataGraph.Series["Series2"].Points.AddXY(sampleData.time, sampleData.EMG1);
                        break;
                    case "EMG_2":
                        dataGraph.Series["Series2"].Points.AddXY(sampleData.time, sampleData.EMG2);
                        break;
                    case "EMG_3":
                        dataGraph.Series["Series2"].Points.AddXY(sampleData.time, sampleData.EMG3);
                        break;
                    case "EMG_4":
                        dataGraph.Series["Series2"].Points.AddXY(sampleData.time, sampleData.EMG4);
                        break;
                    case "EMG_5":
                        dataGraph.Series["Series2"].Points.AddXY(sampleData.time, sampleData.EMG5);
                        break;
                    case "EMG_6":
                        dataGraph.Series["Series2"].Points.AddXY(sampleData.time, sampleData.EMG6);
                        break;
                    case "EMG_7":
                        dataGraph.Series["Series2"].Points.AddXY(sampleData.time, sampleData.EMG7);
                        break;
                    case "accel_x_arduino":
                        dataGraph.Series["Series2"].Points.AddXY(sampleData.time, sampleData.accel_x_arduino);
                        break;
                    case "accel_y_arduino":
                        dataGraph.Series["Series2"].Points.AddXY(sampleData.time, sampleData.accel_y_arduino);
                        break;
                    case "accel_z_arduino":
                        dataGraph.Series["Series2"].Points.AddXY(sampleData.time, sampleData.accel_z_arduino);
                        break;
                    case "StepDetect":
                        dataGraph.Series["Series2"].Points.AddXY(sampleData.time, sampleData.stepDetect);
                        break;
                    case "MuscleTension":
                        dataGraph.Series["Series2"].Points.AddXY(sampleData.time, sampleData.muscleTension);
                        break;
                    default:
                        break;
                }
                switch (SelectedAxis3)
                {
                    case "Roll":
                        dataGraph.Series["Series3"].Points.AddXY(sampleData.time, sampleData.roll);
                        break;
                    case "Pitch":
                        dataGraph.Series["Series3"].Points.AddXY(sampleData.time, sampleData.pitch);
                        break;
                    case "Yaw":
                        dataGraph.Series["Series3"].Points.AddXY(sampleData.time, sampleData.yaw);
                        break;
                    case "Gyro_X":
                        dataGraph.Series["Series3"].Points.AddXY(sampleData.time, sampleData.gyro_x);
                        break;
                    case "Gyro_Y":
                        dataGraph.Series["Series3"].Points.AddXY(sampleData.time, sampleData.gyro_y);
                        break;
                    case "Gyro_Z":
                        dataGraph.Series["Series3"].Points.AddXY(sampleData.time, sampleData.gyro_z);
                        break;
                    case "Accel_X":
                        dataGraph.Series["Series3"].Points.AddXY(sampleData.time, sampleData.accel_x);
                        break;
                    case "Accel_Y":
                        dataGraph.Series["Series3"].Points.AddXY(sampleData.time, sampleData.accel_y);
                        break;
                    case "Accel_Z":
                        dataGraph.Series["Series3"].Points.AddXY(sampleData.time, sampleData.accel_z);
                        break;
                    case "EMG_0":
                        dataGraph.Series["Series3"].Points.AddXY(sampleData.time, sampleData.EMG0);
                        break;
                    case "EMG_1":
                        dataGraph.Series["Series3"].Points.AddXY(sampleData.time, sampleData.EMG1);
                        break;
                    case "EMG_2":
                        dataGraph.Series["Series3"].Points.AddXY(sampleData.time, sampleData.EMG2);
                        break;
                    case "EMG_3":
                        dataGraph.Series["Series3"].Points.AddXY(sampleData.time, sampleData.EMG3);
                        break;
                    case "EMG_4":
                        dataGraph.Series["Series3"].Points.AddXY(sampleData.time, sampleData.EMG4);
                        break;
                    case "EMG_5":
                        dataGraph.Series["Series3"].Points.AddXY(sampleData.time, sampleData.EMG5);
                        break;
                    case "EMG_6":
                        dataGraph.Series["Series3"].Points.AddXY(sampleData.time, sampleData.EMG6);
                        break;
                    case "EMG_7":
                        dataGraph.Series["Series3"].Points.AddXY(sampleData.time, sampleData.EMG7);
                        break;
                    case "accel_x_arduino":
                        dataGraph.Series["Series3"].Points.AddXY(sampleData.time, sampleData.accel_x_arduino);
                        break;
                    case "accel_y_arduino":
                        dataGraph.Series["Series3"].Points.AddXY(sampleData.time, sampleData.accel_y_arduino);
                        break;
                    case "accel_z_arduino":
                        dataGraph.Series["Series3"].Points.AddXY(sampleData.time, sampleData.accel_z_arduino);
                        break;
                    case "StepDetect":
                        dataGraph.Series["Series3"].Points.AddXY(sampleData.time, sampleData.stepDetect);
                        break;
                    case "MuscleTension":
                        dataGraph.Series["Series3"].Points.AddXY(sampleData.time, sampleData.muscleTension);
                        break;
                    default:
                        break;
                }

                }
                catch (NullReferenceException)
                {
                    ArgumentException ex = new ArgumentException("no connection to the server");
                    dataGraph.Series["Series1"].Points.Clear();
                    dataGraph.Series["Series2"].Points.Clear();
                    dataGraph.Series["Series3"].Points.Clear();
                    startTime.Stop();
                    //throw ex;
                }
            }
        } 

        public Int32 time;

        private void btnStartSimulation_Click(object sender, EventArgs e)
        {
            startTime.Start();
        }
        
        private void resetGraphYScale()
        {
            dataGraph.Update();
            for(int i = 0; i <= 2; i++)
            {
                dataGraph.ChartAreas[i].AxisX.Minimum = double.NaN;
                dataGraph.ChartAreas[i].AxisX.Maximum = double.NaN;
                dataGraph.ChartAreas[i].AxisY.Minimum = double.NaN;
                dataGraph.ChartAreas[i].AxisY.Maximum = double.NaN;
            }     
        }
    }
}
