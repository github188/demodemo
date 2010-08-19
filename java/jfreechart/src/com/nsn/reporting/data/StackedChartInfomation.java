/**
* $ Name LastChangeRevision LastChangeDate LastChangeBy $
* $Id$
*/

package com.nsn.reporting.data;

import java.awt.Color;
import java.awt.GradientPaint;
import java.text.DecimalFormat;

import org.jfree.chart.JFreeChart;
import org.jfree.chart.axis.CategoryAxis;
import org.jfree.chart.axis.CategoryLabelPositions;
import org.jfree.chart.axis.NumberAxis;
import org.jfree.chart.axis.ValueAxis;
import org.jfree.chart.labels.CategoryItemLabelGenerator;
import org.jfree.chart.labels.StandardCategoryItemLabelGenerator;
import org.jfree.chart.plot.CategoryPlot;
import org.jfree.chart.plot.PlotOrientation;
import org.jfree.chart.renderer.category.GradientBarPainter;
import org.jfree.chart.renderer.category.StackedBarRenderer;
import org.jfree.data.category.DefaultCategoryDataset;

import com.nsn.reporting.chart.axis.FixedCategoryAxis;
import com.nsn.reporting.chart.renderer.FixedDomainStackBarRenderer;

public class StackedChartInfomation extends ChartInfomation {
	DefaultCategoryDataset dataset = null;
	
	public StackedChartInfomation(){
		super(STYLE_STACK);	
		this.labels.add(new AxisData("Build Date"));
		this.labels.add(new AxisData("Build Count"));
	}

	@Override
	public boolean processData(String data){
		//"090105, PASS, 100"
		
		if(dataset == null) dataset = new DefaultCategoryDataset();
		String[] cols = data.split(",");
		
		if(cols.length != 3) return false;
		
		dataset.addValue(Double.parseDouble(cols[2].trim()),
						 cols[1].trim(), 
						 cols[0].trim());		
		
		return true;
	}

	@Override
	public JFreeChart createChart() {
		CategoryAxis categoryAxis = new FixedCategoryAxis(this.labels.get(0).label, getFixedLength(),
				dataset.getRowCount());
		ValueAxis valueAxis = this.labels.get(1).buildAxis();

		StackedBarRenderer renderer = new FixedDomainStackBarRenderer();

		CategoryPlot plot = new CategoryPlot(dataset, categoryAxis, valueAxis,
				renderer);
		
		plot.setOrientation(PlotOrientation.VERTICAL);
		JFreeChart chart = new JFreeChart(this.getTitle(), JFreeChart.DEFAULT_TITLE_FONT,
				plot, true);
		
		this.decorateChart(chart);

		return chart;
	}
	
	public void decorateChart(JFreeChart chart){
		CategoryPlot plot = (CategoryPlot)chart.getPlot();
		StackedBarRenderer render = (StackedBarRenderer)plot.getRenderer();
		
		CategoryItemLabelGenerator generator = new StandardCategoryItemLabelGenerator(
				"{2}", new DecimalFormat("0") //调整显示的数字和字符格式
				);
		
		render.setBaseSeriesVisibleInLegend(true);
		
		render.setBaseItemLabelGenerator(generator);
		render.setBaseItemLabelsVisible(true);
		//render.setSeriesItemLabelsVisible(1, true);
		
		render.setShadowVisible(false);
		
		//render.setBarPainter(new GradientBarPainter(1,1,1));
		render.setBarPainter(new GradientBarPainter(0,0,0));
		
		//定义Series样式。
		for(int i = 0; i < Math.min(plot.getDataset().getRowCount(), 
									this.serialColors.size()); i++){
			render.setSeriesPaint(i, this.serialColors.get(i));
		}
		
		//CategoryAxis categoryaxis = plot.getDomainAxis(); //横轴上的 Lable 45度倾斜
		//categoryaxis.setCategoryLabelPositions(CategoryLabelPositions.UP_45);	
	}
}
