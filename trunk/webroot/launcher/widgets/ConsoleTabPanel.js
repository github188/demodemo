/**
 * @author James
 */
Sigma.ConsoleTabPanel = function(cfg){
	this.tabPanel = new Ext.TabPanel({
		region:'center'
	});
    Sigma.WinPanel.superclass.constructor.call(this,{
		title:' ',
		layout:'border',
		items:this.tabPanel
	});
	Ext.apply(this, cfg);
};
Ext.extend(Sigma.ConsoleTabPanel, Sigma.WinPanel, {
	/**
	 * 
	 * @param {Object} demo
	 * 
		consoles":[{
		"title":"CLI Session - ISG2000-E",
		"args":"",
		"name":"console1",
		"device":"a3.b3.c3.d3",
		"target":"",
		"prompt":"",
		"type":"normal"
	}]
	 */
	addConsoles:function(demo){
		var consoles = demo.consoles;
		for(var i = 0; i < consoles.length; i++){
			this.addConsole(consoles[i]);
		}
		this._activeItem(0);
	},
	
	/**
	 * 
	 * @param {Object} console
	 * 
		"title":"CLI Session - ISG2000-E",
		"args":"",
		"name":"console1",
		"device":"a3.b3.c3.d3",
		"target":"",
		"prompt":"",
		"type":"normal"
	 */
	addConsole:function(console){
		this.tabPanel.add({
			id:console.name,
			title:console.title,
			html:'<applet codebase="." code="com.sigma.applet.TerminalApplet.class" width="100%" height="100%" archive="/applets/consoleApplet.jar,/applets/jta.jar"></applet>'
//			html:'<applet codebase="/cl" code="com.sigma.applet.TerminalApplet.class" width="100%" height="100%"></applet>'
		});
	},
	
	activeConsole:function(console){
		var tabItem = this.tabPanel.findById(console);
		if(tabItem)
			this.tabPanel.activate(tabItem);
	},
	
	_activeItem:function(index){
		if(this.tabPanel.items && this.tabPanel.items.length > 0)
			this.tabPanel.activate(index);
	}
});

Ext.reg('sigma.consoleTabPanel', Sigma.ConsoleTabPanel);
