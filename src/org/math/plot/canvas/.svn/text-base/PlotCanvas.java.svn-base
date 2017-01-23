package org.math.plot.canvas;

import java.awt.*;
import java.awt.event.*;
import java.awt.image.*;
import java.io.*;
import java.util.*;

import javax.imageio.*;
import javax.swing.*;

import org.math.plot.components.*;
import org.math.plot.plotObjects.*;
import org.math.plot.plots.*;
import org.math.plot.render.*;
import org.math.plot.utils.*;

import static java.lang.Math.*;
import static org.math.plot.plotObjects.Base.*;

/**
 * BSD License
 * 
 * @author Yann RICHET
 */
public abstract class PlotCanvas extends JPanel implements MouseListener, MouseMotionListener, ComponentListener, BaseDependant, MouseWheelListener {

	public int[] panelSize = new int[] { 400, 400 };

	public Base base;

	protected AbstractDrawer draw;

	protected BasePlot grid;

	public LegendPanel linkedLegendPanel;

	public Vector<Plot> plots;

	public Vector<Plotable> objects;

	// ///////////////////////////////////////////
	// ////// Constructor & inits ////////////////
	// ///////////////////////////////////////////

	public PlotCanvas() {
		initPanel();
		initBasenGrid();
		initDrawer();
	}

	public PlotCanvas(Base b, BasePlot bp) {
		initPanel();
		initBasenGrid(b, bp);
		initDrawer();
	}

	public PlotCanvas(double[] min, double[] max) {
		initPanel();
		initBasenGrid(min, max);
		initDrawer();
	}

	public PlotCanvas(double[] min, double[] max, String[] axesScales, String[] axesLabels) {
		initPanel();
		initBasenGrid(min, max, axesScales, axesLabels);
		initDrawer();
	}

	public void attachLegend(LegendPanel lp) {
		linkedLegendPanel = lp;
	}

	private void initPanel() {
		objects = new Vector<Plotable>();
		plots = new Vector<Plot>();

		setDoubleBuffered(true);

		setSize(panelSize[0], panelSize[1]);
		setPreferredSize(new Dimension(panelSize[0], panelSize[1]));
		setBackground(Color.white);

		addComponentListener(this);
		addMouseListener(this);
		addMouseMotionListener(this);
		addMouseWheelListener(this);
	}

	public abstract void initDrawer();

	public void initBasenGrid(double[] min, double[] max, String[] axesScales, String[] axesLabels) {
		base = new Base(min, max, axesScales);
		grid = new BasePlot(base, axesLabels);
		// grid.getAxe(0).getDarkLabel().setCorner(0.5,-1);
		// grid.getAxe(1).getDarkLabel().setCorner(0,-0.5);
	}

	public abstract void initBasenGrid(double[] min, double[] max);

	public abstract void initBasenGrid();

	public void initBasenGrid(Base b, BasePlot bp) {
		base = b;
		grid = bp;
	}

	// ///////////////////////////////////////////
	// ////// set actions ////////////////////////
	// ///////////////////////////////////////////

	public void setActionMode(int am) {
		ActionMode = am;
	}

	public void setNoteCoords(boolean b) {
		allowNoteCoord = b;
	}

	public void setEditable(boolean b) {
		allowEdit = b;
	}

	public boolean getEditable() {
		return allowEdit;
	}

	public void setNotable(boolean b) {
		allowNote = b;
	}

	public boolean getNotable() {
		return allowNote;
	}

	// ///////////////////////////////////////////
	// ////// set/get elements ///////////////////
	// ///////////////////////////////////////////

	public Plot[] getPlots() {
		Plot[] plotarray = new Plot[plots.size()];
		plots.copyInto(plotarray);
		return plotarray;
	}

	public Plot getPlot(int i) {
		return (Plot) plots.get(i);
	}

	public int getPlotIndex(Plot p) {
		for (int i = 0; i < plots.size(); i++)
			if (getPlot(i) == p)
				return i;
		return -1;
	}

	public Plotable[] getPlotables() {
		Plotable[] plotablearray = new Plotable[objects.size()];
		objects.copyInto(plotablearray);
		return plotablearray;
	}

	public Plotable getPlotable(int i) {
		return (Plotable) objects.get(i);
	}

	public BasePlot getGrid() {
		return grid;
	}

	public String[] getAxesScales() {
		return base.getAxesScales();
	}

	public void setAxesLabels(String... labels) {
		grid.setLegend(labels);
		repaint();
	}

