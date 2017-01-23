/*
 * MainPanel5.java
 *
 * Created on April 11, 2008, 12:41 PM
 */
package treesaap.GraphicInterface.Panels;

import treesaap.Driver.DriverForPsoda;
import treesaap.Run.RunUsageBean;
import treesaap.Weka.ChemicalPropertiesException;
import treesaap.Weka.InvalidWekaFileException;
import treesaap.Weka.wekasim;
import java.awt.event.KeyEvent;
import java.io.File;
import java.text.DateFormat;
import java.util.Date;
import java.util.HashSet;
import java.util.Hashtable;
import java.util.Scanner;
import java.util.TreeMap;
import java.util.Vector;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.swing.AbstractListModel;
import javax.swing.JFileChooser;
import javax.swing.JOptionPane;
import javax.swing.ListModel;

/**
 *
 * @author  jayl
 */
public class MainPanel5 extends javax.swing.JPanel {

    // Global
    public String title = "TreeSAAP 3.2 for PSODA for Java 1.5";		//Allows version iteration to be viewed across entire program
    private DriverForPsoda driver;						//Allows indirect instantiation of this class
    private RunUsageBean runBean;
    // variables to store data from PSODA
    private String fileName;
    private Vector currentPropertyList;
    // properties are stored in data structure as strings, so through this lookup table, you can find what the index is (to highlight it in the JList)
    private TreeMap<String, Integer> propertyLookup;
    // relating to run parameters
    int slidingWindowSize;
    int slidingWindowIncrement;
    int numProcessors;

    public String getFileName() {
        return fileName;
    }

    public void setFileName(String fileName) {
        this.fileName = fileName;
    }

    /** Creates new form MainPanel5 */
    public MainPanel5() {
        initControls();
        initComponents();
    }

    /**
     * Constructor to be used when the filename of the tree is known (i.e. from within Psoda)
     * @param filename file name
     */
    public MainPanel5(String filename) {

        initControls();
        LoadTree(filename);
        ActivateControls();
        initComponents();
    }
    
    /**
     * Loads a file after the panel has been instantiated
     * @param filename file name to be loaded
     */
    public void loadFile(String filename)
    {
        LoadTree(filename);
    }

    /**
     * To be called when the parameters are set and the processing is ready to be run
     */
    private void ActivateControls() {
    	if (treeListComboBox.getItemCount() < 1)
    		return;
        treeListComboBox.setEnabled(true);
        runButton.setEnabled(true);
        runButton.setToolTipText(null);
	RefreshCurrentPropertyList();
        
    }
    
    /**
     * To be called when no trees are present
     */
    private void DeactivateControls()
    {
        treeListComboBox.setEnabled(false);
        runButton.setEnabled(false);
    }

    /**
     * 
     * @param filename
     */
    private void LoadTree(String filename) {
        fileNameLabel.setEnabled(true);
        this.fileName = filename;
        parseTree(filename);
        PopulateTreeNames();
        if (driver.getGDFP().getTreeNames().size() > 0)
        {
            fileNameLabel.setText("LOADED: " + filename);
            fileNameLabel.setToolTipText(fileName);
            ActivateControls();
        }
        else 
    	{
        	fileNameLabel.setText("No trees found in " + filename);
        	DeactivateControls();
    	}
    }

    private void initControls() {
        driver = DriverForPsoda.getDriver();

        runBean = driver.getRun().getBean(); // helps us set the parameters for run

        // load the default values from the bean
        slidingWindowIncrement = runBean.getIncrement();
        slidingWindowSize = runBean.getSlidingWindowSize();
        if (slidingWindowSize == -1) {
            slidingWindowSize = 15;
        }
        numProcessors = runBean.getProcessors();
        
        // This initializes the delete List of the data usage bean.  I don't know why this is so clunky...
        driver.getData().getBean().setDeletePropLists(new Vector());
        
        propertyLookup = new TreeMap<String, Integer>();
    }
    
    

