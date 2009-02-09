package com.nsn.reporting.data;

import java.text.DecimalFormat;
import java.util.ArrayList;
import java.util.List;
import java.util.Set;
import java.util.TreeSet;

import org.jfree.chart.JFreeChart;
import org.jfree.chart.axis.CategoryAxis;
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
import org.jfree.data.category.DefaultCategoryDataset;

import com.nsn.reporting.chart.axis.FixedCategoryAxis;
import com.nsn.reporting.chart.renderer.FixedDomainStackBarRenderer;

public class StackedLineChartInfomation extends ChartInfomation {
	List<DefaultCategoryDataset> datasets = new ArrayList<DefaultCategoryDataset>();
	List<Set<String>> groups = new ArrayList<Set<String>>();
	
	public StackedLineChartInfomation(){
		super(STYLE_STACK_LINE);
		
		datasets.add(new DefaultCategoryDataset());
	}

	@Override
	public JFreeChart createChart() {
		CategoryAxis categoryAxis = new FixedCategoryAxis(this.labels.get(0), getFixedLength());
		ValueAxis valueAxis = new NumberAxis(this.labels.get(1));

		StackedBarRenderer renderer = new FixedDomainStackBarRenderer();

		CategoryPlot plot = new CategoryPlot(datasets.get(0), categoryAxis, valueAxis,
				renderer);
		
		plot.setOrientation(PlotOrientation.VERTICAL);
		JFreeChart chart = new JFreeChart(this.getTitle(), JFreeChart.DEFAULT_TITLE_FONT,
				plot, true);
		
		for(int i = 1; i < groups.size() + 1; i++){
			plot.setRangeAxis(i,  new NumberAxis(this.labels.get(i + 1)));
			plot.setDataset(i, datasets.get(i));
			plot.setRenderer(i, new LineAndShapeRenderer());
			plot.mapDatasetToRangeAxis(i, i);
		}		
		
		plot.setDatasetRenderingOrder(DatasetRenderingOrder.FORWARD);
		
		this.decorateChart(chart);

		return chart;		
	}
	
	public boolean processHeader(String name, String data){
		if(!name.equals(HEADER_GROUP)){
			return super.processHeader(name, data);
		}
		
		Set<String> group = new TreeSet<String>();
		for(String s: data.split(",")){
			group.add(s.trim());
		}
		
		datasets.add(new DefaultCategoryDataset());
		groups.add(group);
		
		if(this.labels.size() - 2 < groups.size()){
			this.labels.add(data.split(",")[0].trim());
		}
		
		return true;
	}
	

	@Override
	public boolean processData(String data){
		//"090105, PASS, 100"
		DefaultCategoryDataset dataset = null;
		
		String[] cols = data.split(",");
		
		if(cols.length != 3) return false;
		for(int i = 0; i < groups.size(); i++){
			if(groups.get(i).contains(cols[1].trim())){
				dataset = datasets.get(i + 1);
				break;
			}
		}
		
		if (dataset == null) dataset = datasets.get(0);
		
		dataset.addValue(Double.parseDouble(cols[2].trim()),
				 cols[1].trim(),
				 cols[0].trim());		
		
		return true;
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
