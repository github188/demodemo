package org.notebook.gui;

import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.LinkedList;

import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTabbedPane;
import javax.swing.JTable;
import javax.swing.SwingUtilities;
import javax.swing.table.AbstractTableModel;

import org.notebook.cache.Category;
import org.notebook.services.SyncListener;
import org.notebook.services.SyncService;
import org.notebook.services.SyncTask;

public class SyncStatusDailog extends JDialog implements SyncListener{
	private SyncService service = null;
	private StatusModel doneList = null;
	private StatusModel paddingList = null;
	private StatusModel dataList = null;
	public SyncStatusDailog(JFrame parent, SyncService service){
		super(parent, true);
		this.service = service;
		this.service.addListener(this);
		
		this.setTitle("更新状态...");
		setContentPane(createSettingJPanel());
		this.setPreferredSize(new Dimension(620, 180));
		this.pack();
		this.setResizable(true);
	}
	
	private JPanel createSettingJPanel(){
		JPanel p = new JPanel(new BorderLayout());
		JTabbedPane textControlsPane = new JTabbedPane();
		
		doneList = new StatusModel(this.service.doneTask);
		paddingList = new StatusModel(this.service.taskQueue);
		dataList = new StatusModel(this.service.dataQueue);
		
		JTable doneTable = new StatusTable(paddingList);
		JScrollPane jsp = new JScrollPane(doneTable);		
		textControlsPane.addTab("更新状态", jsp);
		
		JTable dataTable = new StatusTable(dataList);
		JScrollPane jsp3 = new JScrollPane(dataTable);		
		textControlsPane.addTab("更新数据", jsp3);		
		
		JTable taskTable = new StatusTable(doneList);
		JScrollPane jsp2 = new JScrollPane(taskTable);
		textControlsPane.addTab("完成更新", jsp2);
		
        JPanel buttons = new JPanel();        
        JButton close = new JButton("关闭");
        final SyncStatusDailog dailog = this;
        close.addActionListener(new ActionListener(){
			@Override
			public void actionPerformed(ActionEvent e) {
				dailog.setVisible(false);
				service.removeListener(dailog);
				dailog.dispose();
			}
		});                
        buttons.add(close);        
        p.add(textControlsPane, BorderLayout.CENTER);
        p.add(buttons, BorderLayout.SOUTH);
                        
		return p;
	}
	
	class StatusTable extends JTable{
		public StatusTable(StatusModel model){
			super(model);
			this.setGUI();
		}
		
		private void setGUI(){
			this.getColumnModel().getColumn(0).setPreferredWidth(80);
			this.getColumnModel().getColumn(5).setPreferredWidth(80);		
		}
	}
	
	class StatusModel extends AbstractTableModel{
		public LinkedList<SyncTask> data = null;
		protected DateFormat format= new SimpleDateFormat("yyyy-MM-dd hh:mm:ss");
		public StatusModel(LinkedList<SyncTask> data){
			this.data = data;			
		}
		String[] columns = new String[]{"任务", "本地节点", "远程节点", "本地时间", "远程时间", "状态", "错误信息"};
		
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
			SyncTask task = data.get(rowIndex);
			switch(columnIndex){
				case 0: return task.task;
				case 1: return !task.newCreated && task.local != null ? task.local.name : "";
				case 2: return task.remote != null ? task.remote.name : "";
				case 3: return !task.newCreated && task.local != null && task.local.lastUpdated != null ? format.format(task.local.lastUpdated) : "";
				case 4: return task.remote != null && task.remote.lastUpdated != null ? format.format(task.remote.lastUpdated) : "";
				case 5: return task.status + "(" + task.cause + ")";	
				case 6: return task.exception != null? task.exception.toString() : "";	
			}
			return "";
		}
	}

	@Override
	public void start(SyncTask task) {
		this.paddingList.fireTableDataChanged();
		this.doneList.fireTableDataChanged();
		this.dataList.fireTableDataChanged();
	}

	@Override
	public void done(SyncTask task) {
		this.paddingList.fireTableDataChanged();
		this.doneList.fireTableDataChanged();
		this.dataList.fireTableDataChanged();
	}

	@Override
	public void syncError(SyncTask task, Exception e) {
	}

	@Override
	public int conflict(Category local, Category remote, int cause) {
		return 0;
	}

	@Override
	public void waiting() {
		// TODO Auto-generated method stub
		
	}
}
