//Demo client class.
GRAPH_TYPE_HISTORY="history.graph.type";
GRAPH_TYPE_REALTIME="realtime.graph.type";
Ext.app.DemoClient = function(cfg){

    Ext.apply(this, cfg);
    //Ext.apply(this, config);
    this.addEvents(
        /**
         * @event beforeload
         * Fires before a network request is made to retrieve the Json text which specifies a demo.
         * @param {Object} This DemoClient object.
         * @param {Object} callback The callback function specified in the {@link #load} call.
         */
        "beforeload",
        /**
         * @event load
         * Fires when the node has been successfuly loaded.
         * @param {Object} This DemoClient object.
         * @param {Object} response The response object containing the data from the server.
         */
        "load",
        /**
         * @event loadexception
         * Fires if the network request failed.
         * @param {Object} This DemoClient object.
         * @param {Object} error message.
         */
        "loadexception"

        //Demo runtime event.
        //"info"
    );

    Ext.tree.TreeLoader.superclass.constructor.call(this);
    this.on('loadexception', function(d){
      //alert("Demo loading execption, name:" + d.name + " , error:" + d.error);
    });

    this.load(function(d){
          //alert(d.description);
       }
    );

    this.comet = new Ext.app.Pushlet({
                    url: '/pushlet.srv',
                    catalog: '/demo' + this.name
                });

    //register the demo Event listener.
    var demo = this;
    this.comet.on('data', function(p, evt){
        console.log("fire demo event:" + evt);
        demo.fireEvent(evt.getEvent(), demo, evt);
    });

    this.on('info', function(d, E){
        console.log("demo info:" + E.get('msg'));
    })
    //this.load(function(){alert(this.description)});

};

Ext.extend(Ext.app.DemoClient, Ext.util.Observable, {
    load : function(callback){
        if(this.url || this.name || this.demoId){
            this.requestData(callback);
        }
    },

    startDemo: function(callback){
        if(!this.comet.sessionId){
            var client = this;
            this.comet.on('connected', function(p,E,s){
                client.startDemo(callback);
            });
            this.comet.cometStart();
            return;
        }
        this.requestData(callback, 'start');
    },

    lockDemo: function(callback) {
        this.requestData(callback, 'lock');
    },


    requestData : function(callback, op){
        op = (!op) ? 'load' : op;

        var query = '?op=' + op + '&dn=' + this.demoId;
        if(op == 'start'){
            query += '&pId=' + this.comet.sessionId;
        }
        query = (this.url || '/demo.cgi') + query;

        if(this.fireEvent("beforeload", this, callback) !== false){
            this.transId = Ext.Ajax.request({
                method:this.requestMethod,
                url: query || this.url || query,
                success: this.handleResponse,
                failure: this.handleFailure,
                scope: this,
                argument: {callback: callback, op: op || 'load'}
            });
        }else{
            // if the load is cancelled, make sure we notify
            // the node that we are done
            if(typeof callback == "function"){
                callback(this);
            }
        }
    },

    topoURL : function(){
      return "/~" + this.name + "/" + this.topology;
    }

    ,
    isLoading : function(){
        return this.transId ? true : false;
    },

    abort : function(){
        if(this.isLoading()){
            Ext.Ajax.abort(this.transId);
        }
    },

	getGraphs:function(){
		var _graphs = new Array();
		for (var i = 0; i < this.graphs.length; i++) {
			var _graph = this.graphs[i];
			var graph = this._getGraph(_graph);
			if(graph != "undefined")
				_graphs.push(graph);
		};
		return _graphs;
	},

	_getGraph:function(_graph){
		if(_graph.type == GRAPH_TYPE_REALTIME){
			/**
			 * "series":"series1,series2",
			 * "title":"M7iDC-A Interfaces",
			 * "graphStyle":{"name":"gsa"},
			 * "name":"graph1",
			 * "type":"realtime.graph.type"
			 */
			var serieses = new Array();
			var _series = _graph.series.split(",");
			for (var i = 0; i < _series.length; i++) {
				var series = this.getSeries(_series[i])
				if (series != "undefined") {
					serieses.push(series);
				}
			}
			/**
			 * "series":"series1,series2",
			 * "title":"M7iDC-A Interfaces",
			 * "graphStyle":{"name":"gsa"},
			 * "name":"graph1",
			 * "type":"realtime.graph.type"
			 * "serieses":[{
			 * 			"yLabel":"yLabel",
						"OID":"a.b.c.d.e.f.g1",
						"xLabel":"xLabel",
						"XLabel":"xLabel",
						"community":"public",
						"yMax":100,
						"YLabel":"yLabel",
						"type":"snmp",
						"ip":"a1.b1.c1.d1",
						"poll":10,
						"color":"blue",
						"description":"ge-0\/0\/2.0",
						"name":"series1",
						"YMax":100
			    	},{
						"yLabel":"yLabel",
						"OID":"a.b.c.d.e.f.g2",
						"xLabel":"xLabel",
						"XLabel":"xLabel",
						"community":"public",
						"yMax":100,
						"YLabel":"yLabel",
						"type":"snmp",
						"ip":"a1.b1.c1.d1",
						"poll":10,
						"color":"orange",
						"description":"ge-0\/0\/3.0",
						"name":"series2",
						"YMax":100
			 *  	}]
			 */
			Ext.apply(_graph,{
				serieses: serieses
			});
			return _graph;
		}
		else{
			/**
			 * "picture":"http:\/\/127.0.0.1\/pic?node=abc&type=his",
			 * "title":"his",
			 * "name":"his",
	         * "type":"history.graph.type"
			 */
			return _graph;
		}
	},

	getGraph:function(graphName){
		var _graph="undefined";
		for (var i = 0; i < this.graphs.length; i++) {
			if(this.graphs[i].name == graphName)
				_graph = this.graphs[i];
		};

		if(_graph == "undefined")
			return _graph;

		return this._getGraph(_graph);
	},

	getSeries:function(seriesName){
		for(var i = 0; i < this.serieses.length; i++){
			if(this.serieses[i].name == seriesName)
				return this.serieses[i];
		}
		return "undefined";
	},
	
	getGraphNameBySeriesName:function(sname){
		for(var i = 0; i < this.graphs.length; i++){
			var _graph = this.graphs[i];
			var _series = _graph.series.split(",");
			for(var j = 0; j < _series.length; j ++){
				if(_series[j] == sname)
					return _graph.name;
			}
		}
		return "undefined";
	},

    processResponse : function(response, callback, op){
        var json = response.responseText;
        try {
            var o = eval("("+json+")");
            if(op == 'load'){
                if(o.error){
                  Ext.apply(this, o);
                  throw o.error;
                }
                Ext.apply(this, o.data);
            }
            if(typeof callback == "function"){
                callback(this, o);
            }
        }catch(e){
            this.handleFailure(response);
        }
    },

    handleResponse : function(response){
        this.transId = false;
        var a = response.argument;
        this.processResponse(response, a.callback, a.op);

        if(a.op == 'load'){
            this.fireEvent("load", this, a.callback);
        }
    },

    handleFailure : function(response){
        this.transId = false;
        var a = response.argument;
        this.fireEvent("loadexception", this);
        /*
        ignore the call back, if ocur exception.
        if(typeof a.callback == "function"){
            a.callback(this);
        }
        */
    }

});
