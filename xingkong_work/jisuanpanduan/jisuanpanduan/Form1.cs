using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace jisuanpanduan
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }
        Random ra = new Random();
        Random rb = new Random();
        Random rc = new Random();
        Random rf = new Random();
        int a, b, c, f;
        private void button1_Click(object sender, EventArgs e)
        {
            
            a = ra.Next(100);
            do
                b = rb.Next(100);
            while (a == b);
            do
                c = rc.Next(-100, 100);
            while (c == 0);
            textBox1.Text = a + "";
            textBox2.Text = b + "";
            f = rf.Next(1, 3);
            if (f == 1)
            {
                textBox3.Text = a + b + "";
            }
            else
            {
                textBox3.Text = a + b + c + "";
            }
            //label3.Text = " ";
            radioButton1.Checked = false;
            radioButton2.Checked = false;
            radioButton1.Enabled = true;
            radioButton2.Enabled = true;
            label3.Text = " ";
            button1.Enabled=false;
            //label3.Text = " ";
        }

        private void radioButton1_CheckedChanged(object sender, EventArgs e)
        {
            if (f == 1)
                label3.Text = "判断正确";
            else
                label3.Text = "判断错误";
            button1.Enabled=true;
            radioButton1.Enabled=false;
            radioButton2.Enabled=false;
        }

        private void radioButton2_CheckedChanged(object sender, EventArgs e)
        {
            if (f == 2)
                label3.Text = "判断正确";
            else
                label3.Text = "判断错误";
            button1.Enabled = true;
            radioButton1.Enabled = false;
            radioButton2.Enabled = false;
        }
    }
}