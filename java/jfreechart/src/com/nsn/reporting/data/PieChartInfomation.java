/**
* $ Name LastChangeRevision LastChangeDate LastChangeBy $
* $Id$
*/

package com.nsn.reporting.data;

import java.text.DecimalFormat;

import org.jfree.chart.JFreeChart;
import org.jfree.chart.labels.PieSectionLabelGenerator;
import org.jfree.chart.labels.StandardPieSectionLabelGenerator;
import org.jfree.chart.labels.StandardPieToolTipGenerator;
import org.jfree.chart.plot.PiePlot;
import org.jfree.data.general.DefaultPieDataset;
import org.jfree.ui.RectangleInsets;

public class PieChartInfomation extends ChartInfomation {
	
	DefaultPieDataset dataset = null;
	
	public PieChartInfomation(){
		super(STYLE_PIE);
	}

	public JFreeChart createChart() {
		
        PiePlot plot = new PiePlot(dataset);
        plot.setLabelGenerator(new StandardPieSectionLabelGenerator());
        plot.setInsets(new RectangleInsets(0.0, 5.0, 5.0, 5.0));
        plot.setToolTipGenerator(new StandardPieToolTipGenerator());
        
        JFreeChart chart = new JFreeChart(this.getTitle(), 
        		JFreeChart.DEFAULT_TITLE_FONT,
                plot, true);
        
		this.decorateChart(chart);
        
        return chart;
	}

	@Override
	public boolean processData(String data) {
		if(dataset == null) dataset = new DefaultPieDataset();
		String[] cols = data.split(",");
		
		if(cols.length != 2) return false;
		
		dataset.setValue(cols[0].trim(), Double.parseDouble(cols[1].trim()));		
		
		return true;
	}
	
	public void decorateChart(JFreeChart chart){
		PiePlot plot = (PiePlot)chart.getPlot();
		
		PieSectionLabelGenerator generator = new StandardPieSectionLabelGenerator(
				"{0}:{1}", new DecimalFormat("0"), new DecimalFormat("0.0"));
		
		plot.setSimpleLabels(true);
		plot.setLabelGenerator(generator);
	
		for(int i = 0; i < Math.min(plot.getDataset().getItemCount(), 
									this.serialColors.size()); i++){
			plot.setSectionPaint(plot.getDataset().getKey(i), 
					this.serialColors.get(i));
		}
		
		
	}

}
