package gui;

import javax.swing.undo.UndoManager;
import javax.swing.undo.UndoableEdit;


 /* Will hold an UndoManager and identifier for
  * each document.
  */
public class UndoableContainer {
	
	private String docPath = "";
	UndoManager undo;
	public UndoableContainer(String path){
		docPath = path;
		undo = new UndoManager();
	}
	
	public String getPath(){
		return docPath;
	}

	public void setPath(String path){
		docPath = path;
	}
	
	public void addEdit(UndoableEdit edit){
		undo.addEdit(edit);
	}
	
	public UndoManager getUndoManger(){
		return undo;
	}
}
