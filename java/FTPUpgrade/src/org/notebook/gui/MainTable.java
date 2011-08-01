package org.notebook.gui;

import java.awt.Color;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.ArrayList;

import javax.swing.JTable;
import javax.swing.table.AbstractTableModel;

import org.notebook.cache.UpgradeModel;

public class MainTable extends JTable {
	
	public MainTable(){
		super(new TableModel());
		this.setGUI();
		

	}
		
	private void setGUI(){
		this.getColumnModel().getColumn(0).setPreferredWidth(80);
		this.getColumnModel().getColumn(5).setPreferredWidth(80);
		//this.setBackground(Color.RED);		
	}
	
	public static class TableModel extends AbstractTableModel{
		public ArrayList<UpgradeModel> data = new ArrayList<UpgradeModel>();
		protected DateFormat format= new SimpleDateFormat("yyyy-MM-dd hh:mm:ss");
		public TableModel(){			
		}
		String[] columns = new String[]{"本地文件", "大小", "FTP文件", "大小", "是否更新", "更新时间"};
		
		public String getColumnName(int column){
			 return columns[column];
		}
		
		public ArrayList<UpgradeModel> getData(){
			ArrayList<UpgradeModel> tmp = new ArrayList<UpgradeModel>();
			tmp.addAll(data);
			return tmp;
		}
		
		public void addUpgradeModel(UpgradeModel file){
			this.data.add(file);
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
				case 1: return formateSize(task.sourceSize);
				case 2: return task.dst;
				case 3: return formateSize(task.dstSize);
				case 4: return task.isUpdate ? "是": "否";
				case 5: return task.updateDate;
			}
			return "";
		}
		
		private String formateSize(long size){
			if(size == 0)return "";
			double b = size / 1024.0;
			String unit = "Kb";
			if(b > 1024){
				b = b / 1024.0;
				unit = "Mb";
			}			
			return String.format("%s(%1.2f%s)", size, b, unit);
		}
	}

}
