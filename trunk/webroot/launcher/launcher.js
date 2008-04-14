/*
 * Ext JS Library 2.0.1
 * Copyright(c) 2006-2008, Ext JS, LLC.
 * licensing@extjs.com
 * 
 * http://extjs.com/license
 */


// Sample desktop configuration
MyDesktop = new Ext.app.App({
	init :function(){
		Ext.QuickTips.init();
    var module = this.getModule('demo-launcher');
    if(module){
        module.createWindow();
    }
	},

	getModules : function(){
		return [
      new MyDesktop.DemoLauncher()
		];
	},

    // config for the start menu
    getStartConfig : function(){
        return {
            title: 'Jack Slocum',
            iconCls: 'user',
            toolItems: [{
                text:'Settings',
                iconCls:'settings',
                scope:this
            },'-',{
                text:'Logout',
                iconCls:'logout',
                scope:this
            }]
        };
    }
});

MyDesktop.DemoLauncher = Ext.extend(Ext.app.Module, {
    id:'demo-launcher',
    init : function(){
        this.launcher = {
            text: 'Accordion Window',
            iconCls:'accordion',
            handler : this.createWindow,
            scope: this
        }
    },

    createWindow : function(){
        var desktop = this.app.getDesktop();
        var win = desktop.getWindow('demo-launcher');
        if(!win){
            demoList = new Ext.app.DemoList({id:'demoTree',
                      region:'west',
                      title: 'Demo List',
                      margins: '5'                    
                      });
            briefPanel = new Ext.app.DemoBriefPanel({
                      region:'center',
                      margins: '5'
                      });

           demoList.on('click', function(node, e){
             if(node.isLeaf()){
                e.stopEvent();
                if(!node.DemoClient){
                  node.DemoClient = new Ext.app.DemoClient({
                       demoId: node.id,      
                       });
                  //Test graph data.
                  node.DemoClient.on('graph',function(d, evt){
                  	console.log("graph update, [series=" + evt.get('series') + ",xData=" + evt.get('time') + ",yData=" + evt.get('data') + "]");
                  });
                }                
                briefPanel.loadDemo(node.DemoClient);
             }
            });   
                                
            win = desktop.createWindow({
                id: 'demo-launcher',
                title: 'DemoLanucher',
                width:913,
                height:758,
                iconCls: 'accordion',
                shim:false,
                animCollapse:false,
                constrainHeader:false,
                layout:'border',
                border:false,
                layoutConfig: {
                    animate:false
                },
                items: [{
					id:'app_title',
                    region:'north',
					html:'<div>dddddd</div>',
                    height:32
                }
				,
                  demoList,
                  briefPanel
                ]});
            //demo_list.render();
            //root.expand();                
        }
        win.show();
    }
});

