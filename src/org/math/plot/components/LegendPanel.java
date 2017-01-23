package org.math.plot.components;

import java.awt.*;
import java.awt.event.*;

import javax.swing.*;

import org.math.plot.*;
import org.math.plot.canvas.*;
import org.math.plot.plots.*;

/**
 * BSD License
 * 
 * @author Yann RICHET
 */

public class LegendPanel extends JPanel implements ComponentListener {

	private static final long serialVersionUID = 1L;

	PlotCanvas plotCanvas;

	PlotPanel plotPanel;

	Legend[] legends;

	public static int INVISIBLE = -1;

	public static int VERTICAL = 0;

	public static int HORIZONTAL = 1;

	int orientation;

	private int maxHeight;

	private int maxWidth;

	JPanel container;

	private int inset = 5;

	public LegendPanel(PlotPanel _plotPanel, int _orientation) {
		plotPanel = _plotPanel;
		plotCanvas = plotPanel.plotCanvas;
		plotCanvas.attachLegend(this);
		orientation = _orientation;

		container = new JPanel();
		container.setBackground(plotCanvas.getBackground());
		container.setLayout(new GridLayout(1, 1, inset, inset));
		//container.setBorder(BorderFactory.createCompoundBorder(BorderFactory.createLineBorder(Color.BLACK, 1),null));

		updateLegends();

		setBackground(plotCanvas.getBackground());
		addComponentListener(this);
		//setLayout(new GridBagLayout());

		add(container);
	}

	public void updateLegends() {
		if (orientation != INVISIBLE) {
			container.removeAll();
			Plot[] plots = plotCanvas.getPlots();

			maxHeight = 1;
			maxWidth = 1;

			legends = new Legend[plots.length];
			for (int i = 0; i < plots.length; i++) {
				legends[i] = new Legend(plots[i]);

				maxWidth = (int) Math.max(maxWidth, legends[i].getPreferredSize().getWidth());
				maxHeight = (int) Math.max(maxHeight, legends[i].getPreferredSize().getHeight());

				container.add(legends[i]);
			}
			updateSize();
			//repaint();
		}
	}

	private void updateSize() {
		//System.out.println("LegendPanel.updateSize");
		if (orientation == VERTICAL) {
			int nh = 1;
			if (maxHeight < plotCanvas.panelSize[1]/*getHeight()*/)
				nh = plotCanvas.panelSize[1]/*getHeight()*/ / (maxHeight + inset);
			int nw = 1 + legends.length / nh;

			((GridLayout) (container.getLayout())).setColumns(nw);
			((GridLayout) (container.getLayout())).setRows(1 + legends.length / nw);

			container.setPreferredSize(new Dimension((maxWidth + inset) * nw, (maxHeight + inset) * (1 + legends.length / nw)));

		} else if (orientation == HORIZONTAL) {
			int nw = 1;
			if (maxWidth < plotCanvas.panelSize[0]/*getWidth()*/)
				nw = plotCanvas.panelSize[0]/*getWidth()*/ / (maxWidth + inset);
			int nh = 1 + legends.length / nw;

			((GridLayout) (container.getLayout())).setRows(nh);
			((GridLayout) (container.getLayout())).setColumns(1 + legends.length / nh);

			container.setPreferredSize(new Dimension((maxWidth + inset) * (1 + legends.length / nh), (maxHeight + inset) * nh));
		}
		container.updateUI();
	}

	public void note(int i) {
		if (orientation != INVISIBLE) {
			legends[i].setBackground(PlotCanvas.NOTE_COLOR);
			legends[i].name.setForeground(plotPanel.getBackground());
		}
	}

	public void nonote(int i) {
		if (orientation != INVISIBLE) {
			legends[i].setBackground(plotPanel.getBackground());
			legends[i].name.setForeground(PlotCanvas.NOTE_COLOR);
		}
	}

	public void componentResized(ComponentEvent e) {
		//System.out.println("LegendPanel.componentResized");
		//System.out.println("PlotCanvas : "+plotCanvas.panelSize[0]+" x "+plotCanvas.panelSize[1]);
		if (orientation != INVISIBLE) {
			updateSize();
		}
	}

	public void componentMoved(ComponentEvent e) {
	}

	public void componentShown(ComponentEvent e) {
	}

	public void componentHidden(ComponentEvent e) {
	}

	public class Legend extends JPanel {

		private static final long serialVersionUID = 1L;

		JPanel color;

		JLabel name;

		Plot plot;

		public Legend(Plot p) {
			plot = p;

			setLayout(new BorderLayout(2, 2));

			color = new JPanel();
			name = new JLabel();

			setBackground(Color.WHITE);

			update();

			add(color, BorderLayout.WEST);
			add(name, BorderLayout.CENTER);

			name.addMouseListener(new MouseListener() {
				public void mouseClicked(MouseEvent e) {
					if (e.getModifiers() == MouseEvent.BUTTON1_MASK)
						if (plotCanvas.allowEdit && e.getClickCount() > 1)
							editText();
					if (plotCanvas.allowNote && e.getClickCount() <= 1)
						note_nonote();
				}

				public void mousePressed(MouseEvent e) {
				}

				public void mouseReleased(MouseEvent e) {
				}

				public void mouseEntered(MouseEvent e) {
				}

				public void mouseExited(MouseEvent e) {
				}
			});

			color.addMouseListener(new MouseListener() {
				public void mouseClicked(MouseEvent e) {
					if (e.getModifiers() == MouseEvent.BUTTON1_MASK)
						if (plotCanvas.allowEdit && e.getClickCount() > 1)
							editColor();
					if (plotCanvas.allowNote && e.getClickCount() <= 1)
						note_nonote();
				}

				public void mousePressed(MouseEvent e) {
				}

				public void mouseReleased(MouseEvent e) {
				}

				public void mouseEntered(MouseEvent e) {
				}

				public void mouseExited(MouseEvent e) {
				}
			});
		}

		public void editText() {
			String name1 = JOptionPane.showInputDialog(plotCanvas, "Choose name", plot.getName());
			if (name1 != null) {
				plot.setName(name1);
				update();
				updateLegends();
			}
		}

		public void editColor() {
			Color c = JColorChooser.showDialog(plotCanvas, "Choose plot color", plot.getColor());
			if (c != null) {
				plot.setColor(c);
				update();
				plotCanvas.repaint();
			}
		}

		public void update() {
			int size = name.getFont().getSize();
			color.setSize(new Dimension(size, size));
			color.setPreferredSize(new Dimension(size, size));

			// TODO change legend when plot is invisible
			/*if (!plot.visible)
				color.setBackground(Color.LIGHT_GRAY);
			else*/
				color.setBackground(plot.getColor());

			/*if (!plot.visible) {
				name.setFont(name.getFont().deriveFont(Font.ITALIC));
				name.setForeground(Color.LIGHT_GRAY);
			}*/
			name.setText(plot.getName());
			repaint();
		}

		public void note_nonote() {
			plot.noted = !plot.noted;
			plotCanvas.repaint();
		}

	}

}