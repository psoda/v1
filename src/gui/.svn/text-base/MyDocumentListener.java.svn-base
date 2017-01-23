package gui;

import javax.swing.event.DocumentEvent;
import javax.swing.event.DocumentListener;

/**
 * DocumentListener Class
 * Used for listening for Document
 * changes
 *
 * January 12, 2007
 */
public class MyDocumentListener implements DocumentListener{
	MainTabs psodaTabs;

	public MyDocumentListener(MainTabs psodaTabs){
		this.psodaTabs = psodaTabs;
	}
	public void changedUpdate(DocumentEvent arg0) {
				// NOT NECESSARY //
	}

	public void insertUpdate(DocumentEvent e) {
		psodaTabs.setFileNotSaved(e.getDocument());
	}

	public void removeUpdate(DocumentEvent e) {
		psodaTabs.setFileNotSaved(e.getDocument());
	}
	
}

