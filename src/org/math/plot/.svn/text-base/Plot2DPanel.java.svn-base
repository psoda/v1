package org.math.plot;

import java.awt.*;

import org.math.plot.canvas.*;

/**
 * BSD License
 * 
 * @author Yann RICHET
 */

/** class for ascending compatibility */
public class Plot2DPanel extends PlotPanel {

	private static final long serialVersionUID = 1L;

	public Plot2DPanel() {
		super(new Plot2DCanvas());
	}

	public Plot2DPanel(double[] min, double[] max, String[] axesScales, String[] axesLabels) {
		super(new Plot2DCanvas(min, max, axesScales, axesLabels));
	}

	public Plot2DPanel(PlotCanvas _canvas, String legendOrientation) {
		super(_canvas, legendOrientation);
	}

	public Plot2DPanel(PlotCanvas _canvas) {
		super(_canvas);
	}

	public Plot2DPanel(String legendOrientation) {
		super(new Plot2DCanvas(), legendOrientation);
	}

	public int addScatterPlot(String name, Color c, double[]... XY) {
		return ((Plot2DCanvas) plotCanvas).addScatterPlot(name, c, XY);
	}

	public int addScatterPlot(String name, double[]... XY) {
		return addScatterPlot(name, getNewColor(), XY);
	}

	public int addLinePlot(String name, Color c, double[]... XY) {
		return ((Plot2DCanvas) plotCanvas).addLinePlot(name, c, XY);
	}

	public int addLinePlot(String name, double[]... XY) {
		return addLinePlot(name, getNewColor(), XY);
	}

	public int addBarPlot(String name, Color c, double[]... XY) {
		return ((Plot2DCanvas) plotCanvas).addBarPlot(name, c, XY);
	}

	public int addBarPlot(String name, double[]... XY) {
		return addBarPlot(name, getNewColor(), XY);
	}

	public int addStaircasePlot(String name, Color c, double[]... XY) {
		return ((Plot2DCanvas) plotCanvas).addStaircasePlot(name, c, XY);
	}

	public int addStaircasePlot(String name, double[]... XY) {
		return addStaircasePlot(name, getNewColor(), XY);
	}

	public int addBoxPlot(String name, Color c, double[][] XY, double[][] dX) {
		return ((Plot2DCanvas) plotCanvas).addBoxPlot(name, c, XY, dX);
	}

	public int addBoxPlot(String name, double[][] XY, double[][] dX) {
		return addBoxPlot(name, getNewColor(), XY, dX);
	}

	public int addBoxPlot(String name, Color c, double[][] XYdX) {
		return ((Plot2DCanvas) plotCanvas).addBoxPlot(name, c, XYdX);
	}

	public int addBoxPlot(String name, double[][] XY) {
		return addBoxPlot(name, getNewColor(), XY);
	}

	public int addHistogramPlot(String name, Color c, double[][] XY, double[] dX) {
		return ((Plot2DCanvas) plotCanvas).addHistogramPlot(name, c, XY, dX);
	}

	public int addHistogramPlot(String name, double[][] XY, double[] dX) {
		return addHistogramPlot(name, getNewColor(), XY, dX);
	}

	public int addHistogramPlot(String name, Color c, double[][] XYdX) {
		return ((Plot2DCanvas) plotCanvas).addHistogramPlot(name, c, XYdX);
	}

	public int addHistogramPlot(String name, double[][] XYdX) {
		return addHistogramPlot(name, getNewColor(), XYdX);
	}

	public int addHistogramPlot(String name, Color c, double[] X, int n) {
		return ((Plot2DCanvas) plotCanvas).addHistogramPlot(name, c, X, n);
	}

	public int addHistogramPlot(String name, double[] X, int n) {
		return addHistogramPlot(name, getNewColor(), X, n);
	}
	
	public int addHistogramPlot(String name, Color c, double[] X, double... bounds) {
		return ((Plot2DCanvas) plotCanvas).addHistogramPlot(name, c, X, bounds);
	}

	public int addHistogramPlot(String name, double[] X, double... bounds) {
		return addHistogramPlot(name, getNewColor(), X, bounds);
	}

	public int addHistogramPlot(String name, Color c, double[] X, double min, double max, int n) {
		return ((Plot2DCanvas) plotCanvas).addHistogramPlot(name, c, X, min, max, n);
	}

	public int addHistogramPlot(String name, double[] X, double min, double max, int n) {
		return addHistogramPlot(name, getNewColor(), X, min, max, n);
	}

	@Override
	public int addPlot(String type, String name, Color c, double[]... XY) {
		if (type.equalsIgnoreCase(SCATTER)) {
			return addScatterPlot(name, c, XY);
		} else if (type.equalsIgnoreCase(LINE)) {
			return addLinePlot(name, c, XY);
		} else if (type.equalsIgnoreCase(BAR)) {
			return addBarPlot(name, c, XY);
		} else if (type.equalsIgnoreCase(STAIRCASE)) {
			return addStaircasePlot(name, c, XY);
		} else if (type.equalsIgnoreCase(HISTOGRAM)) {
			return addHistogramPlot(name, c, XY);
		} else if (type.equalsIgnoreCase(BOX)) {
			return addBoxPlot(name, c, XY);
		} else {
			throw new IllegalArgumentException("Plot type is unknown : " + type);
		}
	}
}
