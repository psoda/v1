package gui;

import javax.swing.AbstractAction;
import javax.swing.Action;
import javax.swing.undo.CannotRedoException;
import javax.swing.undo.CannotUndoException;
import javax.swing.undo.UndoManager;

import java.awt.event.ActionEvent;

public class UndoRedo{

	private UndoAction undoAction = new UndoAction();
	private RedoAction redoAction = new RedoAction();
	private MainTabs psodaTabs;

	public UndoRedo(MainTabs psodaTabs){
		this.psodaTabs = psodaTabs;
	}

	public void updateUndoState(){
		undoAction.updateUndoState();
	}
	public void updateRedoState(){
		redoAction.updateRedoState();
	}

	public UndoAction getUndoAction(){
		return undoAction;
	}
	public RedoAction getRedoAction(){
		return redoAction;
	}

	/**
	 * UndoAction Class
	 * Used for Undos on documents
	 *
	 * January 12, 2007
	 */
	class UndoAction extends AbstractAction {

		private static final long serialVersionUID = -7387614106995496060L;

		public UndoAction() {
			super("Undo");
			setEnabled(false);
		}

		public void actionPerformed(ActionEvent e) {
			String path = (String)psodaTabs.getInputTabsCurrentDocument().getProperty("path");
			MyUndoableEditListener undoableListener = psodaTabs.getUndoableEditListener();
			UndoManager undo = undoableListener.getUndoManager(path);
			try {
				undo.undo();
			} catch (CannotUndoException ex) {
				System.out.println("Unable to undo: " + ex);
				ex.printStackTrace();
			}
			updateUndoState();
			redoAction.updateRedoState();
		}

		protected void updateUndoState() {
			String path = (String)psodaTabs.getInputTabsCurrentDocument().getProperty("path");
			MyUndoableEditListener undoableListener = psodaTabs.getUndoableEditListener();
			UndoManager undo = undoableListener.getUndoManager(path);
			if(undo != null){
				if (undo.canUndo()) {
					setEnabled(true);
					putValue(Action.NAME, undo.getUndoPresentationName());
				} else {
					setEnabled(false);
					putValue(Action.NAME, "Undo");
				}
			}
		}
	}

	/**
	 * RedoAction Class
	 * Used for Redos on documents
	 *
	 * January 12, 2007
	 */
	class RedoAction extends AbstractAction {

		private static final long serialVersionUID = 5678492830487053685L;
		public RedoAction() {
			super("Redo");
			setEnabled(false);
		} 
		public void actionPerformed(ActionEvent e) {
			String path = (String)psodaTabs.getInputTabsCurrentDocument().getProperty("path");
			MyUndoableEditListener undoableListener = psodaTabs.getUndoableEditListener();
			UndoManager undo = undoableListener.getUndoManager(path);
			try {
				undo.redo();
			} catch (CannotRedoException ex) {
				System.out.println("Unable to redo: " + ex);
				ex.printStackTrace();
			}
			updateRedoState();
			undoAction.updateUndoState();
		}
		protected void updateRedoState() {
			String path = (String)psodaTabs.getInputTabsCurrentDocument().getProperty("path");
			MyUndoableEditListener undoableListener = psodaTabs.getUndoableEditListener();
			UndoManager undo = undoableListener.getUndoManager(path);
			if(undo != null){
				if (undo.canRedo()) {
					setEnabled(true);
					putValue(Action.NAME, undo.getRedoPresentationName());
				} else {
					setEnabled(false);
					putValue(Action.NAME, "Redo");
				}
			}
		}
	}
}
