*** AgentLCM ***
*** Wenwu Tang ***
*** latest update: 01/07/2020 ***

# AgentLCM: Agent-based Land Change Modeling
AgentLCM is an agent-based model for the simulation of land use and land cover change. AgentLCM is based on the modification of GAIASP: GAIA Simulation Package (GAIA: Geographically Aware Intelligent Agents; Tang 2008)

# Publication: 

Tang, W, and Yang, J., 2020(accepted), Agent-based land change modeling of a large watershed: Space-time locations of critical threshold, Journal of Artificial Societies and Social Simulation.

* For latest version and update: https://github.com/wenwu-tang/agentlcm

* Model repository structure
	./data/ : for data and parameters
	./model: for specific model implementation (e.g., land owners, developers...)
	./results: simulation results
	./simulation: main program for simulation modeling
	./src: code for generic agents, environments,...

* To compile: 
	go to ./simulation: then: make simulation

* To run:
	Go to ./simulation; then: make run

* Reference

Tang, W. (2008). Simulating complex adaptive geographic systems: A geographically aware intelligent agent approach. Cartography and Geographic Information Science, 35(4), 239-263
