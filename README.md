# AgentLCM: Agent-based Land Change Modeling
AgentLCM is an agent-based model for the simulation of land use and land cover change. AgentLCM is modified from GAIASP: GAIA Simulation Package (GAIA: Geographically Aware Intelligent Agents; Tang 2008). AgentLCM is written in C++ programming language. 

Please see HERE for an ODD (Overview, Design concept, Details; see Grimm et al. 2010) protocol of AgentLCM developed for the land change simulation of a large watershed (lower High Rock Lake Watershed) in North Carolina, USA. The study region covers 8 counties in North Carolina. 

## Associated Publication: 

Tang, W, and Yang, J., 2020 (accepted), Agent-based land change modeling of a large watershed: Space-time locations of critical threshold, Journal of Artificial Societies and Social Simulation.

## Instruction
**Latest update: 01/07/2020**

* For latest version and update: https://github.com/wenwu-tang/agentlcm

* Model repository structure
	* ./data/ : for data and parameters
	* ./model: for specific model implementation (e.g., land owners, developers...)
	* ./results: simulation results
	* ./simulation: main program for simulation modeling
	* ./src: code for generic agents, environments,...

* To compile: 
	* go to ./simulation: then: make simulation

* To run:
	* go to ./simulation; then: make run

## Reference

Grimm, V., Berger, U., DeAngelis, D. L., Polhill, J. G., Giske, J., & Railsback, S. F. (2010). The ODD protocol: a review and first update. Ecological modelling, 221(23), 2760-2768.

Tang, W. (2008). Simulating complex adaptive geographic systems: A geographically aware intelligent agent approach. Cartography and Geographic Information Science, 35(4), 239-263
