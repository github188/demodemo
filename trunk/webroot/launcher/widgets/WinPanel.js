/**
 * @author James
 */
Sigma.WinPanel = Ext.extend(Ext.Panel, {
    closeable: true,
	hasMaximize:true,
	hasMinimize:false,
    tools: [
	{
        id: 'minimize',
		qtip: 'minimize',
        handler: function(event, toolEl, panel){
            panel._handleEvent('minimize', toolEl);
        }
    },{
        id: 'maximize',
		qtip: 'maximize',
        handler: function(event, toolEl, panel){
            toolEl.hide();
            panel.tools.restore.show();
            panel._handleEvent('maximize', toolEl);
        }
    }, {
        id: 'restore',
		qtip: 'restore',
        hidden: true,
        handler: function(event, toolEl, panel){
            toolEl.hide();
            panel.tools.maximize.show();
            panel._handleEvent('restore', toolEl);
        }
    }, {
        id: 'close',
		qtip:'close',
        handler: function(event, toolEl, panel){
            panel._handleEvent('destroy', toolEl);
            panel.destroy();
        }
    }],
    
    initComponent: function(){
        Sigma.WinPanel.superclass.initComponent.call(this);
        
        this.addEvents(      
		/**
         * @event minimize
         * Fires when the "pressed" state of the minimize button changes
         * @param {Button} this
         * @param {InternalWindow} window
         */
        'minimize',
		/**
         * @event maximize
         * Fires when the "pressed" state of the maximize button changes
         * @param {Button} this
         * @param {InternalWindow} window
         */
        'maximize',
		/**
         * @event restore
         * Fires when the "pressed" state of the restore button changes
         * @param {Button} this
         * @param {InternalWindow} window
         */
        'restore',        
		/**
         * @event destroy
         * Fires when the "pressed" state of the close button changes
         * @param {Button} this
         * @param {InternalWindow} window
         */
        'destroy')
    },
	
	addTool : function(){
        if(!this[this.toolTarget]) { // no where to render tools!
            return;
        }
        if(!this.toolTemplate){
            // initialize the global tool template on first use
            var tt = new Ext.Template(
                 '<div class="x-tool x-tool-{id}">&#160;</div>'
            );
            tt.disableFormats = true;
            tt.compile();
            Ext.Panel.prototype.toolTemplate = tt;
        }
        for(var i = 0, a = arguments, len = a.length; i < len; i++) {
            var tc = a[i], overCls = 'x-tool-'+tc.id+'-over';
			if(this.closeable==false && tc.id == 'close')
				continue;
			if(this.hasMaximize==false && (tc.id == 'maximize' || tc.id == 'restore'))
				continue;
			if(this.hasMinimize==false && tc.id == 'minimize')
				continue;
            var t = this.toolTemplate.insertFirst(this[this.toolTarget], tc, true);
            this.tools[tc.id] = t;
            t.enableDisplayMode('block');
            t.on('click', this.createToolHandler(t, tc, overCls, this));
            if(tc.on){
                t.on(tc.on);
            }
            if(tc.hidden){
                t.hide();
            }
            if(tc.qtip){
                if(typeof tc.qtip == 'object'){
                    Ext.QuickTips.register(Ext.apply({
                          target: t.id
                    }, tc.qtip));
                } else {
                    t.dom.qtip = tc.qtip;
                }
            }
            t.addClassOnOver(overCls);
        }
    },
    
    _handleEvent: function(e, button){
        if (e == 'maximize') 
            this.fireEvent(e, this);
        else if (e == 'restore') 
            this.fireEvent(e, this);
        else if (e == 'destroy') 
            this.fireEvent(e, this);
		else if (e == 'minimize') 
            this.fireEvent(e, this);
    }
    
});
Ext.reg('sigma.winPanel', Sigma.WinPanel);
