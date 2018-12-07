using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net;
using System.Net.Sockets;
using System.Threading;

namespace MyoVisualizedApp
{
    public class UDPServer
    {

        delegate void ShowMessageMethod(string msg);

        UdpClient _server = null;
        IPEndPoint _client = null;
        Thread _listenThread = null;
        private bool _isServerStarted = false;
        public string receivedMessage { get; private set; }

        public UDPServer()
        {
        }

        public void Start()
        {
            //Create the server.
            IPEndPoint serverEnd = new IPEndPoint(IPAddress.Any, 1111);

            if (_server == null)
            {
                _server = new UdpClient(serverEnd);
                //Create the client end.
                _client = new IPEndPoint(IPAddress.Any, 0);
            }
            System.Windows.Forms.MessageBox.Show("Waiting for a client...");


            //Start listening.
            Thread listenThread = new Thread(new ThreadStart(Listening));
            listenThread.Start();
            //Change state to indicate the server starts.
            _isServerStarted = true;
        }

        public void Stop()
        {
                //Stop listening.
                _listenThread.Join();
                System.Windows.Forms.MessageBox.Show("Server stops");
                _server.Close();
                //Changet state to indicate the server stops.
                _isServerStarted = false;

        }

        private void Listening()
        {
            byte[] data;
            //Listening loop.
            while (true)
            {
                //receieve a message form a client.
                data = _server.Receive(ref _client);
                receivedMessage = Encoding.ASCII.GetString(data, 0, data.Length);
                //Send a response message.
                //data = Encoding.ASCII.GetBytes("Server:" + receivedMsg);
                //_server.Send(data, data.Length, _client);
                //Sleep for UI to work.
                //Thread.Sleep(500);
            }
        }
    }
}
