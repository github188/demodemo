/* DemoStep.java
 *
 * Sigma Resources & Technologies, Inc.
 * Copyright (C) 2005-2007, sigma-rt.com
 *
 * PROPRIETARY RIGHTS of Sigma Resources & Technologies are involved in
 * the subject matter of this material. All manufacturing, reproduction,
 * use, and sales rights pertaining to this subject matter are governed
 * by the license agreement. The recipient of this software implicitly
 * accepts the terms of the license.
 *
 * Created on Oct 26, 2007, by Alexandre Tsu.
 * 
 * Project:	DemoLauncher
 * Author :	James Che
 * Auditor: 
 * 
 * # any description here # 
 *
 * For more information, visit:
 * http://www.sigma-rt.com
 * 
 * Change History
 * ---------------------------------------------------
 * REVISION          DATE/TIME        AUTHOR
 * ---------------------------------------------------
 * $Log$
 * ---------------------------------------------------
 *
 */
package com.sigma.demo.data;

import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import com.sigma_rt.asn1.DerInputStream;
import com.sigma_rt.asn1.DerOutputStream;
import com.sigma_rt.asn1.DerUtil;
import com.sigma_rt.asn1.DerValue;
import com.sigma_rt.encoding.DerEncoder;

/**
 * @author James Che
 * 
 */
public class DemoStep implements DerEncoder {
	private static final String _row_split = "\\|";

	private static final String _col_split = ",";

	private String name = null;

	private String subTitle = null;

	private String description = null;

	private String _consoles = null;

	private String[] depends = null;

	private byte[] topologyImage = null;

	private String topologyURL = null;

	private int _consoleRows = 0;

	private int _consoleCols = 0;

	private int consoleSize = 0;

	private int _graphRows = 0;

	private int _graphCols = 0;

	private int graphSize = 0;

	private String _graphs = null;

	private Map<Integer, String[]> consolesMap = new HashMap<Integer, String[]>();

	private Map<Integer, String[]> graphsMap = new HashMap<Integer, String[]>();

	private String preAction = null;

	private String function = null;

	/**
	 * @return the name
	 */
	public String getName() {
		return name;
	}

	/**
	 * @param name
	 *            the name to set
	 */
	public void setName(String name) {
		this.name = name;
	}

	/**
	 * @return the subTitle
	 */
	public String getSubTitle() {
		return subTitle;
	}

	/**
	 * @param titel
	 *            the subTitle to set
	 */
	public void setSubTitle(String subTitle) {
		this.subTitle = subTitle;
	}

	/**
	 * @return the description
	 */
	public String getDescription() {
		return description;
	}

	/**
	 * @param description
	 *            the description to set
	 */
	public void setDescription(String description) {
		this.description = description;
	}

	public void derDecode(DerInputStream in) throws IOException {
		name = DerUtil.getString(in);
		subTitle = DerUtil.getString(in);
		description = DerUtil.getString(in);
		setConsoles(DerUtil.getString(in));
		setGraphs(DerUtil.getString(in));
		preAction = DerUtil.getString(in);
		function = DerUtil.getString(in);

		topologyURL = DerUtil.getString(in);
		DerValue dv = in.getDerValue();
		if (dv.tag == DerValue.tag_OctetString) {
			topologyImage = dv.getOctetString();
		} else {
			topologyImage = null;
		}

		dv = in.getDerValue();
		if (dv.tag == DerValue.tag_Integer) {
			int size = dv.getInteger();
			depends = new String[size];
			for (int i = 0; i < size; i++) {
				depends[i] = DerUtil.getString(in);
			}
		} else {
			depends = null;
		}
	}

	public void derEncode(DerOutputStream out) throws IOException {
		DerUtil.putUTF8String(out, name);
		DerUtil.putUTF8String(out, subTitle);
		DerUtil.putUTF8String(out, description);
		DerUtil.putUTF8String(out, _consoles);
		DerUtil.putUTF8String(out, _graphs);
		DerUtil.putUTF8String(out, preAction);
		DerUtil.putUTF8String(out, function);

		DerUtil.putUTF8String(out, topologyURL);
		if (topologyImage != null)
			out.putOctetString(topologyImage);
		else
			out.putNull();

		if (depends != null) {
			int size = depends.length;
			out.putInteger(size);
			for (String d : depends) {
				DerUtil.putUTF8String(out, d);
			}
		} else {
			out.putNull();
		}
	}

	/**
	 * @param _consoles
	 *            the consoles to set
	 */
	public void setConsoles(String consoles) {
		if (consoles == null)
			return;

		this._consoles = consoles;
		String[] _rows = _consoles.split(_row_split);
		_consoleRows = _rows.length;
		for (int i = 0; i < _consoleRows; i++) {
			String[] _cols = _rows[i].split(_col_split);
			consoleSize += _cols.length;
			_consoleCols = _consoleCols > _cols.length ? _consoleCols
					: _cols.length;
			consolesMap.put(i, _cols);
		}
	}

	public int getConsolesCols() {
		return _consoleCols;
	}

	public int getConsolesRows() {
		return _consoleRows;
	}

