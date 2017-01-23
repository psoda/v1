package org.math.plot;

import java.awt.*;

import org.math.plot.canvas.*;
import org.math.plot.utils.*;

/**
 * BSD License
 * 
 * @author Yann RICHET
 */

/** class for ascending compatibility */
public class Plot3DPanel extends PlotPanel {

	private static final long serialVersionUID = 1L;
	private static String axis0 = "Xaxis";
	private static String axis1 = "Yaxis";
	private static String axis2 = "Zaxis";

	public Plot3DPanel() {
		super(new Plot3DCanvas());
	}

	public Plot3DPanel(double[] min, double[] max, String[] axesScales, String[] axesLabels) {
		super(new Plot3DCanvas(min, max, axesScales, axesLabels));
	}

	public Plot3DPanel(PlotCanvas _canvas, String legendOrientation) {
		super(_canvas, legendOrientation);
	}

	public Plot3DPanel(PlotCanvas _canvas) {
		super(_canvas);
	}

	public Plot3DPanel(String legendOrientation) {
		super(new Plot3DCanvas(), legendOrientation);
	}

	public static void setAxisLabel(int numAxis, String property){
		if(numAxis == 0){
			axis0 = property;
		}
		else if(numAxis == 1){
			axis1 = property;
		}
		else if(numAxis == 2){
			axis2 = property;
		}
	}

	public String getAxisLabel(int numAxis){
		if(numAxis == 0){
			return axis0;
		}
		else if(numAxis == 1){
			return axis1;
		}
		else if(numAxis == 2){
			return axis2;
		}
		return "";
	}

	public int addScatterPlot(String name, Color c, double[]... XY) {
		return ((Plot3DCanvas) plotCanvas).addScatterPlot(name, c, XY);
	}

	public int addScatterPlot(String name, double[]... XY) {
		return addScatterPlot(name, getNewColor(), XY);
	}

	public int addLinePlot(String name, Color c, double[]... XY) {
		return ((Plot3DCanvas) plotCanvas).addLinePlot(name, c, XY);
	}

	public int addLinePlot(String name, double[]... XY) {
		return addLinePlot(name, getNewColor(), XY);
	}

	public int addBarPlot(String name, Color c, double[]... XY) {
		return ((Plot3DCanvas) plotCanvas).addBarPlot(name, c, XY);
	}

	public int addBarPlot(String name, double[]... XY) {
		return addBarPlot(name, getNewColor(), XY);
	}

	public int addBoxPlot(String name, Color c, double[][] XY, double[][] dX) {
		return ((Plot3DCanvas) plotCanvas).addBoxPlot(name, c, XY, dX);
	}

	public int addBoxPlot(String name, double[][] XY, double[][] dX) {
		return addBoxPlot(name, getNewColor(), XY, dX);
	}

	public int addBoxPlot(String name, Color c, double[][] XYdX) {
		return ((Plot3DCanvas) plotCanvas).addBoxPlot(name, c, Array.getColumnsRangeCopy(XYdX, 0, 2), Array.getColumnsRangeCopy(XYdX, 3, 5));
	}

	public int addBoxPlot(String name, double[][] XYdX) {
		return addBoxPlot(name, getNewColor(), XYdX);
	}

	public int addHistogramPlot(String name, Color c, double[][] XY, double[][] dX) {
		return ((Plot3DCanvas) plotCanvas).addHistogramPlot(name, c, XY, dX);
	}

	public int addHistogramPlot(String name, double[][] XY, double[][] dX) {
		return addHistogramPlot(name, getNewColor(), XY, dX);
	}

	public int addHistogramPlot(String name, Color c, double[][] XYdX) {
		return ((Plot3DCanvas) plotCanvas).addHistogramPlot(name, c, Array.getColumnsRangeCopy(XYdX, 0, 2), Array.getColumnsRangeCopy(XYdX, 3, 4));
	}

	public int addHistogramPlot(String name, double[][] XYdX) {
		return addHistogramPlot(name, getNewColor(), XYdX);
	}

	public int addHistogramPlot(String name, Color c, double[][] XY, int nX, int nY) {
		return ((Plot3DCanvas) plotCanvas).addHistogramPlot(name, c, XY, nX, nY);
	}

	public int addHistogramPlot(String name, double[][] XY, int nX, int nY) {
		return addHistogramPlot(name, getNewColor(), XY, nX, nY);
	}

	public int addHistogramPlot(String name, Color c, double[][] XY, double[] boundsX, double[] boundsY) {
		return ((Plot3DCanvas) plotCanvas).addHistogramPlot(name, c, XY, boundsX, boundsY);
	}

	public int addHistogramPlot(String name, double[][] XY, double[] boundsX, double[] boundsY) {
		return addHistogramPlot(name, getNewColor(), XY, boundsX, boundsY);
	}

	public int addHistogramPlot(String name, Color c, double[][] XY, double minX, double maxX, int nX, double minY, double maxY, int nY) {
		return ((Plot3DCanvas) plotCanvas).addHistogramPlot(name, c, XY, minX, maxX, nX, minY, maxY, nY);
	}

	public int addHistogramPlot(String name, double[][] XY, double minX, double maxX, int nX, double minY, double maxY, int nY) {
		return addHistogramPlot(name, getNewColor(), XY, minX, maxX, nX, minY, maxY, nY);
	}

	public int addGridPlot(String name, Color c, double[] X, double[] Y, double[][] Z) {
		return ((Plot3DCanvas) plotCanvas).addGridPlot(name, c, X, Y, Z);
	}

	public int addGridPlot(String name, double[] X, double[] Y, double[][] Z) {
		return addGridPlot(name, getNewColor(), X, Y, Z);
	}

	public int addGridPlot(String name, Color c, double[][] XYZMatrix) {
		return ((Plot3DCanvas) plotCanvas).addGridPlot(name, c, XYZMatrix);
	}

	public int addGridPlot(String name, double[][] XYZMatrix) {
		return addGridPlot(name, getNewColor(), XYZMatrix);
	}

	@Override
	public int addPlot(String type, String name, Color c, double[]... XY) {
		if (type.equalsIgnoreCase(SCATTER)) {
			return addScatterPlot(name, c, XY);
		} else if (type.equalsIgnoreCase(LINE)) {
			return addLinePlot(name, c, XY);
		} else if (type.equalsIgnoreCase(BAR)) {
			return addBarPlot(name, c, XY);
		} else if (type.equalsIgnoreCase(HISTOGRAM)) {
			return addHistogramPlot(name, c, XY);
		} else if (type.equalsIgnoreCase(BOX)) {
			return addBoxPlot(name, c, XY);
		} else if (type.equalsIgnoreCase(GRID)) {
			return addGridPlot(name, c, XY);
		} else {
			throw new IllegalArgumentException("Plot type is unknown : " + type);
		}
	}

}
