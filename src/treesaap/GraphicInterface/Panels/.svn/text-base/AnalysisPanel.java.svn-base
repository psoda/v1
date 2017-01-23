/**
 *	RunPanel.class is an instantiated object that creates
 *	a JPanel containing all the run options and choice of trees
 *	
 *	@author	Kendell Clement
 *	@version	1.0
 *	@since	1.0
 */
package treesaap.GraphicInterface.Panels;

import treesaap.Driver.DriverForPsoda;

import java.awt.Color;
import java.awt.Component;
import java.awt.Container;
import java.awt.Dimension;
import java.awt.GridBagConstraints;
import java.awt.GridBagLayout;
import java.awt.Insets;
import java.awt.Point;
import java.awt.Rectangle;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.AdjustmentEvent;
import java.awt.event.AdjustmentListener;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.io.File;
import java.io.FileNotFoundException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Comparator;
import java.util.HashMap;
import java.util.Scanner;
import java.util.logging.Level;
import java.util.logging.Logger;
import java.util.regex.Pattern;
import java.util.regex.Matcher;
import javax.swing.DefaultCellEditor;
import javax.swing.JButton;
import javax.swing.JPanel;
import javax.swing.JSplitPane;
import javax.swing.JScrollPane;
import javax.swing.JFrame;
import javax.swing.JComboBox;
import javax.swing.JLabel;
import javax.swing.table.DefaultTableCellRenderer;
import javax.swing.table.TableColumn;
import javax.swing.table.TableColumnModel;
import javax.swing.table.TableModel;
import javax.swing.JTable;
import javax.swing.JTextField;
import javax.swing.ScrollPaneConstants;
import javax.swing.table.DefaultTableModel;
import javax.swing.table.TableCellRenderer;

public class AnalysisPanel extends JFrame implements ActionListener {

    private double CRITICALVALUE = 3.19;
    private int highBreak = -1; //Breakpoint between high and mid for analysis
    private int lowBreak = -1; //Breakpoint between mid and low 
    private int categoryCount = -1; //Number of categories
    private JComboBox cbCategory; //Holds analysis categories (summary, each category)


    private JTable tableSpreadsheet; //Holds results
    private JTable tableRowTitles; //Holds row titles
    private JScrollPane scrollPane; //Scroll pane for right panel
    private JScrollPane titleScrollPane; //Scroll pane for left panel with row names
    private String resultsDirectory; //Form: /Users/Kendell/Documents/treesaap/trunk/Output/PAUP_1_Un8_3/
    private String secondaryStructure; //Secondary structure results
    
    private JPanel categoryColorPanel; //Holds the addional controls for formatting the analysis display
    private JComboBox cbHigh; //ComboBoxes for coloring high, mid, and low severity
    private JComboBox cbMid;
    private JComboBox cbLow;
    
    private JPanel highlightPanel; //Holds the controls for highlighting cells
    private JComboBox cbHighlight; //ComboBox for selecting color of highlighting
    private TableModel analysisModel; //Holds the information from the analysis... so we don't have to read it in every time..
    private TableModel analysisRowModel; //Holds row information for the analysis display
    
    private JComboBox taxabox; //Top left of Sequence Analysis table, allows user to select taxa to compare against
    private HashMap taxas; //Stores taxa names mapped to their sequences
    
    private JButton jbuttonZoomOut; //Zoom buttons. Global so they can be disabled when zooming isn't possible
    private JButton jbuttonZoomIn;
    private int colWidth; //Stores the column width 
    
    
    /**
     * Empty Constructor
     */
    public AnalysisPanel() {
    }

    /**
     * @param String
     *            rdirectory - path to the directory where the results are stored.
     */
    public AnalysisPanel(String rdirectory, String sStructure) {
        resultsDirectory = rdirectory;
        secondaryStructure = sStructure;
    }
    
    public boolean hasResults()
    {
        return new File(resultsDirectory + "Evpthwy" + File.separator + "Z-SigProp.txt").exists();
    }

    public void showPanel() {
        JFrame f = new JFrame("Results");
        Container content = f.getContentPane();
        content.add(createComponent());
        f.pack();
        f.setVisible(true);
    }

