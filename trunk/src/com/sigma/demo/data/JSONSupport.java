package com.sigma.demo.data;

import org.json.simple.JSONObject;

public interface JSONSupport {
	public JSONObject jsonEncode();
	public void jsonDecode(JSONObject o);
}