	public void setAxeLabel(int axe, String label) {
		grid.setLegend(axe, label);
		repaint();
	}

	public void setAxesScales(String... scales) {
		base.setAxesScales(scales);
		setAutoBounds();
	}

	public void setAxeScale(int axe, String scale) {
		base.setAxesScales(axe, scale);
		setAutoBounds(axe);
	}

	public void setFixedBounds(double[] min, double[] max) {
		base.setFixedBounds(min, max);
		resetBase();
		repaint();
	}

	public void setFixedBounds(int axe, double min, double max) {
		base.setFixedBounds(axe, min, max);
		resetBase();
		repaint();
	}

	public void includeInBounds(double... into) {
		base.includeInBounds(into);
		resetBase();
		repaint();
	}

	public void includeInBounds(Plot plot) {
		base.includeInBounds(Array.min(plot.getData()));
		base.includeInBounds(Array.max(plot.getData()));
		resetBase();
		repaint();
	}

	public void setAutoBounds() {
		if (plots.size() > 0) {
			Plot plot0 = this.getPlot(0);
			base.setRoundBounds(Array.min(plot0.getData()), Array.max(plot0.getData()));
		} else { // build default min and max bounds
			double[] min = new double[base.dimension];
			double[] max = new double[base.dimension];
			for (int i = 0; i < base.dimension; i++) {
				if (base.getAxeScale(i).equalsIgnoreCase(LINEAR)) {
					min[i] = 0.0;
					max[i] = 1.0;
				} else if (base.getAxeScale(i).equalsIgnoreCase(LOGARITHM)) {
					min[i] = 1.0;
					max[i] = 10.0;
				}
			}
			base.setRoundBounds(min, max);
		}
		for (int i = 1; i < plots.size(); i++) {
			Plot ploti = this.getPlot(i);
			base.includeInBounds(Array.min(ploti.getData()));
			base.includeInBounds(Array.max(ploti.getData()));
		}
		resetBase();
		repaint();
	}

	public void setAutoBounds(int axe) {
		if (plots.size() > 0) {
			Plot plot0 = this.getPlot(0);
			base.setRoundBounds(axe, Array.min(plot0.getData())[axe], Array.max(plot0.getData())[axe]);
		} else { // build default min and max bounds
			double min = 0.0;
			double max = 0.0;
			if (base.getAxeScale(axe).equalsIgnoreCase(LINEAR)) {
				min = 0.0;
				max = 1.0;
			} else if (base.getAxeScale(axe).equalsIgnoreCase(LOGARITHM)) {
				min = 1.0;
				max = 10.0;
			}
			base.setRoundBounds(axe, min, max);
		}

		for (int i = 1; i < plots.size(); i++) {
			Plot ploti = this.getPlot(i);
			base.includeInBounds(axe, Array.min(ploti.getData())[axe]);
			base.includeInBounds(axe, Array.max(ploti.getData())[axe]);
		}
		resetBase();
		repaint();
	}

	public void resetBase() {
		// System.out.println("PlotCanvas.resetBase");
		draw.resetBaseProjection();
		grid.resetBase();

		for (int i = 0; i < objects.size(); i++) {
			if (objects.get(i) instanceof BaseDependant) {
				((BaseDependant) (objects.get(i))).resetBase();
			}
		}
		repaint();
	}

	// ///////////////////////////////////////////
	// ////// add/remove elements ////////////////
	// ///////////////////////////////////////////

	public void addLabel(String text, Color c, double... where) {
		addPlotable(new org.math.plot.plotObjects.Label(text, c, where));
	}

	public void addBaseLabel(String text, Color c, double... where) {
		addPlotable(new org.math.plot.plotObjects.BaseLabel(text, c, where));
	}

	public void addPlotable(Plotable p) {
		objects.add(p);
		// resetBase();
		repaint();
	}

	public void removePlotable(Plotable p) {
		objects.remove(p);
		repaint();
	}

	public void removePlotable(int i) {
		objects.remove(i);
		repaint();
	}

	public int addPlot(Plot newPlot) {
		plots.add(newPlot);
		//System.out.println("Number of points: " + plots.size());
		if(plots.size() == 7000){
			//reducePointsPlotted();
			removePlot(0);
		}
		if (linkedLegendPanel != null)
			linkedLegendPanel.updateLegends();
		if (plots.size() == 1)
			setAutoBounds();
		else
			includeInBounds(newPlot);
		return plots.size() - 1;
	}

/* Not currently used
	public void reducePointsPlotted(){
		System.out.println("Plots.size(): " + plots.size());
		for(int i = 5000; i > 0; i--){
			removePlot(i);
		}
		setAutoBounds();
	}
*/

