//Pushlet cient.

/*
*   @url -- pushlet server url.
*   @catalog -- subscribe catalog.
*   @hander -- the event handler, 
*                @param {Object} the event.
*/
Ext.app.Pushlet = function(cfg){
    
    Ext.apply(this, cfg);
    //Ext.apply(this, config);
    this.state = this.STATE_READY;
    
    this.addEvents(
        /**
         * @event connected
         * Fired event a new session is connected.
         * @param {Object} This Pushlet object.                  
         * @param {Object} the PushletEvent ojbect.
         * @param {String} the created session id.
         */
        "connected",
        /**
         * @event connected
         * Fired event have a data is pushed from server..
         * @param {Object} This Pushlet object.                  
         * @param {Object} the PushletEvent ojbect.
         */
        "data",        
       /**
         * @event error
         * Fired event have error.
         * @param {Object} This Pushlet object.                  
         * @param {Object} the PushletEvent ojbect.
         */
        "error",      
       /**
         * @event refresh
         * The event is sent by pushlet server, to tell the client update the data.
         * @param {Object} This Pushlet object.                  
         * @param {Object} the PushletEvent ojbect.
         */
        "refresh", 
       /**
         * @event state
         * Fired event when the connection state changed.
         * @param {Object} This Pushlet object.                  
         * @param {Object} the PushletEvent ojbect.
         */
        "state"
    );
    
    this.on('refresh', function(pushlet, evt){
            var timeout = evt.get('p_wait');
            console.log('update the request after ' + timeout + 'ms');
			setTimeout(function () {
				pushlet.refresh();
			}, timeout);
        });
    if(this.hander){
        this.on('data', this.hander);
    }
}

