// OleDbSample.cs
using System;
using System.Data;
using System.Data.OleDb;
using System.Xml.Serialization;

public class MainClass {
    public static void Main ()
    {
            // Set Access connection and select strings.
            // The path to BugTypes.MDB must be changed if you build 
            // the sample from the command line:
#if USINGPROJECTSYSTEM
            string strAccessConn = "Provider=Microsoft.Jet.OLEDB.4.0;Data Source=..\\..\\BugTypes.MDB";
#else
            string strAccessConn = "Provider=Microsoft.Jet.OLEDB.4.0;Data Source=BugTypes.MDB";
#endif
 
			ReadMyData(strAccessConn);
   }

	public static void ReadMyData(string myConnString) {
	   string mySelectQuery = "SELECT * FROM Categories";
	   OleDbConnection myConnection = new OleDbConnection(myConnString);
	   OleDbCommand myCommand = new OleDbCommand(mySelectQuery,myConnection);
	   myConnection.Open();
	   OleDbDataReader myReader;
	   myReader = myCommand.ExecuteReader();
	   // Always call Read before accessing data.
	   while (myReader.Read()) {
		  Console.WriteLine(myReader.GetString(0) + ", " + myReader.GetString(1));
	   }
	   // always call Close when done reading.
	   myReader.Close();
	   // Close the connection when done with it.
	   myConnection.Close();
	}

}