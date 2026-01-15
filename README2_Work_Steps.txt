============================================================================
Link: https://docs.omnetpp.org/tutorials/tictoc/part1/
============================================================================

TicToc Tutorial - Modern OMNeT++ Adjustments
============================================================================
1. Project Setup & Package Cleanup

	- Context: The tutorial assumes a flat, single-directory project. Modern OMNeT++ creates a structured project with src (for code) and simulations (for runs) folders, plus package.ned files.

	- Action: Created the tictoc project.

	- Adjustment: Deleted the package.ned files from both the src/ and simulations/ folders.

	- Reason: This allows the project to run in the "default package" (no package declaration required), preventing errors when using the simple tutorial code.

2. Network Definition (.ned)

	- Context: Defines the module structure (Tic and Toc nodes) and connections.

	- Action: Created tictoc1.ned.

	- Location: Placed inside the src/ folder.

	- Note: The tutorial says "project directory," but in modern projects, source files belong in src.

3. C++ Logic (.cc)

	- Context: Defines the behavior (sending/receiving messages) using the Txc1 class.

	- Action: Created txc1.cc.

	- Location: Placed inside the src/ folder (alongside the .ned file).

	- Key Code: Implemented initialize() to start the message chain and handleMessage() to bounce it back.

4. Configuration (.ini)

	- Context: Sets up the simulation run (telling it which network to load).

	- Action: Configured omnetpp.ini.

	- Location: Used the existing file inside the simulations/ folder (instead of creating a new one).

	- Content: Set network = Tictoc1.
	
============================================================================
Running, Debugging & Visualization
============================================================================
1. Running the Simulation

	- Action: Launched the simulation by selecting omnetpp.ini in the simulations/ folder and clicking Run.

	- Outcome: Verified the network behavior in the Qtenv graphical interface. Observed the tic and toc nodes exchanging the tictocMsg packet with a 100ms delay.

2. Debugging & Message Ownership

	- Experiment: Intentionally introduced a bug in txc1.cc by attempting to send() the same message pointer twice.

	- Result: Triggered a runtime error ("Message is currently in scheduled-events").

	- Concept Learned: OMNeT++ enforces strict ownership rules. Once a message is sent, ownership transfers to the simulation kernel. It cannot be accessed or sent again by the module unless it is received back.

3. Sequence Chart Visualization

	- Action: Enabled event logging by adding record-eventlog = true to omnetpp.ini.

	- Result: Generated a .elog file in the results/ directory after terminating the simulation.

	- Analysis: Opened the file in the Sequence Chart tool to visualize the communication timeline. Confirmed the signal propagation delay (represented by the diagonal slope of message arrows) between the two nodes.
	
============================================================================
Enhancing the 2-node TicToc
============================================================================
1. Visualizing the Network (.ned)
	- Action: Created tictoc2.ned and used the @display string to customize the appearance of the modules.
	- Code: Assigned i=block/routing as the default icon, and used i=,cyan and i=,gold to colorize the tic and toc nodes respectively.
	- Outcome: The simulation window now displays distinct, colorful icons for the nodes.
	
2. Implementing State Variables & Counters (.cc)
	- Action: Updated the C++ code to include a counter variable in the class definition.
	- Logic:
		- Initialized counter = 10 in initialize().
		- Decremented the counter in handleMessage() upon message arrival.
		- Added logic to delete the message when the counter reaches zero, effectively stopping the infinite simulation loop.
	- Key Feature: Used the WATCH(counter) macro to make the variable visible in the runtime Inspector panel.
	
3. Adding Parameters (.ned & .ini)
	- Action: Replaced hardcoded values with flexible NED parameters to make the C++ code generic.
	- Parameters:
		- bool sendMsgOnInit: Determines if a module should start the transmission (True for Tic, False for Toc).
		- int limit: Defines how many messages to exchange before stopping.
		- Configuration: Used omnetpp.ini to set values, demonstrating the use of wildcards (e.g., Tictoc4.t*c.limit = 5) to configure multiple nodes simultaneously.
		
4. Using NED Inheritance
	- Action: Refactored the NED files to use object-oriented inheritance.
	- Structure:
		- Created a base module simple Txc5 with common parameters and gates.
		- Created specialized modules simple Tic5 extends Txc5 and simple Toc5 extends Txc5 that override parameters (like sendMsgOnInit).
		- Benefit: Reduced code duplication and simplified the network definition.

