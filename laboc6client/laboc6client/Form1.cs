using System.Net;
using System.Net.Sockets;
using System.Threading;
using System.Text;

namespace laboc6client
{
    public partial class Form1 : Form
    {
        Socket connection;
        static int port = 8005; // порт сервера
        static string address = "127.0.0.1";
        public Form1()
        {
            InitializeComponent();
            sendButton.Click += AddMyMessage;
            connection = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            IPEndPoint ipPoint = new IPEndPoint(IPAddress.Parse(address), port);
            connection.Connect(ipPoint);
            Thread resiever = new Thread(new ThreadStart(ClientHundler));
            resiever.Start();
            messages.Items.Add("connected");
        }

        private void AddMyMessage(object sender, EventArgs e)
        {
            messages.Items.Add("вы: " + message.Text);
            connection.Send(Encoding.Unicode.GetBytes(message.Text));
            message.Text = "";
        }

        private void AddOtherMessage(string s)
        {
            if (messages.InvokeRequired)
            {
                messages.Invoke(AddOtherMessage, s);
            }
            else
            {
                messages.Items.Add(s);
            }
        }

        public void ClientHundler()
        {
            byte[] colorBytes = new byte[100];
            connection.Receive(colorBytes);
            string color = Encoding.Unicode.GetString(colorBytes);
            if (color[0] == 'r')
            {
                messages.ForeColor = Color.Red;
            }
            else if (color[0] == 'b')
            {
                messages.ForeColor = Color.Blue;
            }
            else if (color[0] == 'y')
            {
                messages.ForeColor = Color.Yellow;
            }
            else if (color[0] == 'g')
            {
                messages.ForeColor = Color.Green;
            }
            else
            {
                messages.ForeColor = Color.Black;
            }
            int mess_size = 0;
            byte[] mes = new byte[1000];
            while (true)
            {
                try
                {
                    mess_size = connection.Receive(mes);
                    AddOtherMessage("получено: " + Encoding.Unicode.GetString(mes));
                }
                catch (SocketException e)
                {
                    break;
                }
            }
        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }

        private void client_closed(object sender, FormClosedEventArgs e)
        {
            connection.Close();
        }
    }
}