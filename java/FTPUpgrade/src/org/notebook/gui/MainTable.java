package org.notebook.gui;

import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Collection;

import javax.swing.JTable;
import javax.swing.table.AbstractTableModel;

import org.notebook.cache.UpgradeModel;

public class MainTable extends JTable {
	
	public MainTable(StatusModel model){
		super(model);
		this.setGUI();
		
	}
		
	private void setGUI(){
		this.getColumnModel().getColumn(0).setPreferredWidth(80);
		this.getColumnModel().getColumn(5).setPreferredWidth(80);		
	}
	
	class StatusModel extends AbstractTableModel{
		protected DateFormat format= new SimpleDateFormat("yyyy-MM-dd hh:mm:ss");
		protected ArrayList<UpgradeModel> data = new ArrayList<UpgradeModel>();
		public StatusModel(){			
		}
		String[] columns = new String[]{"源文件", "大小", "目标文件", "大小", "是否更新", "更新时间"};
		
		public String getColumnName(int column){
			 return columns[column];
		}

		@Override
		public int getRowCount() {
			return data.size();
		}

		@Override
		public int getColumnCount() {
			return this.columns.length;
		}

		@Override
		public Object getValueAt(int rowIndex, int columnIndex) {
			if(rowIndex >= data.size()) return "";
			UpgradeModel task = data.get(rowIndex);
			switch(columnIndex){
				case 0: return task.source;
				case 1: return task.sourceSize;
				case 2: return task.dst;
				case 3: return task.dstSize;
				case 4: return task.isUpdate + "";
				case 5: return task.updateDate;		
			}
			return "";
		}
	}	

}
