//Demo widgets class.

Ext.app.DemoList = function(cfg){
    //Ext.apply(this, config);
    Ext.tree.TreePanel.superclass.constructor.call(this, {
        autoScroll: true,
        animate: true,
        enableDD: false,
        containerScroll: true,
        init: Ext.emptyFn,
        rootVisible: false,
        title: 'Demo List',
        region: 'west',
        margins: '5',
        cmargins: '5 5 0 5',
        width: 200,
        minSize: 100,
        maxSize: 300
    });
    
    cfg.loader = new Ext.tree.TreeLoader({
        dataUrl: '/demo-list.cgi',
        preloadChildren: true
    });
    Ext.apply(this, cfg);
    this.init();
    var root = new Ext.tree.AsyncTreeNode({
        text: 'Groups',
        draggable: false,
        id: 'group'
    })
    this.setRootNode(root);
    /*
     //addListener( String eventName, Function handler, [Object scope], [Object options] ) : void
     this.addListener('click',
     function(node, evt){
     alert(node);
     }, this);
     */
};

Ext.extend(Ext.app.DemoList, Ext.tree.TreePanel, {});

/*
 *Demo brief panel, display into right of demo list windows.
 *   __________
 *  |          |
 *  |Topology  |
 *  |          |
 *  |----------|
 *  |Brief     |
 *  |__________|
 */
Ext.app.DemoBriefPanel = function(cfg){
    var brief = this;
    
    var desc = new Ext.Panel({
        id: 'desc_panel',
        title: 'Demo Description',
        region: 'south',
        height: 200,
        minSize: 150,
        maxSize: 450,
        margins: '0 5 5 5',
        split: true,
        buttons: [{
            text: "Next",
            handler: function(b, e1){
                brief.clickNextButton(brief)
            }
        }],
        
        setText: function(text){
            Ext.DomHelper.overwrite(this.body, {
                html: text
            });
        }
    });
    /*
     var topology = new Ext.Panel({
     //title: 'Main Content',
     region:'center',
     margins: '5',
     collapsible:true,
     setTopo: function(url){
     Ext.DomHelper.overwrite(this.body, {tag:'img', src: url});
     }
     });
     */
    var topology = new Sigma.ImageView({
        id: 'top_panel',
        region: 'center',
        closeable: false,
		margins: '5 5 0 5',
		border:false,
        title: 'Topology'
    });
    
    topology.on("maximize", this.maxTopology);
    topology.on("restore", this.restoreTopology);
    
    Ext.Panel.superclass.constructor.call(this, {
        layout: 'border',
        split: true,
        desc: desc,
        topology: topology,
        margins: '5',
        items: [topology, desc]
    });
    
    Ext.apply(this, cfg);
}

Ext.extend(Ext.app.DemoBriefPanel, Ext.Panel, {
    loadDemo: function(d){
        var beief = this;
        if (d.isLoading()) {
            d.on('load', function(dd){
                beief.loadDemo(dd);
            });
        }
        else {
            if (d.error) {
                this.disable();
                alert(d.error);
            }
            else {
                this.enable();
                this.topology.setImage(d.topoURL());
                this.desc.setText(d.description);
                this.currentDemo = d;
            }
        }
    },
    maxTopology: function(panel){
        var demoTree = this.ownerCt.ownerCt.getComponent("demoTree");
        this.demoListWidth = demoTree.getSize().width;
        demoTree.setVisible(false);
        demoTree.setWidth(0);
        
        var desc_panel = this.ownerCt.getComponent("desc_panel");
        this.desc_panel_height = desc_panel.getSize().height;
        desc_panel.setHeight(0);
        desc_panel.setVisible(false);
        this.ownerCt.ownerCt.syncSize();
    },
    
    restoreTopology: function(panel){
        var demoTree = this.ownerCt.ownerCt.getComponent("demoTree");
        if (this.demoListWidth) {
            demoTree.setWidth(this.demoListWidth);
        }
        else {
            demoTree.setWidth(200);
        }
        demoTree.setVisible(true);
        var desc_panel = this.ownerCt.getComponent("desc_panel");
        desc_panel.setVisible(true);
        if (this.desc_panel_height) 
            desc_panel.setHeight(this.desc_panel_height);
        else 
            desc_panel.setHeight(200);
        this.ownerCt.ownerCt.syncSize();
    },
    clickNextButton: function(ctrl){
        if (this.currentDemo) {
            var win = Ext.WindowMgr.get("demo-window-" + this.currentDemo.name);
            if (win) {
                win.show();
            }
            else {
                new Ext.app.DemoWindow({
                    id: "demo-window-" + this.currentDemo.name,
                    demo: this.currentDemo,
                    title: this.currentDemo.name + ' --- Sigma supported',
                    parent: ctrl.ownerCt
                }).show();
		var demo = this.currentDemo;
		this.currentDemo.lockDemo(function (d, o){
			console.log("lock demo:" + o);
			if(o.data){
				demo.startDemo(function (sd, so){
					if(so.error){
						console.log("start demo:" + so);
						alert(so.error);
					}
				});
			} else {
				alert(o.error);
			}
		});
            }
            ctrl.ownerCt.hide();
        }
    }
});

Ext.app.TopologyPanel = Ext.extend(Sigma.ResizePanel, {});


Ext.app.TextPanel = Ext.extend(Sigma.WinPanel, {
    margins: '5',
    collapsible: true,
    setText: function(text){
        if (this.rendered) {
            Ext.DomHelper.overwrite(this.body, {
                html: text
            });
        }
        else { //fix bug,
            this.on('render', function(panel){
                panel.setText(text);
            });
        }
    }
});

