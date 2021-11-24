# Metropolis Monte Carlo Simulation of a magnetic cubic chain
- How will the chain structure change over time, if randomly picked individual cubes are slightly altered according to the Metropolis algorithm?
- Thereby, the most computationally costly part of the process has been parallelized at each time step using fopenmp.

### The initial conditions are build as follows:
- create m cubes aligned along the y-axis in the xy-plane
- each cube consists of n^3 equidistantly placed dipoles
- each dipole has an magnetic momentum vector initially pointing in the y-direction
- the total magnetic chain consists of m x n^3 "elemental" dipoles

### The Metropolis Monte Carlo Algorithm is executed as follows:
- calculate the total magnetic energy for all individual dipole-dipole interactions of all cubes in the chain
- this process is parallelized using fopenmp
- for each Monte Carlo step, either rotate or translate a randomly selected cube
- check that the updated cube position is not colliding with any other cubic volume
- calculate the updated total energy budget
- if the new chain configuration is more energetically favourable than the old one, keep it
- otherwise within a certain probability, discard the new chain configuration

### The data output is written as follows:
- at regular time step intervals a snapshot is written containing the position vectors and the energy value.
- the Python script is used in order to visualize the movement of the cubic chain, either for one snapshot as a png or for all by creating a gif movie
- the input parameters for the Python script have to be in this order: 'all', n, m, stepsize, total_runs

<img src="https://user-images.githubusercontent.com/49908052/142904977-32e01a40-3841-4e8f-90a9-77800fc34035.png" width="520"/>
<p float="left">
<img src="https://user-images.githubusercontent.com/49908052/142904689-10754cb5-8eb9-4bd6-b849-d916ab174d85.gif" width="320"/>
<img src="https://user-images.githubusercontent.com/49908052/142904879-37e0bbef-fcad-409b-9b94-bf6e3efb61b4.gif" width="320"/>
</p>
