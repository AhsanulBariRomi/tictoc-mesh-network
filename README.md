# TicToc Mesh Network Simulation 

A discrete event simulation project built with **OMNeT++** that models a dynamic mesh network using custom routing protocols, stochastic packet loss, and performance statistical analysis.

## 🚀 About OMNeT++
OMNeT++ (Objective Modular Network Testbed in C++) is an extensible, modular, component-based C++ simulation library and framework, primarily for building network simulators. It is widely used in academia and industry to model communication networks, multiprocessors, and other distributed systems.

## 📋 Project Summary
This project demonstrates the progression from a simple two-node communication model to a complex, multi-node mesh network.

**Key Features Implemented:**
* **Mesh Topology:** A 6-node network connected via bidirectional gates with defined delay channels.
* **Custom Protocols:** Implemented "Stop-and-Wait" logic and random routing algorithms for packet forwarding.
* **Smart Messaging:** Designed custom message types (`.msg`) carrying source, destination, and hop-count data.
* **Stochastic Behavior:** Modeled realistic network conditions including random processing delays and packet loss probabilities.
* **Data Analysis:** Integrated statistical data collection (Vectors & Histograms) to analyze network performance (e.g., Hop Count distribution) using the OMNeT++ Analysis Tool.

## 📚 Documentation & How to Run
To fully understand the project structure and how to replicate the simulation, please follow the documentation in this order:

### 1. [Installation Guide](README1_Installation_Guide.md)
* *How to download and set up the OMNeT++ environment and import this project.*

### 2. [Work Steps](README2_Work_Steps.md)
* *A step-by-step breakdown of how the code was built, covering the logic from Part 1 (Basic) to Part 5 (Statistics).*

### 3. [File Flow & Architecture](README3_File_Flow.md)
* *Explanation of the project architecture, detailing how `.ned`, `.msg`, `.ini`, and `.cc` files interact to create the simulation.*

---
*Created as part of an OMNeT++ learning module.*
