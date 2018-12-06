using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Net;
using System.Net.Sockets;
using System.Threading;

namespace UDPRec
{
    public partial class Form1 : Form
    {
        delegate void ShowMessageMethod(string msg);

        UdpClient _server = null;
        IPEndPoint _client = null;
        Thread _listenThread = null;
        private bool _isServerStarted = false;

        public Form1()
        {
            InitializeComponent();
        }

        private void serverMsgBox_Load(object sender, EventArgs e)
        {
            this.btnStart.Text = "StartServer";
        }

        private void btnStart_Click(object sender, EventArgs e)
        {
            if (_isServerStarted)
            {
                Stop();
                btnStart.Text = "StartServer";
            }
            else
            {
                Start();
                btnStart.Text = "StopServer";
            }
        }

        private void Start()
        {
            //Create the server.
            IPEndPoint serverEnd = new IPEndPoint(IPAddress.Any, 1111);
            _server = new UdpClient(serverEnd);
            ShowMsg("Waiting for a client...");
            //Create the client end.
            _client = new IPEndPoint(IPAddress.Any, 0);

            //Start listening.
            Thread listenThread = new Thread(new ThreadStart(Listening));
            listenThread.Start();
            //Change state to indicate the server starts.
            _isServerStarted = true;
        }

        private void Stop()
        {
            try
            {
                //Stop listening.
                _listenThread.Join();
                ShowMsg("Server stops.");
                _server.Close();
                //Changet state to indicate the server stops.
                _isServerStarted = false;
            }
            catch (Exception excp)
            { }
        }

        private void Listening()
        {
            byte[] data;
            //Listening loop.
            while (true)
            {
                //receieve a message form a client.
                data = _server.Receive(ref _client);
                string receivedMsg = Encoding.ASCII.GetString(data, 0, data.Length);
                //Show the message.
                this.Invoke(new ShowMessageMethod(ShowMsg), new object[] { "Client:" + receivedMsg });
                //Send a response message.
                data = Encoding.ASCII.GetBytes("Server:" + receivedMsg);
                _server.Send(data, data.Length, _client);
                //Sleep for UI to work.
                //Thread.Sleep(500);
            }
        }
        private void ShowMsg(string msg)
        {
            this.richTextBox1.Text += msg + "\r\n";
        }
    }
}