    /**
     * Creates all the objects to be placed on this panel, places them in
     * panelObjects
     */
    public Component createComponent() {
        try {
            JPanel topPanel = new JPanel();
            topPanel.setLayout(new GridBagLayout());
            GridBagConstraints c = new GridBagConstraints();
            c.gridx = 0;
            c.gridy = 0;
            c.insets = new Insets(30, 0, 10, 0);
            cbCategory = createCategoryComboBox();
            topPanel.add(cbCategory,c);
            c.insets = new Insets(0,0,0,0);
            c.gridy = 1;
            categoryColorPanel = createCategoryColorPanel();
            highlightPanel = createHighlightPanel();
            topPanel.add(categoryColorPanel,c);
            topPanel.add(highlightPanel,c);
            c.gridy = 2;
            JPanel zoomPanel = new JPanel();
            jbuttonZoomOut = new JButton("Zoom Out");
            jbuttonZoomOut.addActionListener(new zoomOutAction());
            zoomPanel.add(jbuttonZoomOut);
            jbuttonZoomIn = new JButton("Zoom In");
            jbuttonZoomIn.addActionListener(new zoomInAction());
            zoomPanel.add(jbuttonZoomIn);
            topPanel.add(zoomPanel,c);
            tableSpreadsheet = new JTable() {
                @Override
                public Class getColumnClass(int column) 
                { //Used for cell rendering
                    return getValueAt(1, column).getClass();
                }
                @Override
                public boolean isCellEditable(int row, int column)
                { //Disable editing so the double-click can be caught
                    return false;
                }
            };
            //Start analysis of the different files... so there isn't a wait for the user
            SequenceAnalysisThread t = new SequenceAnalysisThread();
            t.start();

            //Initialize spreadsheet table
            tableSpreadsheet.setPreferredScrollableViewportSize(new Dimension(600, 600));
            scrollPane = new JScrollPane(tableSpreadsheet);
            
            scrollPane.getVerticalScrollBar().addAdjustmentListener(new ScrollActionRight());
            tableSpreadsheet.setAutoResizeMode(JTable.AUTO_RESIZE_OFF);

            //Initialize rowTitles table
            tableRowTitles = new JTable() {
                @Override
                public Class getColumnClass(int column) {
                    return getValueAt(1, column).getClass();
                }
            }; //The getColumnClass has to be there for the cell rendering
            
            titleScrollPane = new JScrollPane(tableRowTitles);
            titleScrollPane.getVerticalScrollBar().addAdjustmentListener(new ScrollActionLeft());
            JSplitPane tablePane = new JSplitPane(JSplitPane.HORIZONTAL_SPLIT, titleScrollPane, scrollPane);
            tableRowTitles.setPreferredScrollableViewportSize(new Dimension(180, 600));
            TableCellRenderer renderer = new RightAlignRenderer();
            tableRowTitles.setDefaultRenderer(String.class, renderer);

            //Set contents of tables
            updateDisplay();
            
            return new JSplitPane(JSplitPane.VERTICAL_SPLIT, topPanel, tablePane);

        } catch (Exception e) {
            System.out.println("an error occured: " + e.getMessage());
        }
        return null;
    }

    public void actionPerformed(ActionEvent e) {
        if (e.getActionCommand().equals("comboBoxChanged")) {
            updateDisplay();
        }
    }
    
    //Responds to the click on the Zoom Out button and decreases the column widths
    public class zoomOutAction implements ActionListener
    {
        public void actionPerformed(ActionEvent e)
        {
            //Set column widths
            if (tableSpreadsheet.getColumnModel().getColumn(0).getPreferredWidth() < 2) //Don't shrink smaller than this..
            {
                jbuttonZoomOut.setEnabled(false);
            }
            jbuttonZoomIn.setEnabled(true);
            colWidth = colWidth / 2;
            for (int i = 0; i < tableSpreadsheet.getColumnModel().getColumnCount(); i++)
            {
                TableColumn thisCol = tableSpreadsheet.getColumnModel().getColumn(i);
                thisCol.setPreferredWidth(colWidth);
                thisCol.setMinWidth(0);
            }

        }
    };
    