	public void setPlot(int I, Plot p) {
		plots.setElementAt(p, I);
		if (linkedLegendPanel != null)
			linkedLegendPanel.updateLegends();
		repaint();
	}

	public void changePlotData(int I, double[]... XY) {
		getPlot(I).setData(XY);
		repaint();
	}

	public void changePlotName(int I, String name) {
		getPlot(I).setName(name);
		if (linkedLegendPanel != null)
			linkedLegendPanel.updateLegends();
		repaint();
	}

	public void changePlotColor(int I, Color c) {
		getPlot(I).setColor(c);
		if (linkedLegendPanel != null)
			linkedLegendPanel.updateLegends();
		repaint();
	}

	public void removePlot(int I) {
		plots.remove(I);
		if (linkedLegendPanel != null)
			linkedLegendPanel.updateLegends();
		if (plots.size() == 0) {
			initBasenGrid();
		} else {
			setAutoBounds();			}
	}

	public void removePlot(Plot p) {
		plots.remove(p);
		if (linkedLegendPanel != null)
			linkedLegendPanel.updateLegends();
		if (plots.size() == 0) {
			initBasenGrid();
		} else {
			setAutoBounds();
		}
	}

	public void removeAllPlots() {
		plots.removeAllElements();
		if (linkedLegendPanel != null)
			linkedLegendPanel.updateLegends();
		initBasenGrid();
	}

	public void addQuantiletoPlot(int numPlot, double[]... q) {
		getPlot(numPlot).addQuantiles(q);
	}

	public void addQuantiletoPlot(int numPlot, int numAxe, double... q) {
		getPlot(numPlot).addQuantile(numAxe, q);
	}

	public void addQuantiletoPlot(int numPlot, int numAxe, double q) {
		getPlot(numPlot).addQuantile(numAxe, q);
	}

	// ///////////////////////////////////////////
	// ////// call for toolbar actions ///////////
	// ///////////////////////////////////////////

	public void toGraphicFile(File file) throws IOException {

		Image image = createImage(getWidth(), getHeight());
		paint(image.getGraphics());
		image = new ImageIcon(image).getImage();

		BufferedImage bufferedImage = new BufferedImage(image.getWidth(null), image.getHeight(null), BufferedImage.TYPE_INT_RGB);
		Graphics g = bufferedImage.createGraphics();
		g.drawImage(image, 0, 0, Color.WHITE, null);
		g.dispose();

		try {
			ImageIO.write((RenderedImage) bufferedImage, "PNG", file);
		} catch (IllegalArgumentException ex) {
		}
	}

	public void displaySetScalesFrame() {
		new SetScalesFrame(this);
	}

	public void displayDatasFrame(int i) {
		DatasFrame df = new DatasFrame(this, linkedLegendPanel);
		df.panels.setSelectedIndex(i);
	}

	public void displayDatasFrame() {
		displayDatasFrame(0);
	}

	// ///////////////////////////////////////////
	// ////// Paint method ///////////////////////
	// ///////////////////////////////////////////

	// anti-aliasing constant
	final protected static RenderingHints AALIAS = new RenderingHints(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);

	public static Color NOTE_COLOR = Color.BLACK;

	public static Color EDIT_COLOR = Color.BLACK;

	public boolean allowEdit = false;

	public boolean allowNote = false;

	public boolean allowNoteCoord = false;

	protected double[] coordNoted;

	public void paint(Graphics gcomp) {
		// System.out.println("PlotCanvas.paint");

		Graphics2D gcomp2D = (Graphics2D) gcomp;

		// anti-aliasing methods
		gcomp2D.addRenderingHints(AALIAS);
		gcomp2D.setRenderingHint(RenderingHints.KEY_RENDERING, RenderingHints.VALUE_RENDER_QUALITY);

		gcomp2D.setColor(getBackground());
		gcomp2D.fillRect(0, 0, getSize().width, getSize().height);

		draw.initGraphics(gcomp2D);

		// draw plot
		grid.plot(draw);

		for (int i = 0; i < plots.size(); i++) {
			getPlot(i).plot(draw);
			if (linkedLegendPanel != null)
				linkedLegendPanel.nonote(i);
		}

		for (int i = 0; i < objects.size(); i++)
			getPlotable(i).plot(draw);

		// draw note
		if (allowNote) {
			if (allowNoteCoord && coordNoted != null) {
				draw.setColor(NOTE_COLOR);
				draw.drawCoordinate(coordNoted);
			}
			for (int i = 0; i < plots.size(); i++) {
				if (getPlot(i).noted) {
					if (linkedLegendPanel != null)
						linkedLegendPanel.note(i);
					getPlot(i).note(draw);
					//return;
				}
			}
		}
	}