    /**
     * Loads the tree into memory, mostly ported over from Driver.open(boolean newFile)
     * @param filename
     */
    private void parseTree(String filename) {

        long startTime = new Date().getTime();
        driver.getBean().logMessage("\nTime Started: " + (DateFormat.getDateTimeInstance(DateFormat.LONG, DateFormat.MEDIUM)).format(new Date()));

        if (filename != null && !filename.equals("")) {
            driver.getGDFP().openNewFile(fileName);
        }
        //Time
        long endTime = new Date().getTime();
        driver.getBean().logMessage("Time Ended: " + (DateFormat.getDateTimeInstance(DateFormat.LONG, DateFormat.MEDIUM)).format(new Date()));
        driver.getBean().logMessage("Run Time(s): " + ((endTime - startTime) / (double) 1000) + "\n");

    }

    /** This method is called from within the constructor to
     * initialize the form.
     * WARNING: Do NOT modify this code. The content of this method is
     * always regenerated by the Form Editor.
     */
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        jFileChooser1 = new javax.swing.JFileChooser();
        createListDialog = new javax.swing.JDialog();
        createListJButton = new javax.swing.JButton();
        createListField = new javax.swing.JTextField();
        jLabel5 = new javax.swing.JLabel();
        fileNameExistsDialog = new javax.swing.JDialog();
        jLabel6 = new javax.swing.JLabel();
        fileNameExistsDialogOK = new javax.swing.JButton();
        jPanel1 = new javax.swing.JPanel();
        browseFileButton = new javax.swing.JButton();
        fileNameLabel = new javax.swing.JLabel();
        jPanel2 = new javax.swing.JPanel();
        jScrollPane1 = new javax.swing.JScrollPane();
        propertyList = new javax.swing.JList();
        jPanel4 = new javax.swing.JPanel();
        propertyListComboBox = new javax.swing.JComboBox();
        createListButton = new javax.swing.JButton();
        deleteListButton = new javax.swing.JButton();
        saveListButton = new javax.swing.JButton();
        jPanel5 = new javax.swing.JPanel();
        propListSelectInverseButton = new javax.swing.JButton();
        jLabel1 = new javax.swing.JLabel();
        propListSelectAllButton = new javax.swing.JButton();
        propListSelectNoneButton = new javax.swing.JButton();
        jLabel2 = new javax.swing.JLabel();
        jPanel3 = new javax.swing.JPanel();
        runButton = new javax.swing.JButton();
        processorCountSpinner = new javax.swing.JSpinner();
        slidingWindowSizeSlider = new javax.swing.JSlider();
        jLabel3 = new javax.swing.JLabel();
        slidingWindowSizeBox = new javax.swing.JTextField();
        jLabel4 = new javax.swing.JLabel();
        treeListComboBox = new javax.swing.JComboBox();
        advancedOptionsButton = new javax.swing.JButton();