    //Responds to the click on the Zooom In button
    public class zoomInAction implements ActionListener
    {
        public void actionPerformed(ActionEvent e)
        {
            //Set column widths
            if (tableSpreadsheet.getColumnModel().getColumn(0).getPreferredWidth() > 100) //Don't grow bigger than this..
            {
                jbuttonZoomIn.setEnabled(false);
            }
            jbuttonZoomOut.setEnabled(true);
            Rectangle viewableRect = scrollPane.getViewport().getViewRect();
            Point middlePoint = new Point((viewableRect.width / 2) + viewableRect.x,
                                            (viewableRect.height / 2) + viewableRect.y);
            int row = tableSpreadsheet.rowAtPoint(middlePoint);
            int col = tableSpreadsheet.columnAtPoint(middlePoint);
            colWidth = (colWidth * 2) + 1;
            for (int i = 0; i < tableSpreadsheet.getColumnModel().getColumnCount(); i++)
            {
                TableColumn thisCol = tableSpreadsheet.getColumnModel().getColumn(i);
                thisCol.setPreferredWidth(colWidth);
            }
            tableSpreadsheet.editCellAt(row, col);
        }
    };

    //Scrolls the spreadsheet table to center on the given cell
    public void scrollToCenter(int rowIndex, int vColIndex) 
    {
        Rectangle rect = tableSpreadsheet.getCellRect(rowIndex, vColIndex, true);
        // The location of the view relative to the table
        Rectangle viewRect = scrollPane.getViewport().getViewRect();
        rect.setLocation(rect.x-viewRect.x, rect.y-viewRect.y);
    
        // Calculate location of rect if it were at the center of view
        int centerX = (viewRect.width-rect.width)/2;
        int centerY = (viewRect.height-rect.height)/2;
        // Fake the location of the cell so that scrollRectToVisible
        // will move the cell to the center
        if (rect.x < centerX) {
            centerX = -centerX;
        }
        if (rect.y < centerY) {
            centerY = -centerY;
        }
        rect.translate(centerX, centerY);
    
        // Scroll the area into view.
        scrollPane.getViewport().scrollRectToVisible(rect);
    }
    
    private void updateDisplay() {
        String property = cbCategory.getSelectedItem().toString();
        colWidth = 30;
        if (property.equals("Summary")) {
            setSummaryTableModels();
            jbuttonZoomIn.setEnabled(false);
            jbuttonZoomOut.setEnabled(false);
            TableCellRenderer starRenderer = new starCellRenderer();
            tableSpreadsheet.setDefaultRenderer(String.class, starRenderer);
            TableCellRenderer renderer = new RightAlignRenderer();
            tableRowTitles.setDefaultRenderer(String.class, renderer);
            categoryColorPanel.setVisible(false);
            highlightPanel.setVisible(true);
        } else if (property.equals("Sequence Analysis")) {
            updateForSequenceAnalysis();
        } 
        else // For an individual category, just show the contents of the file...
        {
            updateForProperty(property);
        }
    }// createJTable
    
    private void updateForSequenceAnalysis() 
    {
        jbuttonZoomIn.setEnabled(true);
        jbuttonZoomOut.setEnabled(true);
        //Make sure table information has been created
        int failCount = 0;
        while (analysisModel == null)
        {
            try
            {
                Thread.sleep(10);
                System.out.println("..Still creating analysis panel");
            }
            catch(InterruptedException e){}
            if (failCount++ > 50)
            {
                analysisModel = new DefaultTableModel(new String[]{"Due to errors, this table could not be created."},1);
                analysisRowModel = new DefaultTableModel(new String[]{"Due to errors, this table could not be created."},1);
            }
        }
        //Set table information
        tableSpreadsheet.setModel(analysisModel);
        tableRowTitles.setModel(analysisRowModel);
        categoryColorPanel.setVisible(true);
        highlightPanel.setVisible(false);
            
        //Set column widths
        for (int i = 0; i < analysisModel.getColumnCount(); i++)
        {
            TableColumn thisCol = tableSpreadsheet.getColumnModel().getColumn(i);
            thisCol.setPreferredWidth(colWidth);
        }
        
        //Set highlighting stuff
        TableCellRenderer cvRenderer = new ValueRenderer();
        tableSpreadsheet.setDefaultRenderer(String.class, cvRenderer);
        
        //Add double-click feature to cells
        tableSpreadsheet.addMouseListener(new MouseAdapter(){
            @Override
            public void mouseClicked(MouseEvent evnt) {
                if (cbCategory.getSelectedItem() == "Sequence Analysis")
                {
                    int row = tableSpreadsheet.rowAtPoint(new Point(evnt.getX(), evnt.getY()));
                    int col = tableSpreadsheet.columnAtPoint(new Point(evnt.getX(), evnt.getY()));
                    if (evnt.getClickCount() > 1 && row > 2) 
                    {
                        String myProperty = tableRowTitles.getValueAt(row, 0).toString();
                        cbCategory.setSelectedItem(myProperty);
                        updateForProperty(myProperty);
                        scrollToCenter(row, col);
                    }
                }
            }
        });
        
        //Set combobox editor for row titles
        tableRowTitles.getColumnModel().getColumn(0).setCellEditor(new DefaultCellEditor(taxabox));
        tableRowTitles.getColumnModel().getColumn(0).setCellRenderer(new taxaComboBoxRenderer());        
        tableSpreadsheet.repaint();
                    
        pack();
        repaint();
    }
    
