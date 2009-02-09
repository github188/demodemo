/**
* $ Name LastChangeRevision LastChangeDate LastChangeBy $
* $Id$
*/


package com.nsn.reporting.chart.renderer;

import java.awt.geom.Rectangle2D;

import org.jfree.chart.axis.CategoryAxis;
import org.jfree.chart.plot.CategoryPlot;
import org.jfree.chart.plot.PlotOrientation;
import org.jfree.chart.renderer.category.CategoryItemRendererState;
import org.jfree.chart.renderer.category.StackedBarRenderer;
import org.jfree.data.category.CategoryDataset;

import com.nsn.reporting.chart.axis.FixedCategoryAxis;

public class FixedDomainStackBarRenderer extends StackedBarRenderer {
	
    protected void calculateBarWidth(CategoryPlot plot, Rectangle2D dataArea,
			int rendererIndex, CategoryItemRendererState state) {
    	
        // calculate the bar width
        CategoryAxis xAxis = plot.getDomainAxisForDataset(rendererIndex);
        CategoryDataset data = plot.getDataset(rendererIndex);
        if (data != null) {
            PlotOrientation orientation = plot.getOrientation();
            double space = 0.0;
            if (orientation == PlotOrientation.HORIZONTAL) {
                space = dataArea.getHeight();
            }
            else if (orientation == PlotOrientation.VERTICAL) {
                space = dataArea.getWidth();
            }
            double maxWidth = space * getMaximumBarWidth();
            int columns = getDomainCount(plot, rendererIndex);
            double categoryMargin = 0.0;
            if (columns > 1) {
                categoryMargin = xAxis.getCategoryMargin();
            }

            double used = space * (1 - xAxis.getLowerMargin()
                                     - xAxis.getUpperMargin()
                                     - categoryMargin);
            if (columns > 0) {
                state.setBarWidth(Math.min(used / columns, maxWidth));
            }
            else {
                state.setBarWidth(Math.min(used, maxWidth));
            }
        }    	
	}
    
    public int getDomainCount(CategoryPlot plot, int rendererIndex) {
    	CategoryAxis axis = plot.getDomainAxis();
    	if (axis instanceof FixedCategoryAxis){
    		FixedCategoryAxis faxis = (FixedCategoryAxis)axis;
    		return faxis.getCategoryCount();
    	}else {
    		CategoryDataset data = plot.getDataset(rendererIndex);
    		return data.getColumnCount();
    	}
    }
    
    

}
