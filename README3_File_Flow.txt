===============================================================
How It Works: The OMNeT++ Architecture
===============================================================
This project relies on four main file types working together. Think of building a simulation like putting on a stage play: you need a Script (C++), a Set Design (NED), Props (MSG), and a Director (INI).

1. The Blueprint (.ned)
	- Role: Set Design & Topology The NED files define what exists in the world, but not how it thinks.
	- In this project: tictoc15.ned defines the "actors" (modules named Txc15) and the "wires" connecting them (the mesh network topology).
	- Key Job: It tells the simulator: "Create 6 nodes and connect Node 0 to Node 1 with a 100ms delay line."

2. The Props (.msg)
	- Role: Custom Data Definitions Standard messages are empty envelopes. We use .msg files to define special envelopes with pre-printed forms on them.
	- In this project: tictoc15.msg defines a custom message type called TicTocMsg15.
	- Key Job: It adds fields like source, destination, and hopCount so the nodes can track where a packet came from and where it is going.
	- Behind the Scenes: The compiler reads this and automatically generates C++ code (tictoc15_m.cc and tictoc15_m.h) so the "Brain" can read these fields.

3. The Brain (.cc)
	- Role: Logic & Behavior This is the C++ code that gives the nodes intelligence. Without this, the NED modules are just empty shells.
	- In this project: txc15.cc contains the logic for the Txc15 class.
	- Key Job:
		- Initialization: "I am Node 0, I should start sending."
		- Routing: "This message is for Node 5, but I am Node 1. I will pick a random gate and forward it."
		- Statistics: "A message just arrived! I will record its hop count into the .vec file."

4. The Director (.ini)
	- Role: Configuration & Experiments The .ini file controls the simulation at runtime without changing the code.
	- In this project: omnetpp.ini decides which network to run (network = Tictoc15), sets the simulation speed, and turns on event logging.
	- Key Job: It allows you to run different experiments (e.g., changing the delay from 100ms to 200ms) just by editing text, without recompiling the C++ code.

===============================================================
The Workflow: From Code to Simulation
===============================================================
Here is the "Lifecycle" of your project when you press Build and Run:

Phase 1: The Build (Make)
	- OPP_MSGC: The Message Compiler looks at tictoc15.msg and generates the C++ helpers (_m.cc, _m.h).
	- GCC (C++ Compiler): It reads your txc15.cc (The Brain) and the generated message code. It translates them into machine code (.o files).
	- Linker: It combines all these pieces into a single application file: tictocTest.exe.
	- Note: The Makefile is the invisible recipe that coordinates this entire kitchen process.

Phase 2: The Run (Simulation)
	- Launch: You press "Run." The computer starts tictocTest.exe.
	- Loading: The executable reads omnetpp.ini to know what to do.
	- Setup: It reads tictoc15.ned to build the network in memory.
	- Execution:
		- Node 0 creates a message (.msg).
		- The message travels through the .ned wires.
		- Nodes run their .cc logic to forward it.
		- Stats are written to results/General-#0.vec.

Phase 3: The Analysis
	- Finish: The simulation ends.
	- Plotting: You open the .vec file in the IDE. The Analysis Tool reads the recorded data and draws the Hop Count graph.