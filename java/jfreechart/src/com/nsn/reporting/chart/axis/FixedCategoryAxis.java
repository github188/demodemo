/**
 * $ Name LastChangeRevision LastChangeDate LastChangeBy $
 * $Id$
 */

package com.nsn.reporting.chart.axis;

import java.awt.Graphics2D;
import java.awt.geom.Rectangle2D;

import org.jfree.chart.axis.CategoryAxis;
import org.jfree.text.G2TextMeasurer;
import org.jfree.text.TextBlock;
import org.jfree.text.TextUtilities;
import org.jfree.ui.RectangleEdge;

public class FixedCategoryAxis extends CategoryAxis {
	private int categoryCount = 0;
	private int createdLabelCount = 0;

	private int realCategoryCount = 0;

	private int labelCount = 10;

	public FixedCategoryAxis(String label, int categoryCount,
			int realCategoryCount) {
		super(label);

		this.categoryCount = categoryCount;

		labelCount = 1;
		this.realCategoryCount = realCategoryCount;
	}

	protected double calculateCategoryGapSize(int categoryCount,
			Rectangle2D area, RectangleEdge edge) {
		categoryCount = Math.max(categoryCount, this.categoryCount);
		double result = super.calculateCategoryGapSize(categoryCount, area,
				edge);

		return result;
	}

	protected double calculateCategorySize(int categoryCount, Rectangle2D area,
			RectangleEdge edge) {

		categoryCount = Math.max(categoryCount, this.categoryCount);
		double result = super.calculateCategorySize(categoryCount, area, edge);

		return result;

	}

	public int getCategoryCount() {
		return this.categoryCount;
	}

	protected TextBlock createLabel(Comparable category, float width,
			RectangleEdge edge, Graphics2D g2) {

		if (realCategoryCount > 0) {
			createdLabelCount = createdLabelCount % realCategoryCount;
		}
		String labelText = "";
		createdLabelCount++;
		if (labelCount == 1) {
			labelText = category.toString();
		} else {
			labelText = createdLabelCount % labelCount == 1 ? category
					.toString() : "";
		}

		/*
		 * if (!labelText.equals("")){ labelText = createdLabelCount + ", " +
		 * realCategoryCount; }
		 */

		TextBlock label = TextUtilities.createTextBlock(labelText,
				getTickLabelFont(category), getTickLabelPaint(category), width,
				this.getMaximumCategoryLabelLines(), new G2TextMeasurer(g2));
		return label;
	}

}
