using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Net;
using System.Net.Sockets;

namespace v1
{
    public partial class Inicio : Form
    {
        Socket server;

        public Inicio()
        {
            InitializeComponent();
            IPAddress direc = IPAddress.Parse("192.168.56.102");
            IPEndPoint ipep = new IPEndPoint(direc, 9070);

            //Creamos el socket 
            server = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            try
            {
                server.Connect(ipep);//Intentamos conectar el socket
            }

            catch (SocketException)
            {
                MessageBox.Show("No se ha podido conectar con el servidor");
                //Si hay excepcion imprimimos error y salimos del programa con return
                //Close() ;
            }
        }

        string usuario;
        string contraseña;
        int edad;




        private void aceptarBtn_Click(object sender, EventArgs e)
        {
            usuario = usuario_tBx.Text;
            contraseña = contraseña_txB.Text;

            string mensaje = "1/" + usuario + "/" + contraseña + "/";
            byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
            server.Send(msg);

            byte[] msg2 = new byte[80];
            server.Receive(msg2);
            mensaje = Encoding.ASCII.GetString(msg2).Split('\0')[0];

            if (mensaje == "1")
            {
                MessageBox.Show("Usuario y contraseña correctos");
            }
            else
                MessageBox.Show("Los datos introducidos no son los correctos");

        }


        private void enviar_Btn_Click(object sender, EventArgs e)
        {
            try
            {
                usuario = usuarioRe_TB.Text;
                contraseña = contraseñaRe_TB.Text;
                edad = Convert.ToInt32(edadRe_TB.Text);
            }

            catch (FormatException)
            {
                MessageBox.Show("Error de formato");
            }


            string mensaje = "2/" + usuario + "/" + contraseña + "/" + edad;
            byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
            server.Send(msg);

            byte[] msg2 = new byte[80];
            server.Receive(msg2);
            mensaje = Encoding.ASCII.GetString(msg2).Split('\0')[0];

            if (mensaje == "1")
            {
                MessageBox.Show("Usuario registrado correctamente");
            }
            else if (mensaje =="0")
                MessageBox.Show("Estos datos ya estan registrados");
        }

        private void query_Click(object sender, EventArgs e)
        {
            int id_1;
            int id_2;

            try
            {
                if (NombresPartida.Checked)
                    id_1 = Convert.ToInt32(id1.Text);
                else if (partida.Checked)
                {
                    id_2 = Convert.ToInt32(id2.Text);
                }

            }
            catch
            {
                MessageBox.Show("Error de formato");
            }

            if (NombresPartida.Checked)
            {
                string mensaje = "3/" + id1.Text;
                byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
                server.Send(msg);

                byte[] msg2 = new byte[80];
                server.Receive(msg2);
                mensaje = Encoding.ASCII.GetString(msg2).Split('\0')[0];
                if (mensaje =="0")
                {
                    MessageBox.Show("No existe esta partida");
                }
                else
                    MessageBox.Show( mensaje);

            }
            else if (NombresGanadores.Checked)
            {
                string mensaje = "4/";
                byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
                server.Send(msg);

                byte[] msg2 = new byte[80];
                server.Receive(msg2);
                mensaje = Encoding.ASCII.GetString(msg2).Split('\0')[0];
                MessageBox.Show( mensaje);
            }
            else
            {
                string mensaje = "5/" + id2.Text;
                byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
                server.Send(msg);

                byte[] msg2 = new byte[80];
                server.Receive(msg2);
                mensaje = Encoding.ASCII.GetString(msg2).Split('\0')[0];
                MessageBox.Show(mensaje);
  
            }
        }


        private void Inicio_FormClosing(object sender, FormClosingEventArgs e)
        {

            string mensaje = "0/";
            byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
            server.Send(msg);

            server.Shutdown(SocketShutdown.Both);
            server.Close();
        }
    }
}