    //Responds to changing the taxa selection
    public class taxaComboBoxListener implements ActionListener {
        public void actionPerformed(ActionEvent e) {
            JComboBox cb = (JComboBox)e.getSource();
            String value = (String)cb.getSelectedItem();
            
            //Set row title value 
            analysisRowModel.setValueAt(value, 0, 0);
            tableRowTitles.setModel(analysisRowModel);
            tableRowTitles.repaint();
            
            String[] taxaAacids = (taxas.get(value)).toString().split(" ");

            //Set table amino acid values
            for (int i = 0; i < analysisModel.getColumnCount();i++)//analysisModel.getColumnCount(); i++)
            {
                int index = Integer.parseInt(analysisModel.getColumnName(i).toString());
                analysisModel.setValueAt(taxaAacids[index], 0, i);
            }
        }
    }

    //Renders the taxa selector as a JComboBox, while everything else is a Jlabel
    public class taxaComboBoxRenderer extends JComboBox implements TableCellRenderer 
    {
        public taxaComboBoxRenderer() {
            super(taxabox.getModel());
        }
        public Component getTableCellRendererComponent(JTable table, Object value,
                boolean isSelected, boolean hasFocus, int row, int column) {
            if (isSelected) {
                setForeground(table.getSelectionForeground());
                super.setBackground(table.getSelectionBackground());
            } else {
                setForeground(table.getForeground());
                setBackground(table.getBackground());
            }
            if(row == 0) //Render the combobox for row 0
            {// Select the current value
                setSelectedItem(value);
                return this;
            }
            else //Render as labels for the other rows
            {
                return new JLabel(value.toString());
            }
        }
    }
    
    private void updateForProperty(String property)
    {
        jbuttonZoomIn.setEnabled(true);
        jbuttonZoomOut.setEnabled(true);
        if (categoryCount < 0)
            categoryCount = DriverForPsoda.getDriver().getEvpthwy().getBean().getNumberOfCat();
            
        //Form of propList: From \t To \t category 1 value \t category 2 value \t ... 
        Object[] propList = parseFile(resultsDirectory + "Evpthwy" + File.separator + "SlidingWindow" + File.separator + property + ".txt", 2, categoryCount + 2);

        Object[][] results = new Object[categoryCount][propList.length];
        Object[][] rowNames = new Object[categoryCount][1];
        for (int i = 0; i < propList.length; i++) {
            for (int j = 0; j < categoryCount; j++)
            {
                rowNames[j][0] = "Category " + (j + 1);
                results[j][i] = ((Object[])propList[i])[j + 2]; //get property values
            }
        }
            
        //Add location for the header row
        String[] columnNames = new String[propList.length];

        Object[] infoRow = (Object[])propList[0];
        int windowStart = Integer.parseInt(infoRow[0].toString());
        int windowEnd = Integer.parseInt(infoRow[1].toString().replace("--> ", ""));
        int slidingWindowSize = windowEnd - windowStart;

        infoRow = (Object[])propList[1];
        int slidingWindowIncrement = Integer.parseInt(infoRow[0].toString()) - windowStart;

        int startingValue = Math.round(slidingWindowSize / 2);
        for (int col = 0; col < propList.length; col = col + slidingWindowIncrement) {
            columnNames[col]  = String.valueOf(col + startingValue);
        }

        tableSpreadsheet.setModel(new DefaultTableModel(results, columnNames));
        tableRowTitles.setModel(new DefaultTableModel(rowNames, new String[]{"Center of Sliding Window (width: " + slidingWindowSize + ")"}) {
            public boolean isCellEditable(int rowIndex, int mColIndex) {
                return false;
            }
        });
        TableCellRenderer cvRenderer = new CriticalValueRenderer();
        tableSpreadsheet.setDefaultRenderer(String.class, cvRenderer);
        categoryColorPanel.setVisible(false);
        highlightPanel.setVisible(true);
    }

