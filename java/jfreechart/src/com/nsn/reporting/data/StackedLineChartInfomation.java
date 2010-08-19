package com.nsn.reporting.data;

import java.awt.BasicStroke;
import java.awt.Color;
import java.awt.geom.Ellipse2D;
import java.text.DecimalFormat;
import java.util.ArrayList;
import java.util.List;
import java.util.Set;
import java.util.TreeSet;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import org.jfree.chart.JFreeChart;
import org.jfree.chart.axis.CategoryAxis;
import org.jfree.chart.axis.CategoryLabelPositions;
import org.jfree.chart.axis.NumberAxis;
import org.jfree.chart.axis.ValueAxis;
import org.jfree.chart.labels.CategoryItemLabelGenerator;
import org.jfree.chart.labels.ItemLabelAnchor;
import org.jfree.chart.labels.ItemLabelPosition;
import org.jfree.chart.labels.StandardCategoryItemLabelGenerator;
import org.jfree.chart.plot.CategoryPlot;
import org.jfree.chart.plot.DatasetRenderingOrder;
import org.jfree.chart.plot.PlotOrientation;
import org.jfree.chart.renderer.AbstractRenderer;
import org.jfree.chart.renderer.category.LineAndShapeRenderer;
import org.jfree.chart.renderer.category.StackedBarRenderer;
import org.jfree.data.category.CategoryDataset;
import org.jfree.data.category.DefaultCategoryDataset;
import org.jfree.ui.TextAnchor;

import com.nsn.reporting.chart.axis.FixedCategoryAxis;
import com.nsn.reporting.chart.renderer.FixedDomainStackBarRenderer;

public class StackedLineChartInfomation extends ChartInfomation {
	List<DefaultCategoryDataset> datasets = new ArrayList<DefaultCategoryDataset>();
	List<Set<String>> groups = new ArrayList<Set<String>>();

	public StackedLineChartInfomation() {
		this(STYLE_STACK_LINE);
	}
	
	public StackedLineChartInfomation(String name) {
		super(name);

		datasets.add(new DefaultCategoryDataset());
	}

	@Override
	public JFreeChart createChart() {
		CategoryAxis categoryAxis = new FixedCategoryAxis(this.labels.get(0).label,
				getFixedLength(), datasets.get(0).getColumnCount());
		NumberAxis valueAxis = this.labels.get(1).buildAxis();

		StackedBarRenderer renderer = new FixedDomainStackBarRenderer();
		renderer.setDrawBarOutline(true);
		renderer.setBasePositiveItemLabelPosition(new ItemLabelPosition(
				ItemLabelAnchor.OUTSIDE12, TextAnchor.BOTTOM_CENTER));
		CategoryPlot plot = new CategoryPlot(datasets.get(0), categoryAxis,
				valueAxis, renderer);
		categoryAxis.setCategoryMargin(0.6);

		plot.setOrientation(PlotOrientation.VERTICAL);
		JFreeChart chart = new JFreeChart(this.getTitle(),
				JFreeChart.DEFAULT_TITLE_FONT, plot, true);

		for (int i = 1; i < groups.size() + 1; i++) {
			int axis_index = -1;
			AxisData axisData = this.labels.get(i + 1);
			
			for (int j = 1; j < i; j++){
				if (axisData.equals(this.labels.get(j))){
					axis_index = j - 1;
					break;
				}
			}
			
			//not found exists Axis.
			if (axis_index <  0){
				NumberAxis axis = axisData.buildAxis();
				plot.setRangeAxis(i, axis);
				axis_index = i;
			}
			
			plot.setDataset(i, datasets.get(i));
			LineAndShapeRenderer render = new LineAndShapeRenderer(true, true);
			render.setBaseShapesVisible(true);
			render.setUseFillPaint(true);
			render.setBaseFillPaint(Color.white);
			render.setStroke(new BasicStroke(1.5f));
			render.setSeriesOutlineStroke(0, new BasicStroke(1));
			render
					.setSeriesShape(0, new Ellipse2D.Double(-3.0, -3.0, 6.0,
							6.0));

			plot.setRenderer(i, render);
			plot.mapDatasetToRangeAxis(i, axis_index);
			// 设置最高的一个柱与图片顶端的距离
			// axis.setUpperMargin(0.1);

			CategoryItemLabelGenerator generator = new StandardCategoryItemLabelGenerator(
					"{2}", new DecimalFormat("0") // 调整显示的数字和字符格式
			);
			render.setBaseItemLabelGenerator(generator);
			render.setBaseItemLabelsVisible(false);

		}

		plot.setDatasetRenderingOrder(DatasetRenderingOrder.FORWARD);

		this.decorateChart(chart);

		return chart;
	}

	public boolean processHeader(String name, String data) {
		if (!name.equals(HEADER_GROUP)) {
			return super.processHeader(name, data);
		}

		Set<String> group = new TreeSet<String>();
		for (String s : data.split(",")) {
			group.add(s.trim());
		}

		datasets.add(new DefaultCategoryDataset());
		groups.add(group);

		if (this.labels.size() - 2 < groups.size()) {
			//data.split(",")[0].trim()
			this.labels.add(new AxisData(data.split(",")[0].trim()));
		}
		
		/*
		if (this.labels.size() - 2 < groups.size()) {
			this.labels.add(data.split(",")[0].trim());
		}*/

		return true;
	}

