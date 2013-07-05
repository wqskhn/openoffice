/**************************************************************
 * 
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 * 
 *   http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 * 
 *************************************************************/

package installer;

/*
 * Welcome.java
 *
 * Created on 04 July 2002, 15:43
 */

/**
 *
 * @author  mike
 */
import java.awt.event.*;
import javax.swing.*;
import java.io.*;
import java.net.*;
import java.util.Properties;

public class Welcome extends javax.swing.JPanel implements ActionListener {

    /** Creates new form Welcome */
    public Welcome(InstallWizard wizard) {
	this.wizard = wizard;
	setBorder(new javax.swing.border.EtchedBorder(javax.swing.border.EtchedBorder.RAISED));
        initComponents();
    }

    /** This method is called from within the constructor to
     * initialize the form.
     * WARNING: Do NOT modify this code. The content of this method is
     * always regenerated by the Form Editor.
     */
    private void initComponents() {//GEN-BEGIN:initComponents
        welcomePanel = new javax.swing.JPanel();
        area = new javax.swing.JTextArea();
	nextButtonEnable = true;
        
        setLayout(new java.awt.BorderLayout());
        
        welcomePanel.setLayout(new java.awt.BorderLayout());
        area.setEditable(false);
        area.setLineWrap(true);
	
        String message = "\n\tOffice Scripting Framework Version 0.3" +
            "\n\n\n\tPlease ensure that you have exited from Office";

	/* String userDir = (String) System.getProperty( "user.dir" );
	boolean isValid = validateCurrentUserDir(userDir);
	if( !isValid ) {
		nextButtonEnable = false;
		message = "Please run Installer from the program directory in a valid Office installation";
		setUpWelcomePanel(message);
		return;
	}
	
	int programPosition = userDir.lastIndexOf("program");
	String offInstallPth = null;
	offInstallPth = userDir.substring( 0, programPosition );
	
        wizard.storeLocation(offInstallPth); */
	setUpWelcomePanel(message);
        
    }//GEN-END:initComponents

    private void setUpWelcomePanel(String message){
	area.setText( message );
        welcomePanel.add(area, java.awt.BorderLayout.CENTER);
        add(welcomePanel, java.awt.BorderLayout.CENTER);
	NavPanel nav = new NavPanel(wizard, false, nextButtonEnable, true, "", InstallWizard.VERSIONS);
	nav.setNextListener(this);
	add(nav, java.awt.BorderLayout.SOUTH);
		
	//Banner br = new Banner();
	//add(br, java.awt.BorderLayout.WEST);
    }
    
    
    private boolean validateCurrentUserDir(String userDir){

	
	
	Properties props = null;
	
        File fileVersions = null;
	try
	{
            fileVersions = InstUtil.buildSversionLocation();
	}
	catch(IOException eFnF)
	{
            System.err.println("Cannot find sversion.ini/.sversionrc");
            JOptionPane.showMessageDialog(this, eFnF.getMessage(), "File not Found", JOptionPane.ERROR_MESSAGE);
            wizard.exitForm(null);
	}
        
        try {
            props = InstUtil.getOfficeVersions(fileVersions);
        }
        catch (IOException eIO) {
            //Message about no installed versions found
            System.err.println("Failed to parse SVERSION");
			JOptionPane.showMessageDialog(this, "There was a problem reading from the Office settings file.", "Parse Error", JOptionPane.ERROR_MESSAGE);			
			wizard.exitForm(null);
        }


	boolean versionMatch = false;
	
	for( int i = 0; i < versions.length; i++ ) {
		String key = versions[i];
		String progPath = ( String )props.getProperty( key );
		if ( progPath != null ){	
			progPath = progPath  + File.separator + "program";
			
			File tmpFile = new File(progPath + File.separator + "oostubversion.txt");
			try{
			tmpFile.createNewFile();
			
				if( new File(userDir + File.separator + "oostubversion.txt").exists())
				{
					versionMatch = true;
					break;	
				}
			}
			catch( IOException e)
			{
				// Fail silently
			}
			tmpFile.delete();			
		}
	}	
	return versionMatch;
    }
    
    
    
    public java.awt.Dimension getPreferredSize() {
        return new java.awt.Dimension(InstallWizard.DEFWIDTH, InstallWizard.DEFHEIGHT);
    }    

	public void actionPerformed(ActionEvent ev)
	{
		//Perform next actions here...
	}


    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JPanel welcomePanel;
    private javax.swing.JTextArea area;
    private InstallWizard wizard;
    //private static final String [] versions = {"OpenOffice.org 643", "StarOffice 6.1"};	
    private static final String [] versions = { "StarOffice 6.1" };	
    private boolean nextButtonEnable = true;

    // End of variables declaration//GEN-END:variables
}