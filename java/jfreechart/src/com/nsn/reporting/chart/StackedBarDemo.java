package com.nsn.reporting.chart;

import java.awt.Color;
import java.awt.GradientPaint;
import java.io.FileOutputStream;
import java.text.DecimalFormat;

import org.jfree.chart.ChartUtilities;
import org.jfree.chart.JFreeChart;
import org.jfree.chart.axis.CategoryAxis;
import org.jfree.chart.axis.CategoryLabelPositions;
import org.jfree.chart.axis.NumberAxis;
import org.jfree.chart.axis.ValueAxis;
import org.jfree.chart.labels.CategoryItemLabelGenerator;
import org.jfree.chart.labels.StandardCategoryItemLabelGenerator;
import org.jfree.chart.plot.CategoryPlot;
import org.jfree.chart.plot.DatasetRenderingOrder;
import org.jfree.chart.plot.PlotOrientation;
import org.jfree.chart.renderer.category.GradientBarPainter;
import org.jfree.chart.renderer.category.LineAndShapeRenderer;
import org.jfree.chart.renderer.category.StackedBarRenderer;
import org.jfree.data.category.CategoryDataset;
import org.jfree.data.category.DefaultCategoryDataset;

import com.nsn.reporting.chart.axis.FixedCategoryAxis;
import com.nsn.reporting.chart.renderer.FixedDomainStackBarRenderer;


public class StackedBarDemo {
	
	public static void main(String[] args) throws Exception{
		CategoryDataset dataset = getDataSet2();
		JFreeChart chart = createStackedBarChart(
							"Test Case", // 图表标题
							"Build Date", // 目录轴的显示标签
							"Count", // 数值轴的显示标签
							dataset, // 数据集
							PlotOrientation.VERTICAL, // 图表方向：水平、垂直
							true, 	// 是否显示图例(对于简单的柱状图必须是false)
							true, 	// 是否生成工具
							false 	// 是否生成URL链接
							);
							
		//chart.getPlot().
		CategoryPlot plot = (CategoryPlot)chart.getPlot();
		
		plot.setRangeAxis(1,  new NumberAxis("rate"));
		plot.setDataset(1, getDataSet3());
		plot.setRenderer(1, new LineAndShapeRenderer());
		plot.mapDatasetToRangeAxis(1,1);
		
		plot.setDatasetRenderingOrder(DatasetRenderingOrder.FORWARD);
		
		StackedBarRenderer render = (StackedBarRenderer)plot.getRenderer();
		
		CategoryItemLabelGenerator generator =new StandardCategoryItemLabelGenerator(
				"{3}", new DecimalFormat("000") //调整显示的数字和字符格式
				);
		
		render.setBaseSeriesVisibleInLegend(true);
		
		//plot.setDomainCrosshairVisible(false);
		
		render.setBaseItemLabelGenerator(generator);
		render.setBaseItemLabelsVisible(true);
		//render.setSeriesItemLabelsVisible(1, true);
		
		render.setShadowVisible(false);
		
		render.setBarPainter(new GradientBarPainter(1,1,1));
		
		//定义Series样式。
		GradientPaint gradientpaint = new GradientPaint(0.0F, 0.0F, Color.blue,
				0.0F, 0.0F, Color.blue);
		render.setSeriesPaint(1, gradientpaint);
		
		CategoryAxis categoryaxis = plot.getDomainAxis(); //横轴上的 Lable 45度倾斜
		categoryaxis.setCategoryLabelPositions(CategoryLabelPositions.UP_45);	
		
		/*
		IntervalMarker intervalmarker = new IntervalMarker(4.5D, 750D);
		intervalmarker.setLabel("Target Range");
		intervalmarker.setLabelFont(new Font("SansSerif", 2, 11));
		intervalmarker.setLabelAnchor(RectangleAnchor.LEFT);
		intervalmarker.setLabelTextAnchor(TextAnchor.CENTER_LEFT);
		intervalmarker.setPaint(new Color(222, 222, 255, 128));
		plot.addRangeMarker(intervalmarker, Layer.BACKGROUND);		
		*/
		
		//plot.setAxisOffset(new RectangleInsets(50D, 50D, 50D, 50D));
		
		
		
		FileOutputStream fos_jpg = null;
		try {
			fos_jpg = new FileOutputStream("fruit.jpg");
			ChartUtilities.writeChartAsJPEG(fos_jpg,1,chart,800,300,null);
		} finally {
			try {
				fos_jpg.close();
			} catch (Exception e) {}
		}
		System.out.println("done!");
	}
	