5. Modeling Processing Delay (Self-Messages)
	- Action: Introduced timing by making nodes "hold" messages before replying.
	- Mechanism: Used Self-Messages (scheduleAt()) to simulate processing time.
		- Incoming messages are stored.
		- A self-message is scheduled for $t + delay$.
		- Upon receiving the self-message (timer expiry), the stored message is sent out.

6. Random Numbers & Packet Loss
	- Action: Added stochastic behavior to the simulation.
	- Random Delay: Used volatile parameters in NED and distributions like exponential(3s) and truncnormal(3s,1s) in .ini to vary delay times dynamically.
	- Packet Loss: Implemented a probability check (uniform(0, 1) < 0.1) to simulate a 10% chance of deleting (dropping) a packet.

7. Implementing "Stop-and-Wait" Protocol
	- Action: Built a full reliability protocol to handle packet loss.
	- Logic:
		- Sender (Tic): Sends a copy (dup()) of the message and schedules a timeout timer. If the timer expires (message lost), it retransmits the copy.
		- Receiver (Toc): Sends the message back as an acknowledgement (ACK).
		- Recovery: When the Sender receives the ACK, it cancels the timeout timer (cancelEvent) and prepares the next message.
	- Outcome: The simulation successfully recovers from packet drops by automatically retransmitting, ensuring reliable communication.


============================================================================
Building a Real Mesh Network
============================================================================
1. Moving to Multiple Nodes (in[], out[])

	- Action: Transitioned from a simple 2-node topology to a 6-node mesh network.

	- NED Logic: Introduced module vectors (tic[6]) to create multiple nodes instantly. Used gate vectors (input in[], output out[]) to allow nodes to have a variable number of connections depending on the network layout.

	- C++ Logic: Implemented a basic random routing algorithm using gateSize() and intuniform() to pick a random output gate for forwarding messages.

2. Using Two-Way Connections (inout gates)

	- Action: Simplified the NED network definition by replacing pairs of one-way connections with bidirectional ones.

	- Code: Defined inout gate[] in the NED file.

	- Implementation: Updated the C++ send() command to target the specific output half of the bidirectional gate using the suffix "gate$o".

3. Defining Custom Messages (.msg files)

	- Action: Created a structured message type to replace the generic cMessage.

	- Definition: Created tictoc13.msg to define TicTocMsg13, adding custom fields for source, destination, and hopCount.

	- Integration:

		- Configured OMNeT++ to auto-generate the C++ class (_m.h and _m.cc) from the message definition.

		- Used check_and_cast<TicTocMsg13 *>() to safely cast generic pointers to the custom type.

		- Implemented logic where a node checks if it is the destination (getDestination() == getIndex()). If so, it consumes the message and generates a new one; otherwise, it acts as a router and forwards it.
		
		- It works like a relay race where the "baton" changes hands.
			- Start: Node 0 creates the very first message (let's say tic-0-to-5).
			- Travel: That message bounces around until it hits Node 5.
			- Arrival: Node 5 checks the address: "Hey, this is for me!"
			The Handoff:
			- Node 5 deletes the old message (tic-0-to-5).
			- Node 5 creates a NEW message (e.g., tic-5-to-2).
			- Repeat: Now the new message travels to Node 2. When it arrives, Node 2 will create the next message.
			So, a new message is created at Node 5 only if the previous message was addressed to Node 5.


============================================================================
Statistics & Analysis
============================================================================
1. Live Visualization (Counters)

	- Action: Added real-time visual feedback to the network simulation.

	- Code:
		- Added numSent and numReceived counters to the Txc14 class.
		- Implemented refreshDisplay() to update the module's appearance dynamically.
		- Used getDisplayString().setTagArg("t", ...) to display the text "sent: X rcvd: Y" directly above the node icons in the GUI.

	- Outcome: Users can monitor traffic load on individual nodes instantly during the simulation run.

2. Statistics Collection (Analysis)

	- Action: Implemented professional data collection to analyze network performance (Hop Counts).

	- Mechanism:
		- cOutVector: Used to record the hop count of every individual packet over time into a vector file (.vec).
		- cHistogram: Used to calculate aggregate statistics (Mean, Max, Standard Deviation) for the hop counts.

	- Workflow:
		- Configured the simulation to record event logs.
		- Used the finish() function to save scalar results (.sca) when the simulation concludes.
		- Utilized the OMNeT++ Analysis Tool (.anf files) to load the raw results, filter the data, and generate plots/graphs showing the distribution of message hops across the mesh network
































