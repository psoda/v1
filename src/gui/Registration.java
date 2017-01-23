package gui;

import java.awt.Dimension;

import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JTextField;
import javax.swing.JLabel;
import javax.swing.SpringLayout;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.FileReader;
import java.io.FileWriter;

import java.net.URL;
import java.net.URLConnection;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

public class Registration{
	
	public Registration(String homeDir){
		String firstName = "";
		String lastName = "";
		String institution = "";
		String email = "";
		int lineNum = 1;
		try{
			BufferedReader in = new BufferedReader(new FileReader(homeDir + "/.psodaRegistration"));
			String tmp = "";
			while((tmp = in.readLine()) != null){
				if(lineNum == 1){
					firstName = tmp;
				}
				else if(lineNum == 2){
					lastName = tmp;
				}
				else if(lineNum == 3){
					institution = tmp;
				}
				else if(lineNum == 4){
					email = tmp;
				}
				lineNum++;
			}
		}catch(FileNotFoundException e){}
		 catch(IOException e){
			e.printStackTrace();
		}
		registerPSODA(firstName, lastName, institution, email, homeDir);
	}

	/**
	 * This method will post the registration information to the download page
	 * 
	 * Parameters: Strings representing the various fields and the path to the
	 *				user's home directory
	 * Return Value: NONE
	 */
	JFrame registrationFrame = new JFrame("Psoda Registration");
	private void registerPSODA(String firstName, String lastName, String institution, String email, final String homeDir){
		registrationFrame.setPreferredSize(new Dimension(350, 250));
		registrationFrame.setResizable(false);
		SpringLayout layout = new SpringLayout();
		JPanel registrationPanel = new JPanel(layout);

		JLabel blurbLabel = new JLabel("Please take a moment to register PSODA.");
		JLabel blurbLabel2 = new JLabel("The information you provide is used to");
		JLabel blurbLabel3 = new JLabel("inform you of PSODA updates.");
		JLabel firstNameLabel = new JLabel("First Name: ");
		JLabel lastNameLabel = new JLabel("Last Name: ");
		JLabel institutionLabel = new JLabel("Institution: ");
		JLabel emailLabel = new JLabel("Email: ");

		final JTextField firstNameField = new JTextField(firstName, 15);
		final JTextField lastNameField = new JTextField(lastName, 15);
		final JTextField institutionField = new JTextField(institution, 15);
		final JTextField emailField = new JTextField(email, 15);

		JButton registerButton = new JButton("Register");
		registerButton.setSelected(true);

		registerButton.addActionListener(new ActionListener(){
			public void actionPerformed(ActionEvent e){
				if(firstNameField.getText().equals("") ||
					lastNameField.getText().equals("") ||
					institutionField.getText().equals("") ||
					emailField.getText().equals(""))
				{
					JOptionPane.showMessageDialog(null, "Please fill in all fields.", "WARNING", JOptionPane.WARNING_MESSAGE);
				}
				else{
					try{
						String firstName = makeStringURLCompatible(firstNameField.getText());
						String lastName = makeStringURLCompatible(lastNameField.getText());
						String institution = makeStringURLCompatible(institutionField.getText());
						String email = makeStringURLCompatible(emailField.getText());

						URL url = new URL("http://csl.cs.byu.edu/psoda/downloadPsoda/download.cgi?FirstName=" + firstName +
										"&LastName=" + lastName + "&Institution=" + institution +
										"&email=" + email);
						URLConnection connection = url.openConnection();
						//System.out.println("URL: " + connection.getURL());
						String field = connection.getHeaderField(1);
						if(field == null){
							JOptionPane.showMessageDialog(null, "There was an error during registration.\nYour internet" +
																" connection may be down. Please try again later.", "ERROR", JOptionPane.ERROR_MESSAGE);
						}
						else{
							JOptionPane.showMessageDialog(null, "Registration successful. Thank you!", "Thank You", JOptionPane.INFORMATION_MESSAGE);
							writeRegistrationFile(homeDir);
						}
					}catch(IOException exp){
						JOptionPane.showMessageDialog(null, "There was an error during registration.\nYour internet" +
															" connection may be down. Please try again later.", "ERROR", JOptionPane.ERROR_MESSAGE);
					}
				}
				setRegistrationFrameVisible(false);
				writeRegistrationInformationFile(firstNameField.getText(), lastNameField.getText(), institutionField.getText(), emailField.getText(), homeDir);
			}
		});

		registrationPanel.add(blurbLabel);
		registrationPanel.add(blurbLabel2);
		registrationPanel.add(blurbLabel3);
		registrationPanel.add(firstNameLabel);
		registrationPanel.add(lastNameLabel);
		registrationPanel.add(institutionLabel);
		registrationPanel.add(emailLabel);
		registrationPanel.add(firstNameField);
		registrationPanel.add(lastNameField);
		registrationPanel.add(institutionField);
		registrationPanel.add(emailField);
		registrationPanel.add(registerButton);

		layout.putConstraint(SpringLayout.WEST, blurbLabel, 10, SpringLayout.WEST, registrationPanel);
		layout.putConstraint(SpringLayout.NORTH, blurbLabel, 5, SpringLayout.NORTH, registrationPanel);
		layout.putConstraint(SpringLayout.WEST, blurbLabel2, 10, SpringLayout.WEST, registrationPanel);
		layout.putConstraint(SpringLayout.NORTH, blurbLabel2, 2, SpringLayout.SOUTH, blurbLabel);
		layout.putConstraint(SpringLayout.WEST, blurbLabel3, 10, SpringLayout.WEST, registrationPanel);
		layout.putConstraint(SpringLayout.NORTH, blurbLabel3, 2, SpringLayout.SOUTH, blurbLabel2);

		layout.putConstraint(SpringLayout.WEST, firstNameLabel, 10, SpringLayout.WEST, registrationPanel);
		layout.putConstraint(SpringLayout.NORTH, firstNameLabel, 15, SpringLayout.SOUTH, blurbLabel3);
		layout.putConstraint(SpringLayout.WEST, firstNameField, 5, SpringLayout.EAST, firstNameLabel);
		layout.putConstraint(SpringLayout.NORTH, firstNameField, 15, SpringLayout.SOUTH, blurbLabel3);

		layout.putConstraint(SpringLayout.WEST, lastNameLabel, 10, SpringLayout.WEST, registrationPanel);
		layout.putConstraint(SpringLayout.NORTH, lastNameLabel, 15, SpringLayout.SOUTH, firstNameLabel);
		layout.putConstraint(SpringLayout.WEST, lastNameField, 5, SpringLayout.EAST, firstNameLabel);
		layout.putConstraint(SpringLayout.NORTH, lastNameField, 15, SpringLayout.SOUTH, firstNameLabel);

		layout.putConstraint(SpringLayout.WEST, institutionLabel, 10, SpringLayout.WEST, registrationPanel);
		layout.putConstraint(SpringLayout.NORTH, institutionLabel, 15, SpringLayout.SOUTH, lastNameLabel);
		layout.putConstraint(SpringLayout.WEST, institutionField, 5, SpringLayout.EAST, firstNameLabel);
		layout.putConstraint(SpringLayout.NORTH, institutionField, 15, SpringLayout.SOUTH, lastNameLabel);

		layout.putConstraint(SpringLayout.WEST, emailLabel, 10, SpringLayout.WEST, registrationPanel);
		layout.putConstraint(SpringLayout.NORTH, emailLabel, 15, SpringLayout.SOUTH, institutionLabel);
		layout.putConstraint(SpringLayout.WEST, emailField, 5, SpringLayout.EAST, firstNameLabel);
		layout.putConstraint(SpringLayout.NORTH, emailField, 15, SpringLayout.SOUTH, institutionLabel);

		layout.putConstraint(SpringLayout.EAST, registerButton, -5, SpringLayout.EAST, registrationPanel);
		layout.putConstraint(SpringLayout.SOUTH, registerButton, -5, SpringLayout.SOUTH, registrationPanel);

		registrationFrame.add(registrationPanel);
		registrationFrame.pack();
		registrationFrame.setVisible(true);
	}

