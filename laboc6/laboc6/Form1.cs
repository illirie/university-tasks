using System.Net;
using System.Net.Sockets;
using System.Threading;
using System.Numerics;
using System.Text;
using System.IO;

namespace laboc6
{
    public partial class server : Form
    {
        Socket sServer;
        List<Socket> waitingList = new List<Socket>();
        List<Socket> activeList = new List<Socket>();
        int counter = 0;
        static int port = 8005;
        Semaphore semaphore;
        List<string> log = new List<string>();
        List<string> server_log = new List<string>();
        Thread clientHundler;
        List<string> colors = new List<string>{ "red", "yellow", "blue", "green", "black" };

        public server()
        {
            InitializeComponent();
            sServer = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            IPEndPoint ipPoint = new IPEndPoint(IPAddress.Parse("127.0.0.1"), port);
            sServer.Bind(ipPoint);
            Thread acceptThread = new Thread(new ThreadStart(ServerHundler));
            acceptThread.Start();
        }

        private void AddToLog(string s)
        {
            if (chatlog.InvokeRequired)
            {
                chatlog.Invoke(AddToLog, s);
            }
            else
            {
                chatlog.Items.Add(s);
            }
        }

        private void ServerHundler()
        {
            AddToLog("сервер запущен.");
            server_log.Add("сервер запущен.");
            sServer.Listen(10);
            semaphore = new Semaphore(2, 2);
            while (true) 
            {
                try
                {
                    waitingList.Add(sServer.Accept());
                    clientHundler = new Thread(new ThreadStart(ClientHundler));
                    clientHundler.Start();
                }
                catch (Exception ex)
                {
                    break;
                }
            }
        }
        private void ClientHundler()
        {
            semaphore.WaitOne();
            int index = counter;
            AddToLog("клиент #" + (counter + 1) + " подключился");
            server_log.Add("клиент #" + (counter + 1) + " подключился");
            counter++;
            StringBuilder builder = new StringBuilder();
            activeList.Add(waitingList[index]);
            activeList[index].Send(Encoding.Unicode.GetBytes(colors[index % 5]));
            int mess_size = 0;
            byte[] mes = new byte[1000];
            if (log.Count > 0)
            {
                for (int i = 0; i < log.Count; i++)
                {
                    activeList[index].Send(Encoding.Unicode.GetBytes(log[i]));
                }
            }
            while (true)
            {
                try
                {
                    mess_size = activeList[index].Receive(mes);
                    log.Add(Encoding.Unicode.GetString(mes, 0, mess_size));
                    server_log.Add("клиент " + (index + 1) + " " + Encoding.Unicode.GetString(mes, 0, mess_size));
                    for (int i = 0; i < counter; i++)
                    {
                        if (i == index)
                        {
                            continue;
                        }
                        if (activeList[i] != null)
                        {
                            activeList[i].Send(mes);
                        }
                    }
                }
                catch (SocketException e)
                {
                    AddToLog("клиент #" + (index + 1) + " отключился.");
                    server_log.Add("клиент #" + (index + 1) + " отключился.");
                    activeList[index].Close();
                    activeList[index] = null;
                    semaphore.Release();
                    break;
                }
            }
        }

        private void server_closed(object sender, FormClosedEventArgs e)
        {
            StreamWriter streamWriter = new StreamWriter("last_log.txt");
            for (int i = 0; i < server_log.Count; i++)
            {
                streamWriter.WriteLine(server_log[i]);
            }
            sServer.Close();
            streamWriter.Close();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            StreamWriter streamWriter = new StreamWriter("current_log.txt");
            for (int i = 0; i < server_log.Count; i++)
            {
                streamWriter.WriteLine(server_log[i]);
            }
            streamWriter.Close();
        }

    }
}