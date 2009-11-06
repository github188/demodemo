// OleDbSample.cs
using System;
using System.Data;
using System.Data.OleDb;
using System.Xml.Serialization;
using System.Collections;

public class PhoneBookManager {
	private string connstr = null;
	private OleDbConnection myAccessConn = null;
	private OleDbCommand myAccessCommand = null;

	public PhoneBookManager(string database){
		connstr = "Provider=Microsoft.Jet.OLEDB.4.0;Data Source=" + database;
		initDataConnnection(connstr);
	}

	public bool addContact(string name, string phone){
		//�ж��Ƿ��Ѿ��������֡�
		string mySelectQuery = "SELECT username FROM PhoneBook where username='" + name + "'";
		OleDbCommand myCommand = new OleDbCommand(mySelectQuery, myAccessConn);
		OleDbDataReader myReader = null;
		myReader = myCommand.ExecuteReader();
		if (myReader.Read()){
			myReader.Close();
			return false;
		}
		
		//������������֣���Ӽ�¼��
		string sqlInsert = "insert into PhoneBook(username, phone) values('" + name + "','" + phone + "')";
		myAccessCommand = new OleDbCommand(sqlInsert, myAccessConn);
		myAccessCommand.ExecuteNonQuery();
		return true;
	}

	public bool updateContact(string name, string phone){
		//���¼�¼����
		string sqlUpdate = "update PhoneBook set phone='" + phone + "' where username='" + name + "'";
		Console.WriteLine(sqlUpdate);
		myAccessCommand = new OleDbCommand(sqlUpdate, myAccessConn);
		myAccessCommand.ExecuteNonQuery();
		return true;
	}

	public bool removeContact(string name){
		//ɾ����¼����
		string sqlRemove = "delete from PhoneBook where username='" + name + "'";
		myAccessCommand = new OleDbCommand(sqlRemove, myAccessConn);
		myAccessCommand.ExecuteNonQuery();
		return true;
	}

	public ArrayList listContact(){
		//�������е绰��¼��

		string mySelectQuery = "SELECT * FROM PhoneBook";
		OleDbCommand myCommand = new OleDbCommand(mySelectQuery, myAccessConn);
		OleDbDataReader myReader = null;
		myReader = myCommand.ExecuteReader();

		ArrayList result = new ArrayList();
		string[] row = null; 
		while (myReader.Read()) {
			row = new string[2];
			row[0] = myReader.GetString(0);
			row[1] = myReader.GetString(1);
			result.Add(row);
		}
		// always call Close when done reading.
		myReader.Close();

		return result;
	}

	public void close(){
		myAccessConn.Close();
	}

	private void initDataConnnection(string conn){
		try{
			myAccessConn = new OleDbConnection(conn);
			myAccessConn.Open();
		}catch(Exception ex){
			Console.WriteLine("Error: Failed to create a database connection. \n{0}", ex.Message);
		}
		try{
			string strCreate = "CREATE TABLE PhoneBook(username char(20), phone char(20))";
			myAccessCommand = new OleDbCommand(strCreate, myAccessConn);
			myAccessCommand.ExecuteNonQuery();
		}catch (Exception ex)
		{
			  Console.WriteLine("Error: Failed to retrieve the required data from the DataBase.\n{0}", ex.Message);
		}
	}

	private void printPhookBook(){
		Console.WriteLine("======================");
		foreach (string[] item in listContact())
		{
			Console.WriteLine("{0}={1}", item[0], item[1]);
		}
	}
	
    public static void Main (){
		//����һ���绰����Ķ���
		PhoneBookManager manager = new PhoneBookManager("BugTypes.MDB");
		
		bool result = false;

		result = manager.addContact("kgd1", "0825-8662237");
		result = manager.addContact("kgd2", "0825-8662238");
		manager.printPhookBook();
		
		Console.WriteLine("����kgd2�ĵ绰!");
		result = manager.updateContact("kgd2", "0825-8662239");
		Console.WriteLine("updateContact: {0}\n", result);
		manager.printPhookBook();

		Console.WriteLine("ɾ��kgd2�ĵ绰!");
		result = manager.removeContact("kgd2");
		Console.WriteLine("removeContact: {0}\n", result);
		manager.printPhookBook();
		
		//�ر����ݿ����ӣ��������Ҫ��
		manager.close();
		Console.WriteLine("done...........");
   }
}