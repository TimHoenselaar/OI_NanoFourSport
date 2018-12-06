using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Windows.Forms;
using LiveCharts;
using LiveCharts.Wpf;
using System.IO.Ports;
using System.Net;
using System.Net.Sockets;
using System.Windows.Media;
using LiveCharts.Geared;
using System.Threading;
using System.Threading.Tasks;

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
        public float roll, pitch, yaw, gyro_x, gyro_y, gyro_z, accel_x, accel_y, accel_z;

        private void comboBox2_SelectedIndexChanged(object sender, EventArgs e)
        {
            SelectedAxis2 = comboBox2.Text;
            dataGraph.Series["Series2"].Points.Clear();
        }

        private void comboBox3_SelectedIndexChanged(object sender, EventArgs e)
        {
            SelectedAxis3 = comboBox3.Text;
            dataGraph.Series["Series3"].Points.Clear();
        }

        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            SelectedAxis1 = comboBox1.Text;
            dataGraph.Series["Series1"].Points.Clear();
            label2.Text = SelectedAxis1;
        }

        public int EMG0, EMG1, EMG2, EMG3, EMG4, EMG5, EMG6, EMG7, stepDetect, muscleTension;

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
            /* COM PORT opening
            string portname = comboBox1.Text;
            serCom.OpenPort(serialPort, 115200, portname);
            */

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
                label1.Text = server.receivedMessage;

                //Read data from COM
                //string DataLineLive = serialPort.ReadLine();

                //Read from Server
                string DataLineLive = server.receivedMessage;

                string[] singleLine = DataLineLive.Split(SplitChar);
                float.TryParse(singleLine[0], out roll);
                float.TryParse(singleLine[1], out pitch);
                float.TryParse(singleLine[2], out yaw);
                float.TryParse(singleLine[3], out gyro_x);
                float.TryParse(singleLine[4], out gyro_y);
                float.TryParse(singleLine[5], out gyro_z);
                float.TryParse(singleLine[6], out accel_x);
                float.TryParse(singleLine[7], out accel_y);
                float.TryParse(singleLine[8], out accel_z);
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
                Int32.TryParse(singleLine[19], out time);

                Sample sampleData = new Sample(roll, pitch, yaw, gyro_x, gyro_y, gyro_z, accel_x, accel_y, accel_z, EMG0, EMG1, EMG2, EMG3, EMG4, EMG5, EMG6, EMG7, stepDetect, muscleTension, time);

                label3.Text = sampleData.EMG0 + "";
                dataGraph.ChartAreas[0].AxisX.Minimum = sampleData.time - 4000;
                dataGraph.ChartAreas[1].AxisX.Minimum = sampleData.time - 4000;
                dataGraph.ChartAreas[2].AxisX.Minimum = sampleData.time - 4000;

                simTimer += 10;
                /*
                if(simTimer > 5000)
                {
                    dataGraph.Series[0].Points.RemoveAt(5);
                    dataGraph.Series[1].Points.RemoveAt(5);
                    dataGraph.Series[2].Points.RemoveAt(5);

                    dataGraph.Series[0].Points.RemoveAt(15);
                    dataGraph.Series[1].Points.RemoveAt(15);
                    dataGraph.Series[2].Points.RemoveAt(15);
                    label3.Text = dataGraph.Series[0].Points.Count.ToString();
                    if (dataGraph.Series[0].Points.Count > 3000)
                    {
                        foreach (var series in dataGraph.Series)
                        {
                            series.Points.Clear();
                        }
                    }
                    simTimer = 0;
                }
                */
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
            else
            {
                if (simTimer < runData.Last().time)
                {
                    if(simTimer > 11000)
                    {
                        dataGraph.ChartAreas[0].AxisX.Minimum = simTimer - 11000;
                        dataGraph.ChartAreas[1].AxisX.Minimum = simTimer - 11000;
                        dataGraph.ChartAreas[2].AxisX.Minimum = simTimer - 11000;
                    }
                    simTimer += 10;
                    label1.Text = simTimer + "";
                    foreach (Sample a in runData)
                    {
                        if (a.time < simTimer + 10 && a.time > simTimer)
                        {
                            

                            if (a.gyro_z > upperThreshold && !aboveUpperThresholdValue)
                            {
                                aboveUpperThresholdValue = true;
                                stepCounter++;
                            }
                            else if (a.gyro_z < upperThreshold && aboveUpperThresholdValue)
                            {
                                aboveUpperThresholdValue = false;
                                stepCounter++;
                            }
                        }
                    }
                    label2.Text = stepCounter + "";
                }
            }
        } 

        public Int32 time;

        private void btnStartSimulation_Click(object sender, EventArgs e)
        {
            startTime.Start();
        } 

        private void btnLoadFile_Click(object sender, EventArgs e)
        {
            string sFileName = "";
            try
            {
                OpenFileDialog openFileDialog = new OpenFileDialog();

                if (openFileDialog.ShowDialog() == DialogResult.OK)
                {
                    sFileName = openFileDialog.FileName;
                }
                /*
                dataGraph.Series.Add("Gyroscope");
                dataGraph.Series["Gyroscope"].ChartType = SeriesChartType.Line;
                dataGraph.Series.Add("Acceleration");
                dataGraph.Series["Acceleration"].ChartType = SeriesChartType.Line;
                dataGraph.Series.Add("Rotation");
                dataGraph.Series["Rotation"].ChartType = SeriesChartType.Line;
                */
                StreamReader reader = new StreamReader(sFileName);
                while (reader.Peek() > -1)
                {
                    string DataLine = reader.ReadLine();
                    if (DataLine == File.ReadAllLines(sFileName).Last())
                    {
                        //break;
                    }
                    else
                    {
                        string[] singleLine = DataLine.Split(SplitCharLoaded);
                        float.TryParse(singleLine[0], out roll);
                        float.TryParse(singleLine[1], out pitch);
                        float.TryParse(singleLine[2], out yaw);
                        float.TryParse(singleLine[3], out gyro_x);
                        float.TryParse(singleLine[4], out gyro_y);
                        float.TryParse(singleLine[5], out gyro_z);
                        float.TryParse(singleLine[6], out accel_x);
                        float.TryParse(singleLine[7], out accel_y);
                        float.TryParse(singleLine[8], out accel_z);
                        Int32.TryParse(singleLine[9], out time);

                        Sample sampleData = new Sample(roll, pitch, yaw, gyro_x, gyro_y, gyro_z, accel_x, accel_y, accel_z, time);
                        runData.Add(sampleData);
                    }
                }
                //set ChartAreas
                //dataGraph.Series["Gyro"].ChartArea = "GyroArea";
                //dataGraph.Series["Accel"].ChartArea = "AccelArea";
                //dataGraph.Series["Rotation"].ChartArea = "RotationArea";
                //Set axis min/max for all 3 graphs
                //Gyro
                //dataGraph.ChartAreas["GyroArea"].AxisY.Minimum = -300;
                //dataGraph.ChartAreas["GyroArea"].AxisY.Maximum = 300;
                //Accel
                //dataGraph.ChartAreas["AccelArea"].AxisY.Minimum = -300;
                //dataGraph.ChartAreas["AccelArea"].AxisY.Maximum = 300;
                //Rotation
                //dataGraph.ChartAreas["RotationArea"].AxisY.Minimum = -2;
                //dataGraph.ChartAreas["RotationArea"].AxisY.Maximum = 2;
                //base X line based on maximum time
                //dataGraph.ChartAreas[0].AxisX.Minimum = 0;
                //dataGraph.ChartAreas[0].AxisX.Maximum = runData.Last().time;
            }
            catch (Exception)
            {

            }
        }
    }
}
