1. Download from:
	https://omnetpp.org/download/
	
2. Unzip the folder

3. Open the ==> configure.user file and change PREFER_CLANG to no from yes, otherwise it will create problem in inheritence operations in windows
	PREFER_CLANG=no
	
4. open the commandprompt ==> mingwenv.cmd 
	It will automatically unpack the main GW tool chain

5. run the command ==> ./configure
	it will build the libraries
	
6. next run the command ==> make
	it will make the omnet++ environment ready. Now we can launch the omnet++
	
7. Before doing that lets verify our installation:
	go to ==> cd samples/aloha
	run ==> ./aloha
		it will launch the qt environment. Now we can run and check if everything is working there. (check SS = step 2.png)
		
8. To launch the omnet++
	Go back to the root ==> cd ../..
	Run ==> omnetpp
	
	Also we can create a shortcut at desktop of the .exe file of the ide from ==> F:\Omnet\omnetpp-6.2.0\ide
	
	Setting the workspace to ==> F:\Omnet\omnetpp-6.2.0\workspace
	
========================================	
How to Download and Run This Project
========================================
Since this repository contains the source code, you will need to build it using the OMNeT++ IDE.
Step 1: Download the Code
1.	Click the Code button (green) on this GitHub page.
2.	Select Download ZIP.
3.	Extract the ZIP file to your computer.

Step 2: Create the Project in OMNeT++
1.	Open the OMNeT++ IDE.
2.	Go to File 1$\rightarrow$ New 2$\rightarrow$ OMNeT++ Project.3
3.	Name it tictoc-mesh (or whatever you prefer) and click Next.
4.	Select "Empty Project" and click Finish.

Step 3: Import the Files
1.	Go to the folder where you extracted the ZIP.
2.	Copy the src and simulations folders.
3.	Go back to the OMNeT++ IDE.
4.	Right-click on your new empty project in the Project Explorer and select Paste.
o	Select "Yes to All" if asked to overwrite.

Step 4: Build (The Magic Step)
1.	Right-click on the project folder and select Build Project (or press Ctrl + B).
2.	Wait! The IDE is now looking at your ingredients and automatically creating the Makefile and compiling the executable for your specific machine.

Step 5: Run
1.	Navigate to simulations/omnetpp.ini.
2.	Click the green Run button.