    private void updateHighlightDisplay() {
        // Highlight the *'s if it is the summary
        if (cbCategory.getSelectedItem().toString().equals("Summary")) {

            TableColumnModel tcm = tableSpreadsheet.getColumnModel();
            TableColumn tc = tcm.getColumn(4);
            tc.setCellRenderer(new starCellRenderer());
        } // Otherwise highlight the critical values
        else {
            TableCellRenderer cvRenderer = new CriticalValueRenderer();
            tableSpreadsheet.setDefaultRenderer(String.class, cvRenderer);
        }
        tableSpreadsheet.repaint();
    }

    private void setSummaryTableModels() {
        
        String[] spreadsheetColumnNames = {"Category",
            "Value",
            ".05",
            ".01",
            ".001"
        };
        String[] titleColumnName = {"Property"};

        Object[] propList = parseFile(resultsDirectory + "Evpthwy" + File.separator + "Z-SigProp.txt", 1, 6);

        Object[][] spreadsheetResults = new Object[propList.length][5];
        Object[][] titleResults = new Object[propList.length][1];
        
        for (int i = 0; i < propList.length; i++) {
            Object[] thisPropRow = (Object[])propList[i];
            titleResults[i][0] = thisPropRow[0];
            for (int j = 1; j < thisPropRow.length; j++)
            {
                spreadsheetResults[i][j - 1] = thisPropRow[j];
            }
        }

        tableSpreadsheet.setModel(new DefaultTableModel(spreadsheetResults, spreadsheetColumnNames));
        tableRowTitles.setModel(new DefaultTableModel(titleResults, titleColumnName)  { //new table model does not allow editing
            public boolean isCellEditable(int rowIndex, int mColIndex) {
                return false;
            }
        });
    }

    /**
     * @return
     */
    class SequenceAnalysisThread extends Thread {