Ext.extend(Ext.app.Pushlet, Ext.util.Observable, {
    NV_P_FORMAT: 'p_format=xml-strict',
	NV_P_MODE: 'p_mode=pull',
	pushletURL: null,
	webRoot: null,
	sessionId: null,
	STATE_ERROR: -2,
	STATE_ABORT: -1,
	STATE_NULL: 1,
	STATE_READY: 2,
	STATE_JOINED: 3,
	STATE_LISTENING: 3,
	state: 1,

/************** START PUBLIC FUNCTIONS  **************/

    cometStart : function(catalog){
        this.joinListen(catalog || this.catalog);
    },
    
    cometStop : function(){
        this.leave();
    },

/** Send heartbeat. */
	heartbeat: function() {
		this._doRequest('hb');
	},

/** Join. */
	join: function() {
		this.sessionId = null;

		// Streaming is only supported in Mozilla. E.g. IE does not allow access to responseText on readyState == 3
		this._doRequest('join', this.NV_P_FORMAT + '&' + this.NV_P_MODE);
	},

/** Join, listen and subscribe. */
	joinListen: function(aSubject) {
		this.sessionId = null;
		// Create event URI for listen
		var query = this.NV_P_FORMAT + '&' + this.NV_P_MODE;

		// Optional subject to subscribe to
		if (aSubject) {
			query = query + '&p_subject=' + aSubject;
		}

		this._doRequest('join-listen', query);

	},

/** Close pushlet session. */
	leave: function() {
		this._doRequest('leave');
	},

/** Listen on event channel. */
	listen: function(aSubject) {

		// Create event URI for listen
		var query = this.NV_P_MODE;

		// Optional subject to subscribe to
		if (aSubject) {
			query = query + '&p_subject=' + aSubject;
		}

		this._doRequest('listen', query);
	},

/** Subscribe to (comma separated) subject(s). */
	subscribe: function(aSubject, aLabel) {

		var query = 'p_subject=' + aSubject;
		if (aLabel) {
			query = query + '&p_label=' + aLabel;
		}
		this._doRequest('subscribe', query);

	},

/** Unsubscribe from (all) subject(s). */
	unsubscribe: function(aSubscriptionId) {
		var query;

		// If no sid we unsubscribe from all subscriptions
		if (aSubscriptionId) {
			query = 'p_sid=' + aSubscriptionId;
		}
		this._doRequest('unsubscribe', query);
	},
	
/** Unsubscribe from (all) subject(s). */
	refresh: function() {
	    console.log("do refresh........");
	    this._doRequest('refresh');
	},	

/************** END PUBLIC FUNCTIONS  **************/	

// Do XML HTTP request
	_doRequest: function(anEvent, aQuery) {
		// Check if we are not in any error state
		console.log('anEvent:' + anEvent + '\taQuery' + aQuery);
		console.log('state:' + this.state);
		if (this.state < 0) {
			this._setStatus('died (' + this.state + ')');
			return;
		}

		// We may have (async) requests outstanding and thus
		// may have to wait for them to complete and change state.
		var waitForState = false;
		if (anEvent == 'join' || anEvent == 'join-listen') {
			// We can only join after initialization
			waitForState = (this.state < this.STATE_READY);
		} else if (anEvent == 'leave') {
			this.state = this.STATE_READY;
		} else if (anEvent == 'refresh') {
			// We must be in the listening state
			if (this.state != this.STATE_LISTENING) {
				return;
			}
		} else if (anEvent == 'listen') {
			// We must have joined before we can listen
			waitForState = (this.state < this.STATE_JOINED);
		} else if (anEvent == 'subscribe' || anEvent == 'unsubscribe') {
			// We must be listeing for subscription mgmnt
			waitForState = (this.state < this.STATE_LISTENING);
		} else {
			// All other requests require that we have at least joined
			waitForState = (this.state < this.STATE_JOINED);
		}

		// May have to wait for right state to issue request
		if (waitForState == true) {
		    console.log(anEvent + ' , waiting... state=' + this.state);
			//this._setStatus(anEvent + ' , waiting... state=' + this.state);			
			return;
		}

		// ASSERTION: this.state is OK for this request

		// Construct base URL for GET
		var url = this.url + '?p_event=' + anEvent;

		// Optionally attach query string
		if (aQuery) {
			url = url + '&' + aQuery;
		}

		// Optionally attach session id
		if (this.sessionId != null) {
			url = url + '&p_id=' + this.sessionId;
			if (anEvent == 'p_leave') {
				this.sessionId = null;
			}
		}
		console.log('_doRequest', url);
		this._getXML(url);

		// uncomment to use synchronous XmlHttpRequest
		//var rsp = this._getXML(url);
		//this._onResponse(rsp);  */
	},

// Get XML doc from server
// On response  optional callback fun is called with optional user data.
	_getXML: function(url) {

		// Obtain XMLHttpRequest object
		var xmlhttp = new XMLHttpRequest();
		if (!xmlhttp || xmlhttp == null) {
			alert('No browser XMLHttpRequest (AJAX) support');
			return;
		}

		// Setup optional async response handling via callback
		//var cb = callback;
		var scope = this;
		xmlhttp.onreadystatechange = function() {
			if (xmlhttp.readyState == 4) {
				if (xmlhttp.status == 200) {
					// Processing statements go here...
					scope._onResponse(xmlhttp.responseXML);

					// Avoid memory leaks in IE
					// 12.may.2007 thanks to Julio Santa Cruz
					xmlhttp = null;
				} else {
					var event = new PushletEvent();
					event.put('p_event', 'error')
					event.put('p_reason', '[pushlet] problem retrieving XML data:\n' + xmlhttp.statusText);
					this._onEvent(event);
				}
			}
		};
		// Open URL
		xmlhttp.open('GET', url, true);

		// Send XML to KW server
		xmlhttp.send(null);
	},

/** Handle incoming events from server. */
	_onEvent: function (event) {
		// Create a PushletEvent object from the arguments passed in
		// push.arguments is event data coming from the Server
        
        //getEvent
        console.log('_onEvent:' + event.getEvent());
		// Do action based on event type
		var eventType = event.getEvent();
		var extEvent = 'data';
    	if (eventType == 'error') {
			this.state = this.STATE_ERROR;
			extEvent = 'error';
		} else if (eventType == 'join-ack') {
			this.state = this.STATE_JOINED;
			this.sessionId = event.get('p_id');
			this.fireEvent('connected', this, event, this.sessionId);
			return;
		} else if (eventType == 'join-listen-ack') {
			this.state = this.STATE_LISTENING;
			this.sessionId = event.get('p_id');
			this.fireEvent('connected', this, event, this.sessionId);
			return;
		} else if (eventType == 'listen-ack') {
			this.state = this.STATE_LISTENING;
		} else if (eventType == 'abort') {
			this.state = this.STATE_ERROR;
		} else if (eventType.match(/nack$/)) {
		    eventType = 'nack';
			//this._setStatus('error response: ' + event.get('p_reason'));
		} else if (eventType == 'data'){
		    extEvent = 'data';
		} else if (eventType == 'refresh'){
		    extEvent = 'refresh';
	    }
						
		this.fireEvent(extEvent, this, event);
	},

/**  Handle XMLHttpRequest response XML. */
	_onResponse: function(xml) {
		var events = this._rsp2Events(xml);
		if (events == null) {
			return;
		}

		delete xml;

		//this.debug('_onResponse eventCnt=', events.length);
		// Go through all <event/> elements
		for (i = 0; i < events.length; i++) {
			this._onEvent(events[i]);
			//this.hander(events[i]);
		}
	},

/** Convert XML response to PushletEvent objects. */
	_rsp2Events: function(xml) {
		// check empty response or xml document
		if (!xml || !xml.documentElement) {
			return null;
		}

		// Convert xml doc to array of PushletEvent objects
		var eventElements = xml.documentElement.getElementsByTagName('event');
		var events = new Array(eventElements.length);
		for (i = 0; i < eventElements.length; i++) {
			events[i] = new PushletEvent(eventElements[i]);
		}

		return events;

	},

	statusMsg: 'null',
	statusChanged: false,
	statusChar: '|',


	_showStatus: function() {
		// To show progress
	},

	_setStatus: function(status) {
		this.statusMsg = "pushlet - " + status;
		this.statusChanged = true;
	}
});


function PushletEvent(xml) {
	// Member variable setup; the assoc array stores the N/V pairs
	this.arr = new Array();

	this.getSubject = function() {
		return this.get('p_subject');
	}

	this.getEvent = function() {
		return this.get('p_event');
	}

	this.put = function(name, value) {
		return this.arr[name] = value;
	}

	this.get = function(name) {
		return this.arr[name];
	}

	this.toString = function() {
		var res = '';
		for (var i in this.arr) {
			res = res + i + '=' + this.arr[i] + '\n';
		}
		return res;
	}

	this.toTable = function() {
		var res = '<table border="1" cellpadding="3">';
		var styleDiv = '<div style="color:black; font-family:monospace; font-size:10pt; white-space:pre;">'

		for (var i in this.arr) {
			res = res + '<tr><td bgColor=white>' + styleDiv + i + '</div></td><td bgColor=white>' + styleDiv + this.arr[i] + '</div></td></tr>';
		}
		res += '</table>'
		return res;
	}

	// Optional XML element <event name="value" ... />
	if (xml) {
		// Put the attributes in Map
		for (var i = 0; i < xml.attributes.length; i++) {
			this.put(xml.attributes[i].name, xml.attributes[i].value);
		}
	}
}