    public static JFreeChart createStackedBarChart(String title,
			String domainAxisLabel, String rangeAxisLabel,
			CategoryDataset dataset, PlotOrientation orientation,
			boolean legend, boolean tooltips, boolean urls) {

		if (orientation == null) {
			throw new IllegalArgumentException("Null 'orientation' argument.");
		}

		CategoryAxis categoryAxis = new FixedCategoryAxis(domainAxisLabel, 10, 2);
		ValueAxis valueAxis = new NumberAxis(rangeAxisLabel);

		StackedBarRenderer renderer = new FixedDomainStackBarRenderer();

		CategoryPlot plot = new CategoryPlot(dataset, categoryAxis, valueAxis,
				renderer);
		
		plot.setOrientation(orientation);
		JFreeChart chart = new JFreeChart(title, JFreeChart.DEFAULT_TITLE_FONT,
				plot, legend);
		return chart;

	}	
	
	private static CategoryDataset getDataSet2() {
		DefaultCategoryDataset dataset = new DefaultCategoryDataset();
		dataset.addValue(100, "PASS", "090105");
		dataset.addValue(100, "FAIL", "090105");
		dataset.addValue(100, "Not Run", "090105");

		dataset.addValue(200, "PASS", "090106");
		dataset.addValue(200, "FAIL", "090106");
		dataset.addValue(300, "Not Run", "090106");
		
		dataset.addValue(300, "PASS", "090107");
		dataset.addValue(200, "FAIL", "090107");
		dataset.addValue(400, "Not Run", "090107");
		
		dataset.addValue(300, "PASS", "090108");
		dataset.addValue(100, "FAIL", "090108");
		dataset.addValue(500, "Not Run", "090108");
		
		/*		
		dataset.addValue(100, "PASS", "2008/10/05");
		dataset.addValue(200, "FAIL", "2008/10/05");
		dataset.addValue(200, "Not Run", "2008/10/05");	
		
		dataset.addValue(400, "PASS", "2008/10/06");
		dataset.addValue(400, "FAIL", "2008/10/06");
		dataset.addValue(400, "Not Run", "2008/10/06");
*/		
		return dataset;
	}
	
	private static CategoryDataset getDataSet3() {
		DefaultCategoryDataset dataset = new DefaultCategoryDataset();
		dataset.addValue(8, "PASS1", "090105");
		dataset.addValue(1, "FAIL1", "090105");
		dataset.addValue(7, "Not Run1", "090105");

		dataset.addValue(2, "PASS1", "090106");
		dataset.addValue(2, "FAIL1", "090106");
		dataset.addValue(3, "Not Run1", "090106");
		
		dataset.addValue(3, "PASS1", "090107");
		dataset.addValue(2, "FAIL1", "090107");
		dataset.addValue(4, "Not Run1", "090107");
		
		dataset.addValue(3, "PASS1", "090108");
		dataset.addValue(1, "FAIL1", "090108");
		dataset.addValue(5, "Not Run1", "090108");
		
		/*		
		dataset.addValue(100, "PASS", "2008/10/05");
		dataset.addValue(200, "FAIL", "2008/10/05");
		dataset.addValue(200, "Not Run", "2008/10/05");	
		
		dataset.addValue(400, "PASS", "2008/10/06");
		dataset.addValue(400, "FAIL", "2008/10/06");
		dataset.addValue(400, "Not Run", "2008/10/06");
*/		
		return dataset;
	}	
}
