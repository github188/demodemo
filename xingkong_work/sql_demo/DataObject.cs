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
		//判断是否已经存在名字。
		string mySelectQuery = "SELECT username FROM PhoneBook where username='" + name + "'";
		OleDbCommand myCommand = new OleDbCommand(mySelectQuery, myAccessConn);
		OleDbDataReader myReader = null;
		myReader = myCommand.ExecuteReader();
		if (myReader.Read()){
			myReader.Close();
			return false;
		}
		
		//如果不存在名字，添加记录。
		string sqlInsert = "insert into PhoneBook(username, phone) values('" + name + "','" + phone + "')";
		myAccessCommand = new OleDbCommand(sqlInsert, myAccessConn);
		myAccessCommand.ExecuteNonQuery();
		return true;
	}

	public bool updateContact(string name, string phone){
		//更新记录到表
		string sqlUpdate = "update PhoneBook set phone='" + phone + "' where username='" + name + "'";
		Console.WriteLine(sqlUpdate);
		myAccessCommand = new OleDbCommand(sqlUpdate, myAccessConn);
		myAccessCommand.ExecuteNonQuery();
		return true;
	}

	public bool removeContact(string name){
		//删除记录到表
		string sqlRemove = "delete from PhoneBook where username='" + name + "'";
		myAccessCommand = new OleDbCommand(sqlRemove, myAccessConn);
		myAccessCommand.ExecuteNonQuery();
		return true;
	}

	public ArrayList listContact(){
		//返回所有电话记录。

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

	public void printPhookBook(){
		Console.WriteLine("======================");
		foreach (string[] item in listContact())
		{
			Console.WriteLine("{0}={1}", item[0], item[1]);
		}
	}
	

}