	@Override
	public boolean processData(String data) {
		// "090105, PASS, 100"
		DefaultCategoryDataset dataset = null;

		String[] cols = data.split(",");

		if (cols.length != 3)
			return false;
		for (int i = 0; i < groups.size(); i++) {
			if (groups.get(i).contains(cols[1].trim())) {
				dataset = datasets.get(i + 1);
				break;
			}
		}

		if (dataset == null)
			dataset = datasets.get(0);

		dataset.addValue(this.pasreNumberSafe(cols[2].trim()), 
						 cols[1].trim(),
						 cols[0].trim());

		return true;
	}
	
	private double pasreNumberSafe(String str){
		Matcher m = Pattern.compile("(\\d+(\\.\\d+)?)").matcher(str);
		if (m.find()){
			return Double.parseDouble(m.group(1));
		}
		return 0;
	}

	public void decorateChart(JFreeChart chart) {

		currentTheme.apply(chart);

		CategoryPlot plot = (CategoryPlot) chart.getPlot();
		
		StackedBarRenderer render = (StackedBarRenderer) plot.getRenderer();

		// plot.setBackgroundPaint(Color.lightGray);

		CategoryItemLabelGenerator generator = new MyLabelGenerator(this);

		render.setBaseSeriesVisibleInLegend(true);
		ItemLabelPosition p = new ItemLabelPosition(ItemLabelAnchor.INSIDE12,
				TextAnchor.CENTER_RIGHT, TextAnchor.CENTER_RIGHT,
				-Math.PI / 2.0);
		render.setBasePositiveItemLabelPosition(p);

		ItemLabelPosition p2 = new ItemLabelPosition(ItemLabelAnchor.INSIDE12,
				TextAnchor.CENTER_LEFT, TextAnchor.CENTER_LEFT, -Math.PI / 2.0);
		render.setPositiveItemLabelPositionFallback(p2);
		render.setItemLabelAnchorOffset(-5.0);
		
		render.setBaseItemLabelGenerator(generator);
		
		
		
		plot.getDomainAxis().setCategoryLabelPositions(
				CategoryLabelPositions.UP_90);

		// render.setSeriesItemLabelsVisible(1, true);

		render.setShadowVisible(false);
//		 render.setDrawBarOutline(true);

		// render.setBarPainter(new GradientBarPainter(1,1,1));
		// render.setBarPainter(new GradientBarPainter(0,0,0));
		/*
		 * if (this.getFixedLength() > 30){ render.setItemMargin(0.9); }
		 */
		render.setItemMargin(2);

		ValueAxis rangeAxis = plot.getRangeAxis();
		// 设置最高的一个柱与图片顶端的距离
		int data_count=plot.getDataset().getColumnCount();
//		if(data_count<=30){
//			render.setBaseItemLabelsVisible(false);
//			rangeAxis.setUpperMargin(0.6);
//		}
//		else{
//			render.setBaseItemLabelsVisible(false);
//			rangeAxis.setUpperMargin(0.1);
//		}
		
		if(data_count>=60 && data_count<=90){
			plot.getDomainAxis().setCategoryMargin(0.7);
		}else if(data_count>90){
			plot.getDomainAxis().setCategoryMargin(0.8);
		}

		// set default line color.
		if (plot.getRendererCount() > 1) {
			AbstractRenderer r = (AbstractRenderer) plot.getRenderer(1);
			for (int i = 0; i < plot.getDataset(1).getRowCount(); i++) {
				r.setSeriesPaint(i, Color.BLUE);
			}
		}

		int colorIndex = 0;
		for (int i = 0; i < plot.getRendererCount()
				&& colorIndex < this.serialColors.size(); i++) {

			int seriesCount = plot.getDataset(i).getRowCount();
			AbstractRenderer r = (AbstractRenderer) plot.getRenderer(i);

			for (int j = 0; j < seriesCount
					&& colorIndex < this.serialColors.size(); j++) {
				Color color=this.serialColors.get(colorIndex++);
				r.setSeriesPaint(j, color);
			}
		}

		CategoryAxis categoryaxis = plot.getDomainAxis(); // 横轴上的 Lable 45度倾斜
		categoryaxis.setCategoryLabelPositions(CategoryLabelPositions.UP_90);
		categoryaxis.setLowerMargin(0.02);
	}

	class MyLabelGenerator extends StandardCategoryItemLabelGenerator {
		private int _column = 0;
		private String _label = "";
		private int count = 0;

		public MyLabelGenerator(
				StackedLineChartInfomation stackedLineChartInfomation) {
			// TODO Auto-generated constructor stub
		}

		public String generateLabel(CategoryDataset dataset, int row, int column) {
			String label = generateLabelString(dataset, row, column);
			if(label=="0")
				label="";
			count++;
			int _count=dataset.getRowCount();
			if (count == _count) {
				this.count = 0;
				_label += label;
				String temp = _label;
				_label = "";
				return temp;
			} else {
				_label += label + "/";
				return "";
			}

			// System.out.println("---"+label+"---"+row+"----"+column);
			// return label.equals("0") ? "" : label;
		}
	}

}