        jFileChooser1.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                jFileChooser1ActionPerformed(evt);
            }
        });

        createListDialog.setTitle("Enter New Property List Name");
        createListDialog.setName("createPropListDialog"); // NOI18N

        createListJButton.setText("Create List");
        createListJButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                createListJButtonActionPerformed(evt);
            }
        });

        createListField.setText("New List");
        createListField.addKeyListener(new java.awt.event.KeyAdapter() {
            public void keyTyped(java.awt.event.KeyEvent evt) {
                createListFieldKeyTyped(evt);
            }
        });

        jLabel5.setText("Name of new list:");

        org.jdesktop.layout.GroupLayout createListDialogLayout = new org.jdesktop.layout.GroupLayout(createListDialog.getContentPane());
        createListDialog.getContentPane().setLayout(createListDialogLayout);
        createListDialogLayout.setHorizontalGroup(
            createListDialogLayout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(org.jdesktop.layout.GroupLayout.TRAILING, createListDialogLayout.createSequentialGroup()
                .addContainerGap(61, Short.MAX_VALUE)
                .add(createListField, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, 226, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE)
                .add(18, 18, 18)
                .add(createListJButton)
                .addContainerGap())
            .add(createListDialogLayout.createSequentialGroup()
                .addContainerGap()
                .add(jLabel5)
                .addContainerGap(307, Short.MAX_VALUE))
        );
        createListDialogLayout.setVerticalGroup(
            createListDialogLayout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(createListDialogLayout.createSequentialGroup()
                .add(57, 57, 57)
                .add(jLabel5)
                .addPreferredGap(org.jdesktop.layout.LayoutStyle.UNRELATED)
                .add(createListDialogLayout.createParallelGroup(org.jdesktop.layout.GroupLayout.BASELINE)
                    .add(createListJButton)
                    .add(createListField, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE))
                .addContainerGap(95, Short.MAX_VALUE))
        );

        jLabel6.setText("You already have a list by that name");

        fileNameExistsDialogOK.setText("OK");
        fileNameExistsDialogOK.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                fileNameExistsDialogOKActionPerformed(evt);
            }
        });

        org.jdesktop.layout.GroupLayout fileNameExistsDialogLayout = new org.jdesktop.layout.GroupLayout(fileNameExistsDialog.getContentPane());
        fileNameExistsDialog.getContentPane().setLayout(fileNameExistsDialogLayout);
        fileNameExistsDialogLayout.setHorizontalGroup(
            fileNameExistsDialogLayout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(fileNameExistsDialogLayout.createSequentialGroup()
                .add(fileNameExistsDialogLayout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
                    .add(fileNameExistsDialogLayout.createSequentialGroup()
                        .add(100, 100, 100)
                        .add(jLabel6))
                    .add(fileNameExistsDialogLayout.createSequentialGroup()
                        .add(165, 165, 165)
                        .add(fileNameExistsDialogOK)))
                .addContainerGap(134, Short.MAX_VALUE))
        );
        fileNameExistsDialogLayout.setVerticalGroup(
            fileNameExistsDialogLayout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(fileNameExistsDialogLayout.createSequentialGroup()
                .add(39, 39, 39)
                .add(jLabel6)
                .addPreferredGap(org.jdesktop.layout.LayoutStyle.UNRELATED)
                .add(fileNameExistsDialogOK)
                .addContainerGap(23, Short.MAX_VALUE))
        );

        jPanel1.setBorder(javax.swing.BorderFactory.createTitledBorder("File"));

        browseFileButton.setText("Browse...");
        browseFileButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                browseFileButtonActionPerformed(evt);
            }
        });

        fileNameLabel.setHorizontalAlignment(javax.swing.SwingConstants.LEFT);
        fileNameLabel.setText("(file not loaded)");
        fileNameLabel.setEnabled(false);
        fileNameLabel.setFocusable(false);
        fileNameLabel.setHorizontalTextPosition(javax.swing.SwingConstants.RIGHT);

        org.jdesktop.layout.GroupLayout jPanel1Layout = new org.jdesktop.layout.GroupLayout(jPanel1);
        jPanel1.setLayout(jPanel1Layout);
        jPanel1Layout.setHorizontalGroup(
            jPanel1Layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(org.jdesktop.layout.GroupLayout.TRAILING, jPanel1Layout.createSequentialGroup()
                .addContainerGap()
                .add(fileNameLabel, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, 523, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED, 28, Short.MAX_VALUE)
                .add(browseFileButton)
                .addContainerGap())
        );
        jPanel1Layout.setVerticalGroup(
            jPanel1Layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(jPanel1Layout.createSequentialGroup()
                .add(jPanel1Layout.createParallelGroup(org.jdesktop.layout.GroupLayout.BASELINE)
                    .add(browseFileButton)
                    .add(fileNameLabel))
                .addContainerGap(org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
        );

        fileNameLabel.getAccessibleContext().setAccessibleName("");

        jPanel2.setBorder(javax.swing.BorderFactory.createTitledBorder("Properties"));

        propertyList.setModel(new javax.swing.AbstractListModel() {
            String[] strings = { "Item 1", "Item 2", "Item 3", "Item 4", "Item 5" };
            public int getSize() { return strings.length; }
            public Object getElementAt(int i) { return strings[i]; }
        });
        PopulatePropertyListItems();
        jScrollPane1.setViewportView(propertyList);

        jPanel4.setBorder(javax.swing.BorderFactory.createTitledBorder("List Management"));

        propertyListComboBox.setModel(new javax.swing.DefaultComboBoxModel(new String[] { "Item 1", "Item 2", "Item 3", "Item 4" }));
        PopulatePropertyListNames();
        propertyListComboBox.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                propertyListComboBoxActionPerformed(evt);
            }
        });

        createListButton.setText("Create New");
        createListButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                createListButtonActionPerformed(evt);
            }
        });

        deleteListButton.setText("Delete");
        deleteListButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                deleteListButtonActionPerformed(evt);
            }
        });

        saveListButton.setText("Save");
        saveListButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                saveListButtonActionPerformed(evt);
            }
        });

        org.jdesktop.layout.GroupLayout jPanel4Layout = new org.jdesktop.layout.GroupLayout(jPanel4);
        jPanel4.setLayout(jPanel4Layout);
        jPanel4Layout.setHorizontalGroup(
            jPanel4Layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(jPanel4Layout.createSequentialGroup()
                .addContainerGap()
                .add(createListButton)
                .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                .add(propertyListComboBox, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, 253, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED, 114, Short.MAX_VALUE)
                .add(saveListButton)
                .addPreferredGap(org.jdesktop.layout.LayoutStyle.UNRELATED)
                .add(deleteListButton)
                .add(6, 6, 6))
        );
        jPanel4Layout.setVerticalGroup(
            jPanel4Layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(jPanel4Layout.createSequentialGroup()
                .add(jPanel4Layout.createParallelGroup(org.jdesktop.layout.GroupLayout.BASELINE)
                    .add(createListButton)
                    .add(saveListButton)
                    .add(deleteListButton)
                    .add(propertyListComboBox, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE))
                .addContainerGap(org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
        );

        propListSelectInverseButton.setFont(new java.awt.Font("Dialog", 0, 9));
        propListSelectInverseButton.setText("Invert");
        propListSelectInverseButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                propListSelectInverseButtonActionPerformed(evt);
            }
        });

        jLabel1.setText("Select");

        propListSelectAllButton.setFont(new java.awt.Font("Dialog", 0, 9));
        propListSelectAllButton.setText("All");
        propListSelectAllButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                propListSelectAllButtonActionPerformed(evt);
            }
        });

        propListSelectNoneButton.setFont(new java.awt.Font("Dialog", 0, 9));
        propListSelectNoneButton.setText("None");
        propListSelectNoneButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                propListSelectNoneButtonActionPerformed(evt);
            }
        });

        org.jdesktop.layout.GroupLayout jPanel5Layout = new org.jdesktop.layout.GroupLayout(jPanel5);
        jPanel5.setLayout(jPanel5Layout);
        jPanel5Layout.setHorizontalGroup(
            jPanel5Layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(jPanel5Layout.createSequentialGroup()
                .addContainerGap()
                .add(jLabel1)
                .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                .add(propListSelectAllButton)
                .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                .add(propListSelectNoneButton)
                .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                .add(propListSelectInverseButton)
                .addContainerGap())
        );
        jPanel5Layout.setVerticalGroup(
            jPanel5Layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(jPanel5Layout.createParallelGroup(org.jdesktop.layout.GroupLayout.BASELINE)
                .add(propListSelectInverseButton)
                .add(jLabel1)
                .add(propListSelectAllButton)
                .add(propListSelectNoneButton))
        );

        jLabel2.setText("Select Properties from the List");

        org.jdesktop.layout.GroupLayout jPanel2Layout = new org.jdesktop.layout.GroupLayout(jPanel2);
        jPanel2.setLayout(jPanel2Layout);
        jPanel2Layout.setHorizontalGroup(
            jPanel2Layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(jPanel2Layout.createSequentialGroup()
                .addContainerGap()
                .add(jPanel2Layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
                    .add(jScrollPane1, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, 632, Short.MAX_VALUE)
                    .add(jLabel2)
                    .add(jPanel4, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                    .add(org.jdesktop.layout.GroupLayout.TRAILING, jPanel5, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE))
                .addContainerGap())
        );
        jPanel2Layout.setVerticalGroup(
            jPanel2Layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(jPanel2Layout.createSequentialGroup()
                .add(jLabel2)
                .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                .add(jScrollPane1, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                .add(jPanel5, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                .add(jPanel4, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE)
                .addContainerGap())
        );

        jPanel3.setBorder(javax.swing.BorderFactory.createTitledBorder("Process"));

        runButton.setText("Run");
        runButton.setToolTipText("Browse for a tree to parse first");
        runButton.setEnabled(false);
        runButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                runButtonActionPerformed(evt);
            }
        });

        processorCountSpinner.setValue(new Integer(numProcessors));

        slidingWindowSizeSlider.setValue(slidingWindowSize);
        slidingWindowSizeSlider.addChangeListener(new javax.swing.event.ChangeListener() {
            public void stateChanged(javax.swing.event.ChangeEvent evt) {
                slidingWindowSizeSliderStateChanged(evt);
            }
        });

        jLabel3.setText("Sliding Window Size");

        slidingWindowSizeBox.setText("jTextField1");
        slidingWindowSizeBox.setText(slidingWindowSize+ "");
        slidingWindowSizeBox.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                slidingWindowSizeBoxActionPerformed(evt);
            }
        });

        jLabel4.setText("Processors");

        treeListComboBox.setModel(new javax.swing.DefaultComboBoxModel(new String[] { "(none loaded)" }));
        treeListComboBox.setEnabled(false);

        org.jdesktop.layout.GroupLayout jPanel3Layout = new org.jdesktop.layout.GroupLayout(jPanel3);
        jPanel3.setLayout(jPanel3Layout);
        jPanel3Layout.setHorizontalGroup(
            jPanel3Layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(jPanel3Layout.createSequentialGroup()
                .addContainerGap()
                .add(jPanel3Layout.createParallelGroup(org.jdesktop.layout.GroupLayout.TRAILING, false)
                    .add(jPanel3Layout.createSequentialGroup()
                        .add(jLabel3)
                        .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                        .add(slidingWindowSizeBox, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE))
                    .add(org.jdesktop.layout.GroupLayout.LEADING, slidingWindowSizeSlider, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE))
                .add(jPanel3Layout.createParallelGroup(org.jdesktop.layout.GroupLayout.TRAILING)
                    .add(jPanel3Layout.createSequentialGroup()
                        .add(101, 101, 101)
                        .add(jLabel4)
                        .add(35, 35, 35)
                        .add(processorCountSpinner, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE)
                        .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED, 106, Short.MAX_VALUE)
                        .add(treeListComboBox, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE))
                    .add(jPanel3Layout.createSequentialGroup()
                        .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                        .add(runButton)))
                .addContainerGap())
        );
        jPanel3Layout.setVerticalGroup(
            jPanel3Layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(jPanel3Layout.createSequentialGroup()
                .add(13, 13, 13)
                .add(jPanel3Layout.createParallelGroup(org.jdesktop.layout.GroupLayout.BASELINE)
                    .add(slidingWindowSizeBox, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE)
                    .add(jLabel3)
                    .add(processorCountSpinner, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE)
                    .add(jLabel4)
                    .add(treeListComboBox, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE))
                .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                .add(jPanel3Layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
                    .add(slidingWindowSizeSlider, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE)
                    .add(runButton))
                .add(36, 36, 36))
        );

        advancedOptionsButton.setText("Advanced Options");
        advancedOptionsButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                advancedOptionsButtonActionPerformed(evt);
            }
        });

        org.jdesktop.layout.GroupLayout layout = new org.jdesktop.layout.GroupLayout(this);
        this.setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(layout.createSequentialGroup()
                .addContainerGap()
                .add(layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
                    .add(jPanel2, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                    .add(jPanel1, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE)
                    .add(jPanel3, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                    .add(advancedOptionsButton))
                .addContainerGap())
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(org.jdesktop.layout.GroupLayout.LEADING)
            .add(layout.createSequentialGroup()
                .addContainerGap()
                .add(jPanel1, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(org.jdesktop.layout.LayoutStyle.UNRELATED)
                .add(jPanel2, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, org.jdesktop.layout.GroupLayout.PREFERRED_SIZE)
                .addPreferredGap(org.jdesktop.layout.LayoutStyle.RELATED)
                .add(jPanel3, org.jdesktop.layout.GroupLayout.DEFAULT_SIZE, 126, Short.MAX_VALUE)
                .addPreferredGap(org.jdesktop.layout.LayoutStyle.UNRELATED)
                .add(advancedOptionsButton)
                .add(12, 12, 12))
        );
    }// </editor-fold>//GEN-END:initComponents
    private void propListSelectAllButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_propListSelectAllButtonActionPerformed

        RefreshCurrentPropertyList();

        Object selectedPropertyList = propertyListComboBox.getSelectedItem();
        if (selectedPropertyList != null) {
            driver.getBean().setProperties(selectedPropertyList.toString());
        } else {
            driver.getBean().setProperties("fromPsoda");
            driver.getData().getPropertyLists().put("fromPsoda", ReadInSelectedProperties());
        }
    }//GEN-LAST:event_propListSelectAllButtonActionPerformed

    private void propListSelectNoneButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_propListSelectNoneButtonActionPerformed
        propertyListSelectNone();
    }//GEN-LAST:event_propListSelectNoneButtonActionPerformed

    private void propListSelectInverseButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_propListSelectInverseButtonActionPerformed
        
        int[] currentlySelected = propertyList.getSelectedIndices();
        
        if(currentlySelected.length == 0) {
            propListSelectAllButtonActionPerformed(evt);
            return;
        }
        int max = propertyList.getModel().getSize();
        int[] toBeSelected = new int[max - currentlySelected.length];
        
        if(toBeSelected.length == 0) {
            propListSelectNoneButtonActionPerformed(evt);
            return;
        }
        
        HashSet currentlySelectedSet = new HashSet();
        for (int i = 0; i < currentlySelected.length; i++) {
            currentlySelectedSet.add(new Integer(currentlySelected[i]));

        }
        
        int index = 0;
        for (int i = 0; i < max; i++) {
            if(!currentlySelectedSet.contains(new Integer(i)))
            toBeSelected[index++] = i;
        }
        
        propertyList.setSelectedIndices(toBeSelected);
    }//GEN-LAST:event_propListSelectInverseButtonActionPerformed

    private void createListButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_createListButtonActionPerformed
        createListDialog.pack();
        createListDialog.setVisible(true);
    }//GEN-LAST:event_createListButtonActionPerformed

    private void createListJButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_createListJButtonActionPerformed
        String name = createListField.getText();
        
        if(driver.getData().getPropertyListNames().contains(name)) {
            fileNameExistsDialog.pack();
            fileNameExistsDialog.setVisible(true);
        } else {
            driver.getData().createLists(name, ReadInSelectedProperties());
            createListDialog.dispose();
            
            PopulatePropertyListNames();
            
            propertyListComboBox.setSelectedItem(name);
            
        }
    }//GEN-LAST:event_createListJButtonActionPerformed

    private void createListFieldKeyTyped(java.awt.event.KeyEvent evt) {//GEN-FIRST:event_createListFieldKeyTyped
        if(evt.isActionKey() && evt.getKeyCode() == KeyEvent.VK_ENTER) {
            //            CreateteNewList(createListField.getText());
        }
    }//GEN-LAST:event_createListFieldKeyTyped

    private void fileNameExistsDialogOKActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_fileNameExistsDialogOKActionPerformed
        fileNameExistsDialog.dispose();
    }//GEN-LAST:event_fileNameExistsDialogOKActionPerformed

    private void propertyListComboBoxActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_propertyListComboBoxActionPerformed
  
        RefreshCurrentPropertyList();

        Object selectedPropertyList = propertyListComboBox.getSelectedItem(); 
        if(selectedPropertyList != null) {
            driver.getBean().setProperties(selectedPropertyList.toString());
        }
        else {
            driver.getBean().setProperties("fromPsoda");
            driver.getData().getPropertyLists().put("fromPsoda", ReadInSelectedProperties());
        }
        
    }//GEN-LAST:event_propertyListComboBoxActionPerformed

    private void saveListButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_saveListButtonActionPerformed
        Hashtable lookup = driver.getData().getPropertyLists();
        lookup.put(propertyListComboBox.getSelectedItem(), ReadInSelectedProperties());
    }//GEN-LAST:event_saveListButtonActionPerformed

    private void deleteListButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_deleteListButtonActionPerformed
        
        Object deleted = propertyListComboBox.getSelectedItem();
        driver.getData().getBean().getDeletePropLists().add(deleted);
        
        // reflect that in the GUI
        propertyListComboBox.removeItem(deleted);
        driver.deletePropLists();
