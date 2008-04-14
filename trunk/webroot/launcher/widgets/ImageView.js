/**
 * @author James
 */
Sigma.ImageView=function(cfg){
	Ext.Panel.superclass.constructor.call(this,{
		title:' ',
		isMax:false,
		_westWidth:200,
		isCenter:true,
		collapsible: false,
		layout:'border',
	    tools: [{
	        id: 'plus',
	        qtip: 'Zoom In',
			hidden :true,
	        handler: function(event, toolEl, panel){
				if (panel.resizable) {
					var el = panel.resizable.getEl();
					var w = el.getWidth();
					var h = el.getHeight();
					el.setWidth(w * 1.2);
					el.setHeight(h * 1.2);
					panel.alignCenter();
				}
	        }
	    }, {
	        id: 'minus',
	        qtip: 'Zoom Out',
			hidden :true,
	        handler: function(event, toolEl, panel){
				if (panel.resizable) {
					var el = panel.resizable.getEl();
					var w = el.getWidth();
					var h = el.getHeight();
					el.setWidth(w / 1.2);
					el.setHeight(h / 1.2);
					panel.alignCenter();
				}
	        }
	    }, {
	        id: 'refresh',
	        qtip: 'Normal Zoom',
			hidden :true,
	        handler: function(event, toolEl, panel){
				if (panel.resizable) {
					var el = panel.resizable.getEl();
					el.setX(0);
					el.setY(panel.oldTop);
					panel.resizable.resizeTo(panel.oldWidth, panel.oldHeight);
					panel.alignCenter();
				}
	        }
	    }
		, {
	        id: 'gear',
	        qtip: 'Full Zoom',
			hidden :true,
	        handler: function(event, toolEl, panel){
				if (panel.resizable) {
					panel.maxRoom();
					panel.alignCenter();
				}
	        }
	    },{
        id: 'maximize',
		qtip: 'maximize',
        handler: function(event, toolEl, panel){
            toolEl.hide();
            panel.tools.restore.show();
            panel._handleEvent('maximize', panel);
        }
    }, {
        id: 'restore',
		qtip: 'restore',
        hidden: true,
        handler: function(event, toolEl, panel){
            toolEl.hide();
            panel.tools.maximize.show();
            panel._handleEvent('restore', panel);
        }
    }, {
        id: 'close',
		qtip:'close',
        handler: function(event, toolEl, panel){
            panel._handleEvent('destroy', panel);
            panel.destroy();
        }
    }],
	    items:[{
			id:'v_west',
			region:'west',
			border:true,
			width:200
		},{
			id:'v_center',
			border:true,
			region:'center'
		}]
	});
	Ext.apply(this, cfg);
}
Ext.extend(Sigma.ImageView,Ext.Panel, {
	alignCenter: function(){
        var x = this.getInnerWidth()-this._westWidth-5;
        var y = this.getInnerHeight();
		var el = this.resizable.getEl();
		var rx = el.getWidth();
		var ry = el.getHeight();
		px = (x - rx)/2;
		py = (y - ry)/2;
			
        el.setLeft(px);
		el.setTop(py);
    },
	
	maxRoom:function()
	{
		 var x = this.getInnerWidth();
        var y = this.getInnerHeight();
		var _x = this.oldWidth;
		var _y = this.oldHeight;
		var ratioX = _x/x;
		var ratioY = _y/y;
		percentage = ratioX > ratioY ? ratioX : ratioY;
		x = _x / percentage - 1;
		y = _y / percentage;
		var el = this.resizable.getEl();
		el.setX(0);
		el.setY(this.oldTop);
		this.resizable.resizeTo(x,y);
	},
	
	setImage: function(url){
          if(this.rendered){
		  	this._setResizeImage(url);
		  	this._setViewImage(url);
          }else { //fix bug, when the Panel is lazy loading, can't show image.
            this.on('render', function(panel){
              panel.setImage(url);
            });
          }
        },
		
	_setResizeImage:function(url)
	{
		var v_center = this.findById('v_center');			
		if (v_center.rendered) {
			Ext.DomHelper.append(v_center.body, {
				tag: 'img',
				id: 'r_center',
				style: 'cursor:move;position:relative;left:0;top:0;',
				src: url
			});
				this._redraw();
		}else {
            v_center.on('render', function(v_center){
				v_center.ownerCt._setResizeImage(url);	             
            });
		}
	},
	_setViewImage:function(url)
	{
		var v_west = this.findById('v_west');
		if(v_west.rendered){
			Ext.DomHelper.append(v_west.body,'<table height="100%"><tr ><td align="center"><img width="'+this._westWidth+'" src="'+url+'"></td></tr></table>'
			);
		}else{
			v_west.on('render', function(v_west){
				v_west.ownerCt._setViewImage(url);	             
        	});
		}
	},

	_redraw: function(){
		 this.resizable = new Ext.Resizable('r_center', {
            wrap:false,
            pinned:true,
            preserveRatio: true,
            handles: 'all',
            draggable:true,
			border:false,
            dynamic:true
            });
		var el = this.resizable.getEl();
		this.oldWidth = el.getWidth();
		this.oldHeight = el.getHeight();
		this.oldTop = el.getTop();
		this.tools.plus.show();
		this.tools.minus.show();
		this.tools.refresh.show();
		this.tools.gear.show();
		this.redraw();
		this.on('resize', this.redraw);
	},
	redraw:function(){
		if(this.isMax == true)
			this.maxRoom();
		if(this.isCenter == true)
			this.alignCenter();
	},
	initComponent: function(){
        Sigma.ImageView.superclass.initComponent.call(this);
        
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

Ext.reg('sigma.imageView', Sigma.ImageView);
