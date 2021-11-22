# MagneticChain

### The initial conditions are build as follows:
- create m cubes aligned along the y-axis in the xy-plane
- each cube consists of n^3 equidistantly placed dipoles
- each dipole has an magnetic momentum vector initially pointing in the y-direction
- the total magnetic chain consists of m x n^3 "elemental" dipoles

### The Metropolis Monte Carlo Algorithm is executed as follows:
- calculate the total energy for all individual dipole-dipole interactions of all cubes in the chain
- for each Monte Carlo step, either rotate or translate a randomly selected cube
- check that the updated cube position is not colliding with any other cubic volume
- calculate the updated total energy budget
- if the new chain configuration is more energetically favourable than the old one, keep it
- otherwise within a certain probability, discard the new chain configuration

### The data output is written as follows:
- at every stepping time a snapshot is written containing the position vectors and the energy value.
- the Python script is used in order to visualize the movement of the cubic chain, either for one snapshot or for all in form of a movie
- the input parameters for the Python script have to be: n, 'all', m, stepsize, total_runs

<img src="https://user-images.githubusercontent.com/49908052/142904977-32e01a40-3841-4e8f-90a9-77800fc34035.png" width="520"/>
<p float="left">
<img src="https://user-images.githubusercontent.com/49908052/142904689-10754cb5-8eb9-4bd6-b849-d916ab174d85.gif" width="420"/>
<img src="https://user-images.githubusercontent.com/49908052/142904879-37e0bbef-fcad-409b-9b94-bf6e3efb61b4.gif" width="420"/>
</p>