	// ///////////////////////////////////////////
	// ////// Listeners //////////////////////////
	// ///////////////////////////////////////////

	public final static int ZOOM = 0;

	public final static int TRANSLATION = 1;

	public int ActionMode;

	protected boolean dragging = false;

	protected int[] mouseCurent = new int[2];

	protected int[] mouseClick = new int[2];

	public void mousePressed(MouseEvent e) {
		//System.out.println("PlotCanvas.mousePressed");
		/*
		 * System.out.println("PlotCanvas.mousePressed"); System.out.println("
		 * mouseClick = [" + mouseClick[0] + " " + mouseClick[1] + "]");
		 * System.out.println(" mouseCurent = [" + mouseCurent[0] + " " +
		 * mouseCurent[1] + "]");
		 */
		mouseCurent[0] = e.getX();
		mouseCurent[1] = e.getY();
		e.consume();
		mouseClick[0] = mouseCurent[0];
		mouseClick[1] = mouseCurent[1];
	}

	public void mouseDragged(MouseEvent e) {
		//System.out.println("PlotCanvas.mouseDragged");

		dragging = true;
		/*
		 * System.out.println("PlotCanvas.mouseDragged"); System.out.println("
		 * mouseClick = [" + mouseClick[0] + " " + mouseClick[1] + "]");
		 * System.out.println(" mouseCurent = [" + mouseCurent[0] + " " +
		 * mouseCurent[1] + "]");
		 */
		mouseCurent[0] = e.getX();
		mouseCurent[1] = e.getY();
		e.consume();
		switch (ActionMode) {
		case TRANSLATION:
			draw.translate(mouseCurent[0] - mouseClick[0], mouseCurent[1] - mouseClick[1]);
			mouseClick[0] = mouseCurent[0];
			mouseClick[1] = mouseCurent[1];
			repaint();
			break;
		case ZOOM:
			repaint();
			Graphics gcomp = getGraphics();
			gcomp.setColor(Color.black);
			gcomp.drawRect(min(mouseClick[0], mouseCurent[0]), min(mouseClick[1], mouseCurent[1]), abs(mouseCurent[0] - mouseClick[0]), abs(mouseCurent[1]
					- mouseClick[1]));
			break;
		}
		//repaint();
	}

	public void mouseReleased(MouseEvent e) {
		//System.out.println("PlotCanvas.mouseReleased");

		/*
		 * System.out.println("PlotCanvas.mouseReleased"); System.out.println("
		 * mouseClick = [" + mouseClick[0] + " " + mouseClick[1] + "]");
		 * System.out.println(" mouseCurent = [" + mouseCurent[0] + " " +
		 * mouseCurent[1] + "]");
		 */
		mouseCurent[0] = e.getX();
		mouseCurent[1] = e.getY();
		e.consume();
		switch (ActionMode) {
		case ZOOM:
			if (abs(mouseCurent[0] - mouseClick[0]) > 10 && abs(mouseCurent[1] - mouseClick[1]) > 10) {
				int[] origin = { min(mouseClick[0], mouseCurent[0]), min(mouseClick[1], mouseCurent[1]) };
				double[] ratio = { abs((double) (mouseCurent[0] - mouseClick[0]) / (double) panelSize[0]),
						abs((double) (mouseCurent[1] - mouseClick[1]) / (double) panelSize[1]) };
				draw.dilate(origin, ratio);
				repaint();
			}
			break;
		}
		//repaint();
	}

