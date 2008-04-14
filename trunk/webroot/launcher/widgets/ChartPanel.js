/**
 * @author James
 */
Sigma.ChartPanel = function(cfg){
      Sigma.ChartPanel.superclass.constructor.call(this,{
    	//_cont             = el,
    	_yMin: null,
    	_yMax: null,
    	_xGridDensity: 0,
    	_yGridDensity: 0,
    	_flags: 0,          // type. CHART_LINE|CHART_AREA|CHART_BAR|CHART_STACKED
    	_series: Array(),
    	_labelPrecision: 0,
    	_horizontalLabels: new Array(),
    	_barWidth: 10,
    	_barDistance: 2,
    	_bars: 0,
    	_showLegend: true,    
    	cls: 'chart'
      });
      Ext.apply(this, cfg);
      
      if((typeof CanvasChartPainterFactory != 'undefined') && (window.CanvasRenderingContext2D)) {
		this._painterFactory = CanvasChartPainterFactory;
	  } else if (typeof JsGraphicsChartPainterFactory != 'undefined') {
		this._painterFactory = JsGraphicsChartPainterFactory;
	  } else { this._painterFactory = null; }
	    
      this.on('resize', this.drawChart, this);
      this.on('render', function(){
            this.body.addClass(this.cls)
        }, this);
};

