package gui;

import java.util.ArrayList;
import java.util.Observable;
import java.util.TreeMap;
import java.util.Map;

public class GuiCommandNode extends Observable {

	/**
	 * The name of this command
	 */
	private String commandName;

	/**
	 * The name/value pairs that will be used as the parameters for this command
	 */
	private Map<String, String> params;

	public native static void runCommand(String command);
	public native static String toCode(String commandName, String[] paramNames,
			String[] paramValues);
	public native static String[] getParamNames(String commandName);
	public native static String[] getParamDescriptions(String commandName);
	public native static String[] getParamOptions(String commandName,
			String paramName);
	public native static String[] getFileParamNames(String commandName);
	public native static String[] getDefaultValues(String commandName);

	public GuiCommandNode() {
		commandName = "";
		params = new TreeMap<String, String>();
		setChanged();
		notifyObservers();
	}

	public void setCommandName(String name) {
		commandName = name;
		setChanged();
		notifyObservers();
	}

	public void populateNodeWithCommandDefaults() {
		populateNodeWithCommandDefaults(commandName);
	}

	public void populateNodeWithCommandDefaults(String name) {
		setCommandName(name);
		String[] paramNames = getParamNames(commandName);
		String[] defaultValues = getDefaultValues(commandName);
		int numParams = paramNames.length;
		params = new TreeMap<String, String>();
		for (int i = 0; i < numParams; i++) {
			params.put(paramNames[i], defaultValues[i]);
		}
		setChanged();
		notifyObservers();
	}

	/*
	 * public void populateNode(String psodaCode) { setChanged();
	 * notifyObservers(); }
	 */

	public void execute() {
		ExecutePsodaCodeThread runningThread = new ExecutePsodaCodeThread(
				toCode());
		runningThread.start();
	}

	public String toCode() {
		String[] paramNames = new String[params.size()];
		String[] paramValues = new String[params.size()];
		int i = 0;
		for (Map.Entry<String, String> thisEntry : params.entrySet()) {
			paramNames[i] = thisEntry.getKey();
			paramValues[i] = thisEntry.getValue();
			i++;
		}
		return toCode(commandName, paramNames, paramValues);
	}

	public String[] getParamNames() {
		return getParamNames(commandName);
	}

	public String[] getDefaultValues() {
		return getDefaultValues(commandName);
	}

	public String[] getParamDescriptions() {
		return getParamDescriptions(commandName);
	}

	public String[] getParamOptions(String paramName) {
		return getParamOptions(commandName, paramName);
	}

	public ArrayList<String[]> getParamOptions() {
		ArrayList<String[]> paramOptions = new ArrayList<String[]>();
		String[] paramNames = getParamNames(commandName);
		for (String thisParamName : paramNames) {
			String[] theseOptions = getParamOptions(thisParamName);
			paramOptions.add(theseOptions);
		}
		return paramOptions;
	}

	public String getParameter(String paramName) {
		return (params.get(paramName));
	}

	public void setParameter(String paramName, String paramValue) {
		params.put(paramName, paramValue);
		setChanged();
		notifyObservers();
	}
	
	public String[] getFileParamNames() {
		return getFileParamNames(commandName);
	}

}