	public void mouseClicked(MouseEvent e) {
		//System.out.println("PlotCanvas.mouseClicked");

		/*
		 * System.out.println("PlotCanvas.mouseClicked"); System.out.println("
		 * mouseClick = [" + mouseClick[0] + " " + mouseClick[1] + "]");
		 * System.out.println(" mouseCurent = [" + mouseCurent[0] + " " +
		 * mouseCurent[1] + "]");
		 */
		mouseCurent[0] = e.getX();
		mouseCurent[1] = e.getY();
		e.consume();
		mouseClick[0] = mouseCurent[0];
		mouseClick[1] = mouseCurent[1];

		if (allowEdit) {
			if (e.getModifiers() == MouseEvent.BUTTON1_MASK && e.getClickCount() > 1) {
				for (int i = 0; i < grid.getAxes().length; i++)
					if (grid.getAxe(i).isSelected(mouseClick, draw) != null) {
						grid.getAxe(i).edit(this);
						return;
					}

				for (int i = 0; i < plots.size(); i++)
					if (getPlot(i).isSelected(mouseClick, draw) != null) {
						getPlot(i).edit(this);
						return;
					}
			}
		}

		if (!dragging && allowNote) {
			for (int i = 0; i < plots.size(); i++) {
				double[] _coordNoted = getPlot(i).isSelected(mouseClick, draw);
				if (e.getModifiers() == MouseEvent.BUTTON1_MASK) {
					if (_coordNoted != null) {
						getPlot(i).noted = !getPlot(i).noted;
					} else {
						getPlot(i).noted = false;
					}
				} else if (e.getModifiers() == MouseEvent.BUTTON3_MASK) {
					if (_coordNoted != null) {
						if (coordNoted != null) {
							boolean alreadyNoted = true;
							for (int j = 0; j < _coordNoted.length; j++)
								alreadyNoted = alreadyNoted && _coordNoted[j] == coordNoted[j];
							if (alreadyNoted) {
								coordNoted = null;
							} else {
								coordNoted = _coordNoted;
							}
						} else {
							coordNoted = _coordNoted;
						}
					}
				}
			}
			repaint();
		} else
			dragging = false;
	}

	public void mouseEntered(MouseEvent e) {
	}

	public void mouseExited(MouseEvent e) {
	}

	public void mouseMoved(MouseEvent e) {
		//System.out.println("PlotCanvas.mouseMoved");
		/*
		 * System.out.println("PlotCanvas.mouseClicked"); System.out.println("
		 * mouseClick = [" + mouseClick[0] + " " + mouseClick[1] + "]");
		 * System.out.println(" mouseCurent = [" + mouseCurent[0] + " " +
		 * mouseCurent[1] + "]");
		 */
		/*mouseCurent[0] = e.getX();
		mouseCurent[1] = e.getY();
		e.consume();
		if (allowNote) {
			for (int i = 0; i < plots.size(); i++) {
				double[] _coordNoted = getPlot(i).isSelected(mouseCurent, draw);
				if (_coordNoted != null) {
					getPlot(i).noted = !getPlot(i).noted;
				} else {
					getPlot(i).noted = false;
				}
			}
			repaint();
		}*/
	}

	public void mouseWheelMoved(MouseWheelEvent e) {
		//System.out.println("PlotCanvas.mouseWheelMoved");
		/*
		 * System.out.println("PlotCanvas.mouseWheelMoved");
		 * System.out.println(" mouseClick = [" + mouseClick[0] + " " +
		 * mouseClick[1] + "]"); System.out.println(" mouseCurent = [" +
		 * mouseCurent[0] + " " + mouseCurent[1] + "]");
		 */
		mouseCurent[0] = e.getX();
		mouseCurent[1] = e.getY();
		e.consume();
		int[] origin;
		double[] ratio;
		// double factor = 1.5;
		switch (ActionMode) {
		case ZOOM:
			if (e.getWheelRotation() == -1) {
				origin = new int[] { (int) (mouseCurent[0] - panelSize[0] / 3/* (2*factor) */), (int) (mouseCurent[1] - panelSize[1] / 3/* (2*factor) */) };
				ratio = new double[] { 0.666/* 1/factor, 1/factor */, 0.666 };
			} else {
				origin = new int[] { (int) (mouseCurent[0] - panelSize[0] / 1.333/* (2/factor) */),
						(int) (mouseCurent[1] - panelSize[1] / 1.333/* (2/factor) */) };
				ratio = new double[] { 1.5, 1.5 /* factor, factor */};
			}
			draw.dilate(origin, ratio);
			repaint();
			break;
		}
	}

	public void componentHidden(ComponentEvent e) {
	}

	public void componentMoved(ComponentEvent e) {
	}

	public void componentResized(ComponentEvent e) {
		//System.out.println("PlotCanvas.componentResized");
		panelSize = new int[] { (int) (getSize().getWidth()), (int) (getSize().getHeight()) };
		draw.resetBaseProjection();
		//System.out.println("PlotCanvas : "+panelSize[0]+" x "+panelSize[1]);
		repaint();
		linkedLegendPanel.componentResized(e);
	}

	public void componentShown(ComponentEvent e) {
	}

}