Ext.app.DemoWindow = function(cfg){

    var fourPanel = new Ext.app.DemoFourPanel({
        id: 'four_panel'
    });
    var actionPanel = new Ext.app.ActionPanel({
        id: 'action_panel'
    });
    
    Ext.Panel.superclass.constructor.call(this, {
        width: 913,
        height: 758,
        iconCls: 'accordion',
        shim: false,
        animCollapse: false,
        constrainHeader: true,
        layout: 'border',
        maximizable: true,
        minimizable: true,
        collapsible: true,
        items: [fourPanel, actionPanel]
    });
    
    Ext.apply(this, cfg);
    
    fourPanel.initDemo(cfg.demo);
    actionPanel.initDemo(cfg.demo);
    this.on("destroy ", this.destroy)
}

Ext.extend(Ext.app.DemoWindow, Ext.Window, {
    destroy: function(owner){
        this.parent.show();
    },
});


Ext.app.ActionPanel = function(cfg){

    Ext.TabPanel.superclass.constructor.call(this, {
        title: 'Demo Actions',
        region: 'south',
        height: 200,
        minSize: 150,
        maxSize: 350,
        margins: '0 5 5 5',
        split: true,
        buttons: [{
            text: "Run"
        }]
    });
    
    Ext.apply(this, cfg);
};

Ext.extend(Ext.app.ActionPanel, Ext.TabPanel, {
    initDemo: function(d){
        this.demo = d;
        //clear the tabs.
        this.items.clear();
        //Init demo Actions.
        for (var i = 0; i < d.steps.length; i++) {
            this.add(new Ext.Panel({
                title: d.steps[i].name,
                html: d.steps[i].description
            }));
        }
        this.activate(this.items.first());
    }
});

Ext.app.DemoFourPanel = function(cfg){
    var topologyPanel = new Ext.app.TopologyPanel({
        id: 'dm-topo',
        title: 'Topology',
        closeable: false
    });
    
    
    var descriptionPanel = new Ext.app.TextPanel({
        id: 'dm-text',
        title: 'Description',
        closeable: false,
        collapsible: false
    });
    
	var graphsPanel = new Sigma.GraphTabPanel({
		id:'dm-graphs',
		title:'Graphs',
		closeable: false,
        collapsible: false
	});
	
	var consolesPanel = new Sigma.ConsoleTabPanel({
		id:'dm-console',
		title:'Consoles',
		closeable: false,
        collapsible: false
	});
    
    Ext.Panel.superclass.constructor.call(this, {
        margins: '5',
        region: 'center',
        layout: 'table',
        collapsible: true,
        border: true,
        layoutConfig: {
            // The total column count must be specified here
            columns: 2
        },
        items: [topologyPanel, descriptionPanel, graphsPanel, consolesPanel]
    });
    
    Ext.apply(this, cfg);
    topologyPanel.on("maximize", function(panel){
        panel.ownerCt.maxPanel(panel.ownerCt, panel);
    });
    topologyPanel.on("restore", function(panel){
        panel.ownerCt.restorePanel(panel.ownerCt);
    });
    descriptionPanel.on("maximize", function(panel){
        panel.ownerCt.maxPanel(panel.ownerCt, panel);
    });
    descriptionPanel.on("restore", function(panel){
        panel.ownerCt.restorePanel(panel.ownerCt);
    });
	graphsPanel.on("maximize", function(panel){
        panel.ownerCt.maxPanel(panel.ownerCt, panel);
    });
	graphsPanel.on("restore", function(panel){
        panel.ownerCt.restorePanel(panel.ownerCt);
    });
	consolesPanel.on("maximize", function(panel){
        panel.ownerCt.maxPanel(panel.ownerCt, panel);
    });
	consolesPanel.on("restore", function(panel){
        panel.ownerCt.restorePanel(panel.ownerCt);
    });
    this.on('resize', this._onResize);
    this.on('afterlayout', this._onResize);
};

Ext.extend(Ext.app.DemoFourPanel, Ext.Panel, {
    initDemo: function(d){
        this.findById('dm-topo').setImage(d.topoURL());
        this.findById('dm-text').setText(d.description);
        this.findById('dm-graphs').addGraphs(d);
		this.findById('dm-console').addConsoles(d);
    },
    _onResize: function(container){
        if (this.maxed && this.maxed == true && this.maxedPanel) {
            this.maxedPanel.setHeight(container.getInnerHeight());
            this.maxedPanel.setWidth(container.getInnerWidth());
        }
        else {
            container.items.each(function(item, index, length){
                item.setVisible(true);
                var height = container.getInnerHeight() / (length / container.layoutConfig.columns) - 2;
                var width = container.getInnerWidth() / container.layoutConfig.columns - 2;
                item.setHeight(height);
                item.setWidth(width);
            });
        }
    },
    
    restorePanel: function(container){
        var action_panel = this.ownerCt.getComponent("action_panel");
        action_panel.setVisible(true);
        if (this.action_panel_height) 
            action_panel.setHeight(this.action_panel_height);
        else 
            action_panel.setHeight(200);
        this.maxed = false;
        container.ownerCt.syncSize();
    },
    
    maxPanel: function(container, panel){
        var action_panel = this.ownerCt.getComponent("action_panel");
        this.action_panel_height = action_panel.getSize().height;
        action_panel.setVisible(false);
        action_panel.setHeight(0);
        container.ownerCt.syncSize();
        this.maxedPanel = panel;
        this.maxed = true;
        container.items.each(function(item, index, length){
            if (item != panel) {
                item.setVisible(false);
                item.setHeight(0);
                item.setWidth(0);
            }
            else {
                item.setVisible(true);
                item.setHeight(container.getInnerHeight());
                item.setWidth(container.getInnerWidth());
            }
        });
    }
});