        @Override
        public void run() {
            try
            {
                //Create taxa box
                taxabox = createTaxaComboBox();
                taxabox.addActionListener(new taxaComboBoxListener());
                
                // Cycle through all of the output files that end with .txt and add the properties..
                File pathName = new File(resultsDirectory + "Evpthwy" + File.separator + "SlidingWindow");
                String[] fileNames = pathName.list();
                if (fileNames.length == 0) {
                    return;
                }
                
                // Create row titles as we go..
                int extraRows = 3; //How many rows are not properties (structure, sequence, etc.)
                ArrayList<String> rowNames = new ArrayList<String>();
                rowNames.add(taxabox.getItemAt(0).toString());
                rowNames.add("Ancestral Node");
                rowNames.add("Secondary Structure");
                
                for (int i = 1; i < fileNames.length; i++) {
                    String itemName = fileNames[i];
                    if (itemName.indexOf(".txt") == -1) {
                        continue;
                    }
                    rowNames.add(itemName.replace(".txt", ""));
                }
                //Set up number of categories and the high/low breaks
                categoryCount = DriverForPsoda.getDriver().getEvpthwy().getBean().getNumberOfCat();
                int span = Math.round((float)categoryCount / 3);
                highBreak = categoryCount - span;
                lowBreak = categoryCount - (2 * span);
                
                // The first three row names are From, To, and Structure, so the first category is the fourth item in the array
                Object[] propList = parseFile(resultsDirectory + "Evpthwy" + File.separator + "SlidingWindow" + File.separator + rowNames.get(extraRows) + ".txt", 2, categoryCount + 2);

                //Create results table
                Object[][] results = new Object[rowNames.size()][propList.length]; 

                // For all other categories, insert significant entries into the results table
                for (int row = extraRows; row < rowNames.size(); row++) {
                    propList = parseFile(resultsDirectory + "Evpthwy" + File.separator + "SlidingWindow" + File.separator + rowNames.get(row) + ".txt", 2, categoryCount + 2);
                    for (int col = 0; col < propList.length; col++) {
                        Object[] thisRow = (Object[])propList[col];

                        String displayString = "";
                        for (int k = categoryCount; k > 0; k--) { // for each of the categories in the important categories list, see if they are significant, and add them to the cell..
                            if (Double.parseDouble(thisRow[k + 1].toString()) >= CRITICALVALUE) //shift over two to the right because first two cells are sliding window
                            {
                                displayString += k + " ";
                            }
                        }
                        results[row][col] = displayString;
                    }
                }
                
                //Add location for the header
                Object[] infoRow = (Object[])propList[0];
                int windowStart = Integer.parseInt(infoRow[0].toString());
                int windowEnd = Integer.parseInt(infoRow[1].toString().replace("--> ", ""));
                int slidingWindowSize = windowEnd - windowStart;

                infoRow = (Object[])propList[1];
                int slidingWindowIncrement = Integer.parseInt(infoRow[0].toString()) - windowStart;
                Object[] header = new Object[propList.length];
                header[0] = "Center of Sliding Window (width: " + slidingWindowSize + ")"; 

                //Add secondary structures and amino acid sequences
                String[] ancestralAacids = taxas.get("Ancestral Node").toString().split(" ");
                String firstTaxa = taxas.keySet().toArray()[1].toString(); //holds name of taxa that is shown first
                String[] taxaAacids = taxas.get(firstTaxa).toString().split(" ");
                int startingValue = Math.round(slidingWindowSize / 2);
                int sequenceIndex = startingValue;
                for (int col = 0; col < propList.length; col++) {
                    results[0][col] = taxaAacids[sequenceIndex]; //user-selected taxa amino acid sequence
                    results[1][col] = ancestralAacids[sequenceIndex]; //Ancestral amino acid sequence
                    results[2][col] = secondaryStructure.charAt(sequenceIndex); // Set the secondary structure column to these values
                    header[col]  = col + startingValue;
                    sequenceIndex += slidingWindowIncrement;
                }
                
                analysisModel = new DefaultTableModel(results, header);
                
                //Create model for left row name table
                String[][] rowHeader = new String[rowNames.size()][1];
                for (int i = 0; i < rowNames.size(); i++)
                {
                    rowHeader[i][0] = rowNames.get(i); 
                }
                analysisRowModel = new DefaultTableModel(rowHeader, new String[]{"Center of Sliding Window (width: " + slidingWindowSize + ")"}){ //new table model does not allow editing
                    public boolean isCellEditable(int rowIndex, int mColIndex) {
                        if (rowIndex == 0)
                            return true;
                        else return false;
                    }
                };
                if(taxabox.getItemCount() > 1)
                    analysisRowModel.setValueAt(firstTaxa, 0, 0); //Set taxa combo box
            }
            catch (Exception e)
            {
                System.out.println("An error occured while creating the Analysis Panel." + e.getMessage() + e.getStackTrace());
                cbCategory.removeItem("Sequence Analysis");
            }
            
        }
    }
    
    // Listens for movement in the left scroll panel, and adjusts the right scroll panel
    public class ScrollActionLeft implements AdjustmentListener{
        public void adjustmentValueChanged(AdjustmentEvent ae){
            int value = ae.getValue();
            scrollPane.getVerticalScrollBar().setValue(value);
        }
    }

    // Listens for movement in the right scroll panel, and adjusts the left scroll panel
    public class ScrollActionRight implements AdjustmentListener{
        public void adjustmentValueChanged(AdjustmentEvent ae){
            int value = ae.getValue();
            titleScrollPane.getVerticalScrollBar().setValue(value);
            //The bottom item can't be viewed if there is a scrollbar there.. so this puts a scrollbar
            // in the left pane if there is one in the right bane when there is scrolling action
            if (scrollPane.getHorizontalScrollBar().isVisible())
                titleScrollPane.setHorizontalScrollBarPolicy(ScrollPaneConstants.HORIZONTAL_SCROLLBAR_ALWAYS);
            else titleScrollPane.setHorizontalScrollBarPolicy(ScrollPaneConstants.HORIZONTAL_SCROLLBAR_AS_NEEDED);
        }
    }
    
    //Creates the combo box where the user can select colors
    private JComboBox createColorComboBox() {
        return new JComboBox(new String[]{"Red", "Orange", "Yellow", "Green", "Blue", "None"});
    }