	public DemoConsole[] getConsoles(int row, DemoData demo) {
		String[] demoNames = consolesMap.get(row);
		DemoConsole[] consoles = new DemoConsole[demoNames.length];
		for (int i = 0; i < demoNames.length; i++) {
			consoles[i] = demo.getConsole(demoNames[i]);

		}
		return consoles;
	}

	public DemoConsole getConsole(String consoleName, DemoData demo) {
		if (consoleName == null || demo == null)
			return null;

		List<DemoConsole> consoles = getConsoles(demo);
		for (DemoConsole console : consoles) {
			if (console == null)
				continue;

			if (consoleName.equals(console.getName()))
				return console;
		}

		return null;
	}

	public List<DemoConsole> getConsoles(DemoData demo) {
		List<DemoConsole> consoles = new ArrayList<DemoConsole>();
		for (int i = 0; i < _consoleRows; i++) {
			DemoConsole[] _consoles = getConsoles(i, demo);
			for (DemoConsole _con : _consoles) {
				consoles.add(_con);
			}
		}
		return consoles;
	}

	/**
	 * @param _graphs
	 *            the graphs to set
	 */
	public void setGraphs(String graphs) {
		if (graphs == null)
			return;

		this._graphs = graphs;
		String[] _rows = _graphs.split(_row_split);
		_graphRows = _rows.length;
		for (int i = 0; i < _graphRows; i++) {
			String[] _cols = _rows[i].split(_col_split);
			graphSize += _cols.length;
			_graphCols = _graphCols > _cols.length ? _graphCols : _cols.length;
			graphsMap.put(i, _cols);
		}
	}

	public int getGraphsCols() {
		return _graphCols;
	}

	public int getGraphsRows() {
		return _graphRows;
	}

	public Graph[] getGraphs(int row, DemoData demo) {
		String[] graphNames = graphsMap.get(row);
		Graph[] graphs = new Graph[graphNames.length];
		for (int i = 0; i < graphNames.length; i++) {
			graphs[i] = demo.getGraph(graphNames[i]);

		}
		return graphs;
	}

	public Graph getGraph(String graphName, DemoData demo) {
		if (graphName == null || demo == null)
			return null;

		List<Graph> graphs = getGraphs(demo);
		for (Graph graph : graphs) {
			if (graph == null)
				continue;

			if (graphName.equals(graph.getName()))
				return graph;
		}

		return null;
	}

	public List<Graph> getGraphs(DemoData demo) {

		List<Graph> graphs = new ArrayList<Graph>();
		for (int i = 0; i < _graphRows; i++) {
			Graph[] _graphs = getGraphs(i, demo);
			for (Graph _g : _graphs) {
				graphs.add(_g);
			}
		}
		return graphs;
	}

	/**
	 * @return the consoleSize
	 */
	public int getConsoleSize() {
		return consoleSize;
	}

	/**
	 * @return the graphSize
	 */
	public int getGraphSize() {
		return graphSize;
	}

	/**
	 * @return the depends
	 */
	public String[] getDepends() {
		return depends;
	}

	/**
	 * @param depends
	 *            the depends to set
	 */
	public void setDepends(String[] depends) {
		this.depends = depends;
	}

	/**
	 * @param depends
	 *            The sample:"a_depend,b_depend,c_depend"
	 */
	public void setDepends(String depends) {
		if (depends != null)
			this.depends = depends.split(_col_split);
	}

	/**
	 * @return the topologyImage
	 */
	public byte[] getTopologyImage() {
		return topologyImage;
	}

	/**
	 * @param topologyImage
	 *            the topologyImage to set
	 */
	public void setTopologyImage(byte[] topologyImage) {
		this.topologyImage = topologyImage;
	}

	/**
	 * @return the topologyURL
	 */
	public String getTopologyURL() {
		return topologyURL;
	}

	/**
	 * @param topologyURL
	 *            the topologyURL to set
	 */
	public void setTopologyURL(String topologyURL) {
		this.topologyURL = topologyURL;
	}

	/**
	 * @return the preAction
	 */
	public String getPreAction() {
		return preAction;
	}

	/**
	 * @param preAction
	 *            the preAction to set
	 */
	public void setPreAction(String preAction) {
		this.preAction = preAction;
	}

	public List<RealtimeGraph> getGraphs(String seriesName, DemoData demo) {
		List<RealtimeGraph> result = new ArrayList<RealtimeGraph>();
		List<Graph> graphs = this.getGraphs(demo);
		for (Graph graph : graphs) {
			if (graph.getType().equals(Graph.TYPE_SMP)) {
				RealtimeGraph rg = (RealtimeGraph) graph;
				if (rg.isContains(seriesName))
					result.add(rg);
			} else {
				continue;
			}
		}
		return result;
	}

	/**
	 * @return the function
	 */
	public String getFunction() {
		return function;
	}

	/**
	 * @param function
	 *            the function to set
	 */
	public void setFunction(String function) {
		this.function = function;
	}

	public String toString() {
		String d = null;
		if (depends != null) {
			for (String depend : depends) {
				d = d == null ? depend : d + "," + depend;
			}
		}
		return "Action name:" + name + "; subTitle: " + subTitle
				+ "; description: " + description + ";topologyURL:"
				+ this.topologyURL + ";graphs :" + _graphs + "; consoles:"
				+ _consoles + "; depends:" + d;
	}
}
