package gui;

import java.util.ArrayList;

import javax.swing.text.Document;
import javax.swing.JOptionPane;

import javax.swing.undo.UndoManager;
import javax.swing.event.UndoableEditEvent;
import javax.swing.event.UndoableEditListener;

/**
 * UndoableEditListener Class
 * Used for allowing undos on Documents
 * changes
 *
 * January 12, 2007
 */
public class MyUndoableEditListener implements UndoableEditListener {
	UndoRedo undoRedo;
	MainTabs psodaTabs;

	public MyUndoableEditListener(MainTabs psodaTabs, UndoRedo undoRedo){
		this.psodaTabs = psodaTabs;
		this.undoRedo = undoRedo;
	}

	// This method will find the container holding the undoManager for a given
	// Document and update the undo and redo states
	ArrayList<UndoableContainer> containerList = new ArrayList<UndoableContainer>();
	public void undoableEditHappened(UndoableEditEvent e) {
		Document doc = psodaTabs.getInputTabsCurrentDocument();
		String path = (String) doc.getProperty("path");
		for(int i = 0; i < containerList.size(); i++){
			if(((UndoableContainer)containerList.get(i)).getPath().equals(path)){
				containerList.get(i).addEdit(e.getEdit());
			}
		}
			undoRedo.updateUndoState();
			undoRedo.updateRedoState();
	}
	
	public void addContainer(UndoableContainer container){
		containerList.add(container);
	}

	public void removeContainer(UndoableContainer container){
		containerList.remove(container);
	}

	public UndoableContainer getContainer(String path){
		for(int i = 0; i < containerList.size(); i++){
			if( ((UndoableContainer)(containerList.get(i))).getPath().equals(path) ){
				return containerList.get(i);	
			}
		}
		System.out.println("getContainer -> PATH: " + path);
		JOptionPane.showMessageDialog(null, "SYSTEM ERROR: getContainer Returned NULL.\nPlease report this bug", "SYSTEM ERROR", JOptionPane.ERROR_MESSAGE);
		return null;
	}
	
	public UndoManager getUndoManager (String path){
		for(int i = 0; i < containerList.size(); i++){
				if(((UndoableContainer)(containerList.get(i))).getPath().equals(path)){
				return containerList.get(i).getUndoManger();
			}
		}
		System.out.println("getUndoManager -> PATH: " + path);
		JOptionPane.showMessageDialog(null, "SYSTEM ERROR: getUndoManager Returned NULL.\nPlease report this bug\n" + path, "SYSTEM ERROR", JOptionPane.ERROR_MESSAGE);
		return null;
	}
}