    //Creates the panel where the user selects colors to highlight high, mid, and low Sequence Analysis values
    private JPanel createCategoryColorPanel() {
        JPanel myPanel = new JPanel();
        JLabel label1 = new JLabel("Low:");
        cbLow = createColorComboBox();
        cbLow.setSelectedIndex(4);
        cbLow.addActionListener(new ActionListener() 
        {
            public void actionPerformed(ActionEvent e) {
                updateForSequenceAnalysis();
            }
        });
        JLabel label2 = new JLabel("Mid:");
        cbMid = createColorComboBox();
        cbMid.setSelectedIndex(3);

        cbMid.addActionListener(new ActionListener() 
        {
            public void actionPerformed(ActionEvent e) {
                updateForSequenceAnalysis();
            }
        });
        JLabel label3 = new JLabel("High:");
        cbHigh = createColorComboBox();
        cbHigh.addActionListener(new ActionListener() 
        {
            public void actionPerformed(ActionEvent e) {
                updateForSequenceAnalysis();
            }
        });
        myPanel.add(label1);
        myPanel.add(cbLow);
        myPanel.add(label2);
        myPanel.add(cbMid);
        myPanel.add(label3);
        myPanel.add(cbHigh);
        return myPanel;
    }
    
    //Creates the panel where the user selects the highlight color
    private JPanel createHighlightPanel()
    {
        JPanel myPanel = new JPanel();
        JLabel myLabel = new JLabel("Highlight Color:");
        cbHighlight = createColorComboBox();
        cbHighlight.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                updateHighlightDisplay();
            }
            });
        myPanel.add(myLabel);
        myPanel.add(cbHighlight);
        return myPanel;
    }

    //Creates the combobox that allows the user to select categories
    private JComboBox createCategoryComboBox() {
        JComboBox mybox = new JComboBox();

        File pathName = new File(resultsDirectory + "Evpthwy" + File.separator + "SlidingWindow");
        if (!pathName.isDirectory()) {
            System.out.println("The specified directory is not a valid output directory.");
            mybox.addItem("No results were found in " + pathName.getAbsolutePath());
            return mybox;
        }

        mybox.addItem("Summary");
        mybox.addItem("Sequence Analysis");

        String[] fileNames = pathName.list();
        for (int i = 0; i < fileNames.length; i++) {
            String itemName = fileNames[i];
            if (itemName.indexOf(".txt") == -1) {
                continue;
            }
            mybox.addItem(itemName.replace(".txt", ""));
        }

        mybox.addActionListener(this);

        return mybox;
    }
    
    //Creates the combo box for Sequence Analysis where the user selectes the taxa to view
    private JComboBox createTaxaComboBox()
    {
        JComboBox myBox = new JComboBox();
        try
        {
            taxas = new HashMap();
            //Read in sequence file (set in DriverForPsoda.printAacidToFile)
            Scanner taxaFile = new Scanner(new File(resultsDirectory + "TranslatedSeqs.txt"));
            //System.out.println("looking for file at " + resultsDirectory + "TranslatedSeqs.txt");
            while(taxaFile.hasNextLine())
            {
                String taxa = taxaFile.nextLine(); //Taxa name  
                String taxaSeq;
                if (taxaFile.hasNextLine())
                    taxaSeq = taxaFile.nextLine(); //Sequence
                else taxaSeq = "No Sequence Found for " + taxa;
                myBox.addItem(taxa);
                taxas.put(taxa, taxaSeq);
            }
            taxaFile.close();
        }
        catch(Exception e)
        {
            System.out.println("The translated sequences file (TranslatedSeqs.txt) could not be found");
        }
        return myBox;        
    }
    
    //Parses the information out of a file, and returns it as a 2-d array
    private Object[] parseFile(String fileName, int ignoreFirstLines, int columns) {
        try {
            Scanner scanner = new Scanner(new File(fileName));

            ArrayList<Object[]> list = new ArrayList<Object[]>();
            for (int i = 0; i < ignoreFirstLines; i++) {
                scanner.nextLine();
            } // ignore first couple of lines
            while (scanner.hasNext()) {
                String nex = scanner.nextLine();
                String[] tokens = new String[columns];
                String[] otherTokens = nex.split("\t");
                for (int i = 0; i < columns; i++) {
                    if (otherTokens.length > i) {
                        tokens[i] = otherTokens[i];
                    } else {
                        tokens[i] = " ";
                    }
                }
                list.add(tokens);
            }
            scanner.close();
            Object[] array = list.toArray();
            Arrays.sort(array,new ArrayComparator());
            return array;
        } catch (FileNotFoundException e) {
            return null;
        }
    }
    
    class ArrayComparator implements Comparator
    {
    	public final int compare(Object a, Object b)
    	{
    		return (((String[])a)[0]).compareTo(((String[])b)[0]);
    	}
    }
    
    //Renderer for highlighting significant values
    private class starCellRenderer extends DefaultTableCellRenderer {
        @Override
        public Component getTableCellRendererComponent(JTable table, Object value, boolean isSelected,
                boolean hasFocus, int row, int column) {
            super.getTableCellRendererComponent(table, value, false, false, row, column);
            if (column == 4 && value.toString().equals("*")) {
                setBackground(getColor(cbHighlight.getSelectedItem().toString()));
            } else {
                setBackground(Color.WHITE);
            }
            return this;
        }
    }

    //Renderer for Sequence Analysis, highlighting high, mid, and low values
    class ValueRenderer extends DefaultTableCellRenderer {
        @Override
        public Component getTableCellRendererComponent(JTable table, Object value,
                boolean isSelected, boolean hasFocus, int row, int column) {
            super.getTableCellRendererComponent(table, value, isSelected, hasFocus, row, column);
            String valueString = value.toString().replace(" ", "");
            if (row < 3 || valueString.length() < 1)
            {
                if (row == 0) //Matching amino acid sequences
                {
                    if (!tableSpreadsheet.getValueAt(1, column).equals(value))
                    {
                        setBackground(Color.GRAY);
                        return this;
                    }
                }
                setBackground(table.getBackground());
                return this;
            }
            
	    Pattern pattern = Pattern.compile("\\s*(\\d+)\\s+");
	    Matcher matcher = pattern.matcher(value.toString());
	    int firstValue = 0;
            try
            {
		if (matcher.find())
			firstValue = Integer.parseInt(matcher.group().toString().trim());
            }
            catch(NumberFormatException e)
            {return this;}
            if (firstValue <= lowBreak)
                setBackground(getColor(cbLow.getSelectedItem().toString()));
            else if (firstValue <= highBreak)
                setBackground(getColor(cbMid.getSelectedItem().toString()));
            else 
                setBackground(getColor(cbHigh.getSelectedItem().toString()));
            return this;
        }
    }

    //Returns a color object from a string
    private Color getColor(String color) {
        if (color.equals("Red")) {
            return new Color(255,51,51);
            //return new Color(204,0,102);//Pastelly
        }
        if (color.equals("Orange")) {
            return new Color(245,122,0);
            //return new Color(204,102,0);
        }
        if (color.equals("Yellow")) {
            return new Color(255,255,51);
            //return new Color(204,204,0);
        }
        if (color.equals("Green")) {
            return new Color(153,255,51);
            //return new Color(102,204,0);
        }
        if (color.equals("Blue")) {
            return new Color(51,51,255);
            //return new Color(0,102,204);
        } else {
            return Color.WHITE;
        }
    }

    //Renderer for highligting significant values >= than CRITICIALVALUE
    class CriticalValueRenderer extends DefaultTableCellRenderer {

        @Override
        public Component getTableCellRendererComponent(JTable table, Object value,
                boolean isSelected, boolean hasFocus, int row, int column) {
            super.getTableCellRendererComponent(table, value, isSelected, hasFocus, row, column);
            try {
                if (Double.parseDouble(value.toString()) >= CRITICALVALUE) {
                    setBackground(getColor(cbHighlight.getSelectedItem().toString()));
                } else {
                    setBackground(table.getBackground());
                }
            } catch (NumberFormatException e) {
            }

            return this;
        }
    }
    
    //Renderer for aligning row headers right
    class RightAlignRenderer extends DefaultTableCellRenderer {

        @Override
        public Component getTableCellRendererComponent(JTable table, Object value,
                boolean isSelected, boolean hasFocus, int row, int column) {
            super.getTableCellRendererComponent(table, value, isSelected, hasFocus, row, column);
            
            setHorizontalAlignment(JLabel.RIGHT);
            return this;
        }
    }
}// AnalysisPanel

