/*
 * Created on 21 juil. 2005 by richet
 */
package org.math.plot.utils;

import static org.math.plot.utils.Array.*;

public class Histogram {
	//  histograms functions

	public static double[][] histogram_classes(double[] values, double[] bounds) {
		return mergeColumns(centers(bounds), histogram(values, bounds));
	}

	public static double[][] histogram_classes(double[] values, double min, double max, int n) {
		double[] bounds = bounds(values, min, max, n);
		return mergeColumns(centers(bounds), histogram(values, bounds));
	}

	public static double[][] histogram_classes(double[] values, int n) {
		double[] bounds = bounds(values, n);
		return mergeColumns(centers(bounds), histogram(values, bounds));
	}

	public static double[] histogram(double[] values, double[] bounds) {
		double[] h = new double[bounds.length - 1];
		for (int i = 0; i < values.length; i++) {
			for (int j = 0; j < h.length; j++) {
				if (((bounds[j + 1] - values[i]) * (bounds[j] - values[i]) < 0) || ((bounds[j] == values[i])))
					h[j]++;
			}
		}
		return h;
	}

	public static double[] histogram(double[] values, double min, double max, int n) {
		double[] bounds = bounds(values, min, max, n);
		return histogram(values, bounds);
	}

	public static double[] histogram(double[] values, int n) {
		return histogram(values, n);
	}

	private static double[] bounds(double[] values, int n) {
		double min = min(values);
		double max = max(values);
		return bounds(values, min, max, n);
	}

	private static double[] bounds(double[] values, double min, double max, int n) {
		double[] bounds = new double[n + 1];
		for (int i = 0; i < bounds.length; i++) {
			bounds[i] = min + (max - min) * i / (double) n;
		}
		return bounds;
	}

	private static double[] centers(double[] bounds) {
		double[] center = new double[bounds.length - 1];
		for (int i = 0; i < center.length; i++)
			center[i] = (bounds[i] + bounds[i + 1]) / 2;
		return center;
	}

	// histograms 2D functions

	public static double[][] histogram_classes_2D(double[][] values, double[] boundsX, double[] boundsY) {
		return insertColumn(centers_2D(boundsX, boundsY), histogram_2D(values, boundsX, boundsY), 2);
	}

	public static double[][] histogram_classes_2D(double[][] values, double minX, double maxX, int nX, double minY, double maxY, int nY) {
		double[] valuesX = getColumnCopy(values, 0);
		double[] valuesY = getColumnCopy(values, 1);
		double[] boundsX = bounds(valuesX, minX, maxX, nX);
		double[] boundsY = bounds(valuesY, minY, maxY, nY);
		return insertColumn(centers_2D(boundsX, boundsY), histogram_2D(values, boundsX, boundsY), 2);
	}

	public static double[][] histogram_classes_2D(double[][] values, int nX, int nY) {
		double[] valuesX = getColumnCopy(values, 0);
		double[] valuesY = getColumnCopy(values, 1);
		double[] boundsX = bounds(valuesX, nX);
		double[] boundsY = bounds(valuesY, nY);
		return insertColumn(centers_2D(boundsX, boundsY), histogram_2D(values, boundsX, boundsY), 2);
	}

	public static double[] histogram_2D(double[][] values, double[] boundsX, double[] boundsY) {
		double[] h = new double[(boundsX.length - 1) * (boundsY.length - 1)];
		for (int n = 0; n < values.length; n++) {
			for (int i = 0; i < boundsX.length - 1; i++) {
				for (int j = 0; j < boundsY.length - 1; j++) {
					if ((((boundsX[i + 1] - values[n][0]) * (boundsX[i] - values[n][0]) < 0) || ((boundsX[i] == values[n][0])))
							&& (((boundsY[j + 1] - values[n][1]) * (boundsY[j] - values[n][1]) < 0) || ((boundsY[j] == values[n][1]))))
						h[index(i, j, boundsX.length - 1, -1)]++;
				}
			}
		}
		return h;
	}

	public static double[] histogram_2D(double[][] values, double minX, double maxX, int nX, double minY, double maxY, int nY) {
		double[] valuesX = getColumnCopy(values, 0);
		double[] valuesY = getColumnCopy(values, 1);
		double[] boundsX = bounds(valuesX, minX, maxX, nX);
		double[] boundsY = bounds(valuesY, minY, maxY, nY);
		return histogram_2D(values, boundsX, boundsY);
	}

	public static double[] histogram_2D(double[][] values, int nX, int nY) {
		double[] valuesX = getColumnCopy(values, 0);
		double[] valuesY = getColumnCopy(values, 1);
		double[] boundsX = bounds(valuesX, nX);
		double[] boundsY = bounds(valuesY, nY);
		return histogram_2D(values, boundsX, boundsY);
	}

	private static double[][] centers_2D(double[] boundsX, double[] boundsY) {
		int nb_centers = (boundsX.length - 1) * (boundsY.length - 1);
		double[][] center = new double[nb_centers][2];
		for (int i = 0; i < boundsX.length - 1; i++) {
			for (int j = 0; j < boundsY.length - 1; j++) {
				int k = index(i, j, boundsX.length - 1, -1);
				center[k][0] = (boundsX[i] + boundsX[i + 1]) / 2;
				center[k][1] = (boundsY[j] + boundsY[j + 1]) / 2;
			}
		}
		return center;
	}

	private static int index(int i, int j, int imax, int jmax) {
		return i + imax * j;
	}

}