Ext.extend(Sigma.ChartPanel, Ext.Panel,{
    initDemo: function(d){
    	this.setDefaultType(CHART_LINE);
    	this.setGridDensity(10, 10);
    	this.setHorizontalLabels(['mon', 'tue', 'wed', 'thu', 'fri', 'sat', 'sun', 'mon', 'tue', 'wed']);
    	this.setShowLegend(true);
    	this.addSeries('test1', '#4040FF', [ 5, 10, 20, 10, 40, 52, 68, 70, 70, 60]);
    	this.addSeries('test2', '#8080FF', [ 8,  7, 12, 20, 24, 16, 36, 28, 28, 45]);
    	this.addSeries('test3', '#FF8080', [ 3, 12,  5, 18, 20, 13, 28, 36, 18,  5]);
    },
	/**
	 * 
	 * @param {Array} serieses
	 * 
	 * {
	 *	"yLabel":"yLabel",
		"OID":"a.b.c.d.e.f.g1",
		"xLabel":"xLabel",
		"community":"public",
		"yMax":100,
		"type":"snmp",
		"ip":"a1.b1.c1.d1",
		"poll":10,
		"color":"blue",
		"description":"ge-0\/0\/2.0",
		"name":"series1",
	},{
		"yLabel":"yLabel",
		"OID":"a.b.c.d.e.f.g2",
		"xLabel":"xLabel",
		"community":"public",
		"yMax":100,
		"type":"snmp",
		"ip":"a1.b1.c1.d1",
		"poll":10,
		"color":"orange",
		"description":"ge-0\/0\/3.0",
		"name":"series2",
	}
	 */
	addSerieses:function(serieses){
		for(var i = 0; i < serieses.length; i++){
			var series = serieses[i];
			this.addSeries(series.name, series.color,new Array());
			this.setHorizontalLabels(series.xLabel);
			this.setVerticalRange(0,series.yMax);
		};
	},
    
    setPainterFactory: function(f) {
    	this._painterFactory = f;
    },

    setVerticalRange: function(min, max) {
    	this._yMin = min;
    	this._yMax = max;
    },

    setLabelPrecision: function(precision) {
	    this._labelPrecision = precision;
    },

    setShowLegend: function(b) {
    	this._showLegend = b;
    },

    setGridDensity: function(horizontal, vertical) {
	this._xGridDensity = horizontal;
	this._yGridDensity = vertical;
    },

    setHorizontalLabels: function(labels) {
    	this._horizontalLabels = labels;
    },

    setDefaultType: function(flags) {
	    this._flags = flags;
    },

    setBarWidth: function(width) {
	    this._barWidth = width;
    },

    setBarDistance: function(distance) {
    	this._barDistance = distance;
    },
    
    addSeries: function(seriesName, color, values, flags) {
    	var o, offset;
    	if (!flags) { flags = this._flags; }
    	if ((flags & CHART_BAR) == CHART_BAR) { offset = this._barDistance + this._bars * (this._barWidth + this._barDistance); this._bars++; }
    	else { offset = 0; }
    	o = new ChartSeries(seriesName, color, values, flags, offset);
    	this._series[seriesName]=o;
    },
	
	putServiesData:function(seriesName,value){
		var values = this._series[seriesName].values;
		values.push(value);
		if(values.length > 10){
			values.shift();			
		}
		console.log("series:" + seriesName+", values length:" + values.length);
	},
    
    drawChart: function() {
    	var painter, i, o, o2, len, xlen, ymin, ymax, series, type, self, bLabels;
    	
    	if (!this._painterFactory) { return; }
    
    	/* Initialize */
    	series = new Array();
    	stackedSeries = new Array();
    	xlen = 0;
    	ymin = this._yMin;
    	ymax = this._yMax;
    
    	/* Separate stacked series (as they need processing). */
		for(e in this._series) {
			o = this._series[e];
    		if ((o.flags & CHART_STACKED) == CHART_STACKED) { series.push(o); }
		}
		/*
    	for (i = 0; i < this._series.length; i++) {
    		o = this._series[i]
    	}*/
    
    	/* Calculate values for stacked series */
    	for (i = series.length - 2; i >= 0; i--) {
    		o  = series[i].values;
    		o2 = series[i+1].values;
    		len = (o2.length > o.length)?o2.length:o.length;
    		for (j = 0; j < len; j++) {
    			if ((o[j]) && (!o2[j])) { continue; }
    			if ((!o[j]) && (o2[j])) { o[j] = o2[j]; }
    			else { o[j] = parseInt(o[j]) + parseFloat(o2[j]); }
    	}	}
    
    	/* Append non-stacked series to list */
		for(e in this._series) {
			o = this._series[e];
			if(typeof o!='function')
			{
				if ((o.flags & CHART_STACKED) != CHART_STACKED) {
					series.push(o);
				}
			}
		}
//    	for (i = 0; i < this._series.length; i++) {
//    		o = this._series[i]
//    		if ((o.flags & CHART_STACKED) != CHART_STACKED) { series.push(o); }
//    	}
    
    	/* Determine maximum number of values, ymin and ymax */
    	for (i = 0; i < series.length; i++) {
			o = series[i]
			if (o.values) {
				if (o.values.length > xlen) {
					xlen = o.values.length;
				}
				for (j = 0; j < o.values.length; j++) {
					if ((o.values[j] < ymin) || (ymin == null)) {
						ymin = o.values[j];
					}
					if (o.values[j] > ymax) {
						ymax = o.values[j];
					}
				}
			}
		}
    
    	/*
    	 * For bar only charts the number of charts is the same as the length of the
    	 * longest series, for others combinations it's one less. Compensate for that
    	 * for bar only charts.
    	 */
    	if (series.length == this._bars) {
    		xlen++;
    		this._xGridDensity++;
    	}
    
    	/*
    	 * Determine whatever or not to show the legend and axis labels
    	 * Requires density and labels to be set.
    	 */
    	bLabels = ((this._xGridDensity) && (this._yGridDensity) && (this._horizontalLabels.length >= this._xGridDensity));
    
    	/* Create painter object */
    	painter = this._painterFactory();
    	painter.create(this.body.dom);
    
    	/* Initialize painter object */
    	painter.init(xlen, ymin, ymax, this._xGridDensity, this._yGridDensity, bLabels);
    
    	/* Draw chart */
    	painter.drawBackground();
    
    	/*
    	 * If labels and grid density where specified, draw legend and labels.
    	 * It's drawn prior to the chart as the size of the legend and labels
    	 * affects the size of the chart area.
    	 */
    	if (this._showLegend) { painter.drawLegend(series); }
    	if (bLabels) {
    		painter.drawVerticalAxis(this._yGridDensity, this._labelPrecision);
    		painter.drawHorizontalAxis(xlen, this._horizontalLabels, this._xGridDensity, this._labelPrecision);
    	}
    
    	/* Draw chart */
    	painter.drawChart();
    
    	/* Draw series */
    	for (i = 0; i < series.length; i++) {
    		type = series[i].flags & ~CHART_STACKED;
    		switch (type) {
    			case CHART_LINE: painter.drawLine(series[i].color, series[i].values); break;
    			case CHART_AREA: painter.drawArea(series[i].color, series[i].values); break;
    			case CHART_BAR:  painter.drawBars(series[i].color, series[i].values, xlen-1, series[i].offset, this._barWidth); break;
    			default: ;
    		};
    	}
    
    	/*
    	 * Draw axis (after the series since the anti aliasing of the lines may
    	 * otherwise be drawn on top of the axis)
    	 */
    	painter.drawAxis();
    
    }
});
Ext.reg('sigma.chartPanel', Sigma.ChartPanel);