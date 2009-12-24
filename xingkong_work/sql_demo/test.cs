// OleDbSample.cs
using System;
using System.Data;
using System.Data.OleDb;
using System.Xml.Serialization;
using System.Collections;

public class test {
	
    public static void Main (){
		//创建一个电话管理的对象。
		PhoneBookManager manager = new PhoneBookManager("BugTypes.MDB");
		
		bool result = false;

		result = manager.addContact("kgd1", "0825-8662237");
		result = manager.addContact("kgd2", "0825-8662238");
		manager.printPhookBook();
		
		Console.WriteLine("更新kgd2的电话!");
		result = manager.updateContact("kgd2", "0825-8662239");
		Console.WriteLine("updateContact: {0}\n", result);
		manager.printPhookBook();

		Console.WriteLine("删除kgd2的电话!");
		result = manager.removeContact("kgd2");
		Console.WriteLine("removeContact: {0}\n", result);
		manager.printPhookBook();
		
		//关闭数据库连接，这个很重要。
		manager.close();
		Console.WriteLine("done...........");
   }
}