	/**
	 * This method will take a string and make it URL compatible. Specifically,
	 * it will replace spaces with '%20' which represents the space for URLs
	 * 
	 * Parameters: A String to be converted
	 * Return Value: A String with spaces replaced by '%20'
	 */
	private String makeStringURLCompatible(String string){
		String newString = new String();
		for(int i = 0; i < string.length(); i++){
			if(string.charAt(i) == ' '){
				newString += "%20";
			}
			else{
				newString += string.substring(i, i+1);
			}
		}
		return newString;
	}

	/**
	 * This method will set the registrationFrame's visible status
	 * 
	 * Parameters: A boolean determining the status
	 * Return Value: NONE
	 */
	private void setRegistrationFrameVisible(boolean bool){
		registrationFrame.setVisible(bool);
	}

	/**
	 * This method will write a file to the user's home directory
	 * with the information they provided. The main use of this file
	 * is to repopulate the registration fields if registration was
	 * not successful
	 * 
	 * Parameters: Several Strings representing the information
	 * provided and a string representing the user's home directory
	 * Return Value: NONE
	 */
	private void writeRegistrationInformationFile(String firstName, String lastName, String institution, String email, String homeDir){
		try{
			BufferedWriter out = new BufferedWriter(new FileWriter(homeDir + "/.psodaRegistration"));
			out.write(firstName + "\n");
			out.write(lastName + "\n");
			out.write(institution + "\n");
			out.write(email);
			out.close();
		}catch(IOException e){
			e.printStackTrace();
		}
	}

	/**
	 * This method will write a file to the user's home directory
	 * with a registration number. The main use of this file
	 * is to know if the person has already registered. If the file
	 * is present the registrationFrame will not be shown again.
	 * 
	 * Parameters: A String representing the user's home directory
	 * Return Value: NONE
	 */
	private void writeRegistrationFile(String homeDir){
		try {
			BufferedWriter out = new BufferedWriter(new FileWriter(homeDir + "/.psodaRegistered"));
			out.write("Thank you for registering PSODA!\npq9u2-9da01-al27kj-l320zl");
			out.close();
		}catch(IOException e){
			e.printStackTrace();
		}
	}
}
