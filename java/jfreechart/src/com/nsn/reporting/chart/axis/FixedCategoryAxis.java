/**
* $ Name LastChangeRevision LastChangeDate LastChangeBy $
* $Id$
*/

package com.nsn.reporting.chart.axis;

import java.awt.geom.Rectangle2D;

import org.jfree.chart.axis.CategoryAxis;
import org.jfree.ui.RectangleEdge;

public class FixedCategoryAxis extends CategoryAxis {
	private int categoryCount = 0;
	
	public FixedCategoryAxis(String label, int categoryCount){
		super(label);
		
		this.categoryCount = categoryCount;
	}
	
    protected double calculateCategoryGapSize(int categoryCount,
			Rectangle2D area, RectangleEdge edge) {
    	categoryCount = Math.max(categoryCount, this.categoryCount); 
    	double result = super.calculateCategoryGapSize(categoryCount, area, edge);
    	
		return result;
	}
    
    protected double calculateCategorySize(int categoryCount, Rectangle2D area,
			RectangleEdge edge) {
    	categoryCount = Math.max(categoryCount, this.categoryCount);
    	double result = super.calculateCategorySize(categoryCount, area, edge);

    	return result;

	}
    
    public int getCategoryCount(){
    	return this.categoryCount;
    }
}