//        propertyListComboBox.setSelectedIndex(0);
    }//GEN-LAST:event_deleteListButtonActionPerformed

    private void slidingWindowSizeSliderStateChanged(javax.swing.event.ChangeEvent evt) {//GEN-FIRST:event_slidingWindowSizeSliderStateChanged
        slidingWindowSize = slidingWindowSizeSlider.getValue();
        slidingWindowSizeBox.setText(slidingWindowSize + "");
    }//GEN-LAST:event_slidingWindowSizeSliderStateChanged

    private void slidingWindowSizeBoxActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_slidingWindowSizeBoxActionPerformed
        slidingWindowSize = Integer.parseInt(slidingWindowSizeBox.getText());
        slidingWindowSizeSlider.setValue(slidingWindowSize);
    }//GEN-LAST:event_slidingWindowSizeBoxActionPerformed

    private void runButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_runButtonActionPerformed
         try {

            runBean.setSlidingWindowSize(slidingWindowSize);
            runBean.setProcessors(numProcessors);
            runBean.setIncrement(slidingWindowIncrement);

            driver.run((String) treeListComboBox.getSelectedItem());

            String outputDir = driver.getBean().getOutputDirectory();
            wekasim weka = new wekasim("treesaap"+File.separator+"Data"+File.separator+"SECONDARY_STRUCTURES"+File.separator+"secondarystructure.txt", driver.getData().getProperties());

            //Get nucleotide sequence
            String nucSeq = getAncestNucs();
            String secondStruct = weka.Simulate(nucSeq);
            
             //System.out.println("Second struct: " + secondStruct);
            AnalysisPanel ap = new AnalysisPanel(outputDir + File.separator, secondStruct);
            if (ap.hasResults())
                ap.showPanel();
            else 
            {
                System.out.println("has no results");
                JOptionPane pane = new JOptionPane("No properties were selected", JOptionPane.INFORMATION_MESSAGE);
                pane.setVisible(true);
                
            }
        } catch (InvalidWekaFileException ex) {
            Logger.getLogger(MainPanel5.class.getName()).log(Level.SEVERE, null, ex);
        } catch (ChemicalPropertiesException ex) {
            Logger.getLogger(MainPanel5.class.getName()).log(Level.SEVERE, null, ex);
        } 
    }//GEN-LAST:event_runButtonActionPerformed

    private void browseFileButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_browseFileButtonActionPerformed
        
        jFileChooser1.showDialog(this, "Select Tree");
    }//GEN-LAST:event_browseFileButtonActionPerformed

    private void advancedOptionsButtonActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_advancedOptionsButtonActionPerformed
        driver.DisplayGui();
    }//GEN-LAST:event_advancedOptionsButtonActionPerformed

    private void jFileChooser1ActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_jFileChooser1ActionPerformed
        if (evt.getActionCommand().equals(JFileChooser.APPROVE_SELECTION))
        {
            File selectedFile = jFileChooser1.getSelectedFile();
            if (selectedFile !=null && selectedFile.exists()) {
                this.fileName = selectedFile.getAbsolutePath();
                LoadTree(fileName);
            }
        }
    }//GEN-LAST:event_jFileChooser1ActionPerformed

    private String getAncestNucs()
    {
        try
        {
            Scanner s = new Scanner(new File(driver.getBean().getOutputDirectory() + File.separator + "AncestNucSeq.txt"));
            if (s.hasNextLine())
                s.nextLine(); //Name of sequence: Ancestral Node
            else return null;
            if (s.hasNextLine())
                return s.nextLine();
            else return null;
        }
        catch(Exception e)
        {
            driver.getBean().errorMessage(e.getMessage());
        }
        return null;
    }
    
    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JButton advancedOptionsButton;
    private javax.swing.JButton browseFileButton;
    private javax.swing.JButton createListButton;
    private javax.swing.JDialog createListDialog;
    private javax.swing.JTextField createListField;
    private javax.swing.JButton createListJButton;
    private javax.swing.JButton deleteListButton;
    private javax.swing.JDialog fileNameExistsDialog;
    private javax.swing.JButton fileNameExistsDialogOK;
    private javax.swing.JLabel fileNameLabel;
    private javax.swing.JFileChooser jFileChooser1;
    private javax.swing.JLabel jLabel1;
    private javax.swing.JLabel jLabel2;
    private javax.swing.JLabel jLabel3;
    private javax.swing.JLabel jLabel4;
    private javax.swing.JLabel jLabel5;
    private javax.swing.JLabel jLabel6;
    private javax.swing.JPanel jPanel1;
    private javax.swing.JPanel jPanel2;
    private javax.swing.JPanel jPanel3;
    private javax.swing.JPanel jPanel4;
    private javax.swing.JPanel jPanel5;
    private javax.swing.JScrollPane jScrollPane1;
    private javax.swing.JSpinner processorCountSpinner;
    private javax.swing.JButton propListSelectAllButton;
    private javax.swing.JButton propListSelectInverseButton;
    private javax.swing.JButton propListSelectNoneButton;
    private javax.swing.JList propertyList;
    private javax.swing.JComboBox propertyListComboBox;
    private javax.swing.JButton runButton;
    private javax.swing.JButton saveListButton;
    private javax.swing.JTextField slidingWindowSizeBox;
    private javax.swing.JSlider slidingWindowSizeSlider;
    private javax.swing.JComboBox treeListComboBox;
    // End of variables declaration//GEN-END:variables

    private void RefreshCurrentPropertyList() {
        Object currentProperty = propertyListComboBox.getSelectedItem();
        if (currentProperty == null || ((String) currentProperty).equals("")) {
            return;
        }

        Vector propList = ((Vector) driver.getData().getPropertyLists().get(currentProperty));
        driver.getBean().setProperties((String) currentProperty);

        if (propList != null) {

            propertyListSelectNone();

            int[] propIndices = new int[propList.size()];

            int i = 0;
            for (Object prop : propList) {
                propIndices[i++] = propertyLookup.get(prop).intValue();
            }

            propertyList.setSelectedIndices(propIndices);

        }
	else
	{ 
System.out.println("setting all set.");
       	    //Set all properties as selected
            int[] selected = new int[propertyList.getSize().width];
            for (int i = 0; i < selected.length; i++)
            {
        	selected[i] = i;
            }
            propertyList.setSelectedIndices(selected);
	}
    }

    private void propertyListSelectNone() {
        propertyList.removeSelectionInterval(0, propertyList.getModel().getSize() - 1);
    }
    
    
    private Vector ReadInSelectedProperties() {
       Vector propertyNames = new Vector();
       
        for (Object name : propertyList.getSelectedValues()) {
            propertyNames.addElement(name);
        }
        
        return propertyNames;

       
    }    
    private void PopulatePropertyListNames() {
        
        propertyListComboBox.removeAllItems();
        
        Vector propListNames = driver.getData().getPropertyListNames();

        for (Object listName : propListNames) {
            propertyListComboBox.addItem(listName);

        }
    } 
    
        private void PopulateTreeNames() {
        treeListComboBox.removeAllItems();
        
        Vector treeListNames = driver.getGDFP().getTreeNames();
        
        for (Object treeName : treeListNames) {
            treeListComboBox.addItem(treeName);
        }

    }
        
            private void PopulatePropertyListItems() {
        
        
        propertyList.removeAll();
        
        
        Vector properties = driver.getData().getPropertyNames();
                int counter = 0;
        for (Object prop : properties) {

            propertyLookup.put((String)prop, new Integer(counter++));
        }

        ListModel newModel = new AbstractListModelImpl();
            
        propertyList.setModel(newModel);
        
    }

    private class AbstractListModelImpl extends AbstractListModel {

        public AbstractListModelImpl() {
        }
        Vector propList = driver.getData().getPropertyNames();

        public int getSize() {
            return propList.size();
        }

        public Object getElementAt(int index) {

            return propList.elementAt(index);
        }
    }
}
