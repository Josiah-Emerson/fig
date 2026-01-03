I believe the solver works correctly. See the MSD (Mean Squared Distance) check below. Might be good idea to look into other
ways to check the validity of the solver. Now just need to create some plots of the particle, and check dimensions past 1. Also,
remove the added bits from the solver which were used for investigating things. Also, look into refactoring solver since we no longer subtract to find the distance and allat.


The expected MSD is: 4.4e-05
The average MSD after 10000 iterations for 1 time steps is: 4.36658e-05
The average MSD after 10000 iterations for 5 time steps is: 4.31166e-05
The average MSD after 10000 iterations for 10 time steps is: 4.3788e-05
The average MSD after 10000 iterations for 50 time steps is: 4.40551e-05
The average MSD after 10000 iterations for 100 time steps is: 4.45078e-05
The average MSD after 10000 iterations for 500 time steps is: 4.43106e-05
The average MSD after 10000 iterations for 1000 time steps is: 4.373e-05
The average MSD after 10000 iterations for 5000 time steps is: 4.3849e-05
The average MSD after 10000 iterations for 10000 time steps is: 4.39099e-05
The average MSD after 10000 iterations for 50000 time steps is: 4.46298e-05
