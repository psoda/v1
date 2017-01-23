/*
 * Created on 31 mai 2005 by richet
 */
package org.math.plot.render;

import java.awt.*;
import java.awt.font.*;

import org.math.plot.canvas.*;

public abstract class AWTDrawer extends AbstractDrawer {

	protected Projection projection;

	public AWTDrawer(PlotCanvas _canvas) {
		super(_canvas);
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see org.math.plot.render.AbstractDrawer#resetProjection()
	 */
	public void resetBaseProjection() {
		projection.initBaseCoordsProjection();
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see org.math.plot.render.AbstractDrawer#setColor(java.awt.Color)
	 */
	public void setColor(Color c) {
		comp2D.setColor(c);
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see org.math.plot.render.AbstractDrawer#setFont(java.awt.Font)
	 */
	public void setFont(Font f) {
		comp2D.setFont(f);
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see org.math.plot.render.AbstractDrawer#getColor()
	 */
	public Color getColor() {
		return comp2D.getColor();
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see org.math.plot.render.AbstractDrawer#getFont()
	 */
	public Font getFont() {
		return comp2D.getFont();
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see org.math.plot.render.AbstractDrawer#project(double[])
	 */
	public int[] project(double... pC) {
		return projection.screenProjection(pC);
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see org.math.plot.render.AbstractDrawer#projectRatio(double[])
	 */
	public int[] projectBase(double... rC) {
		return projection.screenProjectionBaseRatio(rC);
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see org.math.plot.render.AbstractDrawer#translate(int[])
	 */
	public void translate(int... t) {
		projection.translate(t);
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see org.math.plot.render.AbstractDrawer#dilate(int[], double[])
	 */
	public void dilate(int[] screenOrigin, double[] screenRatio) {
		projection.dilate(screenOrigin, screenRatio);
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see org.math.plot.render.AbstractDrawer#drawString(java.lang.String,
	 *      double[], double, double, double)
	 */
	public void drawText(String label, double... pC) {
		int[] sC = projection.screenProjection(pC);

		// Corner offset adjustment : Text Offset is used Here
		FontRenderContext frc = comp2D.getFontRenderContext();
		Font font1 = comp2D.getFont();
		int x = sC[0];
		int y = sC[1];
		double w = font1.getStringBounds(label, frc).getWidth();
		double h = font1.getSize2D();
		x -= (int) (w * text_Eastoffset);
		y += (int) (h * text_Northoffset);

		if (text_angle != 0)
			comp2D.rotate(text_angle, x + w / 2, y - h / 2);

		comp2D.drawString(label, x, y);

		if (text_angle != 0)
			comp2D.rotate(-text_angle, x + w / 2, y - h / 2);
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see org.math.plot.render.AbstractDrawer#drawStringRatio(java.lang.String,
	 *      double[], double, double, double)
	 */
	public void drawTextBase(String label, double... rC) {
		int[] sC = projection.screenProjectionBaseRatio(rC);

		// Corner offset adjustment : Text Offset is used Here
		FontRenderContext frc = comp2D.getFontRenderContext();
		Font font1 = comp2D.getFont();
		int x = sC[0];
		int y = sC[1];
		double w = font1.getStringBounds(label, frc).getWidth();
		double h = font1.getSize2D();
		x -= (int) (w * text_Eastoffset);
		y += (int) (h * text_Northoffset);

		if (text_angle != 0)
			comp2D.rotate(text_angle, x + w / 2, y - h / 2);

		comp2D.drawString(label, x, y);

		if (text_angle != 0)
			comp2D.rotate(-text_angle, x + w / 2, y - h / 2);
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see org.math.plot.render.AbstractDrawer#drawLineRatio(double[],
	 *      double[])
	 */
	public void drawLineBase(double[]... rC) {
		int[][] sC = new int[rC.length][];
		for (int i = 0; i < sC.length; i++)
			sC[i] = projection.screenProjectionBaseRatio(rC[i]);
		drawLine(sC);
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see org.math.plot.render.AbstractDrawer#drawLine(double[], double[])
	 */
	public void drawLine(double[]... pC) {
		int[][] sC = new int[pC.length][];
		for (int i = 0; i < sC.length; i++){
			sC[i] = projection.screenProjection(pC[i]);
		}
		drawLine(sC);
	}

	private void drawLine(int[]... c) {
		Stroke s = null;
		switch (line_type) {
		case CONTINOUS_LINE:
			s = new BasicStroke(line_width);
			break;
		case DOTTED_LINE:
			s = new BasicStroke(line_width, BasicStroke.CAP_ROUND, BasicStroke.JOIN_ROUND, 1f, new float[] { 2f }, 0f);
			break;
		}
		comp2D.setStroke(s);

		int[] x = new int[c.length];
		for (int i = 0; i < c.length; i++)
			x[i] = c[i][0];
		int[] y = new int[c.length];
		for (int i = 0; i < c.length; i++)
			y[i] = c[i][1];
		comp2D.drawPolyline(x, y, c.length);
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see org.math.plot.render.AbstractDrawer#drawDot(double[])
	 */
	public void drawDot(double... pC) {
		int[] sC = projection.screenProjection(pC);
		switch (dot_type) {
		case ROUND_DOT:
			comp2D.fillOval(sC[0] - dot_radius, sC[1] - dot_radius, 2 * dot_radius, 2 * dot_radius);
			break;
		case CROSS_DOT:
			comp2D.drawLine(sC[0] - dot_radius, sC[1] - dot_radius, sC[0] + dot_radius, sC[1] + dot_radius);
			comp2D.drawLine(sC[0] + dot_radius, sC[1] - dot_radius, sC[0] - dot_radius, sC[1] + dot_radius);
			break;
		case PATTERN_DOT:
			int yoffset = dot_pattern.length / 2;
			int xoffset = dot_pattern[0].length / 2;
			for (int i = 0; i < dot_pattern.length; i++)
				for (int j = 0; j < dot_pattern[i].length; j++)
					if (dot_pattern[i][j])
						// comp2D.setColor(new Color(getColor())
						comp2D.fillRect(sC[0] - xoffset + j, sC[1] - yoffset + i, 1, 1);
			break;
		}
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see org.math.plot.render.AbstractDrawer#drawPloygon(double[][])
	 */
	public void drawPolygon(double[]... pC) {
		int[][] c = new int[pC.length][2];
		for (int i = 0; i < pC.length; i++)
			c[i] = projection.screenProjection(pC[i]);

		int[] x = new int[c.length];
		for (int i = 0; i < c.length; i++)
			x[i] = c[i][0];
		int[] y = new int[c.length];
		for (int i = 0; i < c.length; i++)
			y[i] = c[i][1];
		comp2D.drawPolygon(x, y, c.length);
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see org.math.plot.render.AbstractDrawer#fillPloygon(double[][])
	 */
	public void fillPolygon(double[]... pC) {
		int[][] c = new int[pC.length][2];
		for (int i = 0; i < pC.length; i++)
			c[i] = projection.screenProjection(pC[i]);

		int[] x = new int[c.length];
		for (int i = 0; i < c.length; i++)
			x[i] = c[i][0];
		int[] y = new int[c.length];
		for (int i = 0; i < c.length; i++)
			y[i] = c[i][1];
		Composite cs = comp2D.getComposite();
		comp2D.setComposite(AlphaComposite.getInstance(AlphaComposite.SRC_OVER,0.2f));
		comp2D.fillPolygon(x, y, c.length);
		comp2D.setComposite(cs);
	}

}
