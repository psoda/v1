package gui;

public class ExecutePsodaCodeThread implements Runnable {

	private String code = "";
	private Thread thread;

	public ExecutePsodaCodeThread(String _code){
		code = _code;
	}

	public void start()
	{
		thread = new Thread(this);
		thread.start();
	}
	
	public void run(){
		if(!PSODA.getInstance().getIsRunning()){
			PSODA.getInstance().setIsRunning(true);
			PSODA.getInstance().writeStringToGui("> " + code + "\n");
			GuiCommandNode.runCommand(code + "\n");
			PSODA.getInstance().setIsRunning(false); // This will set 'running' to false when the thread ends //
		}
	}
	
}
