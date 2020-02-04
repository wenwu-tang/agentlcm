# Agent-LCM: Agent-based Land Change Modeling
Agent-LCM is an agent-based model (ABM) for the simulation of land use and land cover change. Agent-LCM is modified from GAIASP: GAIA Simulation Package (GAIA: Geographically Aware Intelligent Agents; Tang 2008). Agent-LCM is written in C++ programming language. 

Please see HERE (http://jasss.soc.surrey.ac.uk/23/1/15.html#toc-appendix-the-overview-design-concepts-details-odd-protocol-) for an ODD (Overview, Design concept, Details; see Grimm et al. 2010) ABM protocol of Agent-LCM developed for the land change simulation of a large watershed (lower High Rock Lake Watershed) in North Carolina, USA. The study region covers 8 counties in the Piedmont region of North Carolina. 

## Instruction

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

## Associated Publication: 

Tang, W, and Yang, J., 2020, Agent-based land change modeling of a large watershed: Space-time locations of critical threshold, Journal of Artificial Societies and Social Simulation. 23(1): 15. Online Available at: http://jasss.soc.surrey.ac.uk/23/1/15.html

## Reference

Grimm, V., Berger, U., DeAngelis, D. L., Polhill, J. G., Giske, J., & Railsback, S. F. (2010). The ODD protocol: a review and first update. Ecological modelling, 221(23), 2760-2768.

Tang, W. (2008). Simulating complex adaptive geographic systems: A geographically aware intelligent agent approach. Cartography and Geographic Information Science, 35(4), 239-263
