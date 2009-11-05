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
 
            // Create the dataset and add the Categories table to it:
            DataSet myDataSet = new DataSet();
            OleDbConnection myAccessConn = null;
			OleDbCommand myAccessCommand = null;
            try
            {
                  myAccessConn = new OleDbConnection(strAccessConn);
            }
            catch(Exception ex)
            {
                  Console.WriteLine("Error: Failed to create a database connection. \n{0}", ex.Message);
                  return;
            }
			
			//执行创建表的SQL
			try
			{
				string strCreate = "CREATE TABLE Categories(cate_name char(255), cate_value char(255))";
				myAccessCommand = new OleDbCommand(strCreate, myAccessConn);
				myAccessConn.Open();
				myAccessCommand.ExecuteNonQuery();
			}catch (Exception ex)
            {
                  Console.WriteLine("Error: Failed to retrieve the required data from the DataBase.\n{0}", ex.Message);
            }
			
			//插入记录到表
			string sqlInsert = "insert into Categories(cate_name, cate_value) values('test1','test value')";
			myAccessCommand = new OleDbCommand(sqlInsert, myAccessConn);
			myAccessCommand.ExecuteNonQuery();
			
			//查询表的记录
			string strAccessSelect = "SELECT * FROM Categories";
            try
            {
                  myAccessCommand = new OleDbCommand(strAccessSelect,myAccessConn);
                  OleDbDataAdapter myDataAdapter = new OleDbDataAdapter(myAccessCommand);
                 // myAccessConn.Open();
                  myDataAdapter.Fill(myDataSet,"Categories");
            }
            catch (Exception ex)
            {
                  Console.WriteLine("Error: Failed to retrieve the required data from the DataBase.\n{0}", ex.Message);
                  return;
            }
            finally
            {
                  myAccessConn.Close();
            }
 
            DataTableCollection dta = myDataSet.Tables;
            foreach (DataTable dt in dta)
            {
            Console.WriteLine("Found data table {0}", dt.TableName);
            }
          
            Console.WriteLine("{0} tables in data set", myDataSet.Tables.Count);
            Console.WriteLine("{0} tables in data set", dta.Count);
            Console.WriteLine("{0} rows in Categories table", myDataSet.Tables["Categories"].Rows.Count);
            Console.WriteLine("{0} columns in Categories table", myDataSet.Tables["Categories"].Columns.Count);
            DataColumnCollection drc = myDataSet.Tables["Categories"].Columns;
            int i = 0;
            foreach (DataColumn dc in drc)
            {
                  Console.WriteLine("Column name[{0}] is {1}, of type {2}",i++ , dc.ColumnName, dc.DataType);
            }

			//输出表的内容
            DataRowCollection dra = myDataSet.Tables["Categories"].Rows;
            foreach (DataRow dr in dra)
            {
                  Console.WriteLine("CategoryName[{0}] is {1}", dr[0], dr[1]);
            }
      
   }
}