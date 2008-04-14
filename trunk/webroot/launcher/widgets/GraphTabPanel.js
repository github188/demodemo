/**
 * @author James
 */
Sigma.GraphTabPanel = function(cfg) {
	// this.tabPanel = new Ext.TabPanel({
	// region:'center'
	// });
	Sigma.WinPanel.superclass.constructor.call(this, {
		title : ' ',
		layout : 'border',
		// items:this.tabPanel
		items : [{
			id : 'tab_graphs',
			region:'center',
			html : '<applet codebase="." code="com.sigma.applet.GraphsApplet.class" archive="/applets/graphsApplet.jar,/applets/jcommon-1.0.12.jar,/applets/jfreechart-1.0.9.jar" width="100%" height="100%"></applet>'
		}]
	});
	this.on("tabchange", this.onTabChange);
	Ext.apply(this, cfg);
};
Ext.extend(Sigma.GraphTabPanel, Sigma.WinPanel, {
	/**
	 * @param graph
	 * 
	 * "series":"series1,series2", "title":"M7iDC-A Interfaces",
	 * "graphStyle":{"name":"gsa"}, "name":"graph1",
	 * "type":"realtime.graph.type" "serieses":[{ "yLabel":"yLabel",
	 * "OID":"a.b.c.d.e.f.g1", "xLabel":"xLabel", "XLabel":"xLabel",
	 * "community":"public", "yMax":100, "YLabel":"yLabel", "type":"snmp",
	 * "ip":"a1.b1.c1.d1", "poll":10, "color":"blue",
	 * "description":"ge-0\/0\/2.0", "name":"series1", "YMax":100 },{
	 * "yLabel":"yLabel", "OID":"a.b.c.d.e.f.g2", "xLabel":"xLabel",
	 * "XLabel":"xLabel", "community":"public", "yMax":100, "YLabel":"yLabel",
	 * "type":"snmp", "ip":"a1.b1.c1.d1", "poll":10, "color":"orange",
	 * "description":"ge-0\/0\/3.0", "name":"series2", "YMax":100 }]
	 */
	_addRealtimeGraph : function(graph) {
		// chart = new Sigma.ChartPanel({
		// id:'chart_panel_' + graph.name,
		// region:'center'
		// });
		// chart.addSerieses(graph.serieses);
		// this.tabPanel.add({
		// id:graph.name,
		// title:graph.title,
		// layout:'border',
		// items:chart
		// });
		// chart.show();
	},

	onTabChange : function(tabPanel, tabItem) {
		// chart = tabItem.findById('chart_panel_' + gname)
		// if(chart){
		// chart.drawChart();
		// }
	},

	/**
	 * @param graph
	 * 
	 * "picture":"http:\/\/127.0.0.1\/pic?node=abc&type=his", "title":"his",
	 * "name":"his", "type":"history.graph.type"
	 */
	_addHistoryGraph : function(graph) {
		// this.tabPanel.add({
		// id:graph.name,
		// title:graph.title,
		// html:graph.picture
		// });
	},

	/**
	 * @param graph
	 */
	addGraph : function(graph) {
		if (graph.type == GRAPH_TYPE_REALTIME) {
			this._addRealtimeGraph(graph);
		} else {
			this._addHistoryGraph(graph);
		}
	},

	addGraphs : function(demo) {
		var graphs = demo.getGraphs();
		for (var i = 0; i < graphs.length; i++) {
			this.addGraph(graphs[i]);
		}
		this._activeItem(0);
		// var tp = this;
		// demo.on('graph',function(d, evt){
		// var sname = evt.get('series');
		// var data = evt.get('data');
		// var time = evt.get('time');
		//			
		// var gname = d.getGraphNameBySeriesName(sname);
		// var chart = tp.findById('chart_panel_' + gname);
		// if (chart) {
		// chart.putServiesData(sname, data);
		// }
		// var tab = tp.tabPanel.getActiveTab();
		// chart = tab.findById('chart_panel_' + gname)
		// if(chart){
		// chart.drawChart();
		// }
		// });
	},

	activeGraph : function(graph) {
		var tabItem = this.tabPanel.findById(graph);
		if (tabItem)
			this.tabPanel.activate(tabItem);
	},

	_activeItem : function(index) {
		// if(this.tabPanel.items && this.tabPanel.items.length > 0)
		// this.tabPanel.activate(index);
	}
});
Ext.reg('sigma.graphTabPanel', Sigma.GraphTabPanel);
