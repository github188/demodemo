// OleDbSample.cs
using System;
using System.Data;
using System.Data.OleDb;
using System.Xml.Serialization;
using System.Collections;

public class test {
	
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