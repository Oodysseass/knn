# KNN Algorithm with MPI
Implements kNN algorith both sequentially and with MPI for distributed systems.

<h2>Dependencies</h2>
<ul>
  <li>MPI library and mpic++ wrapper of compiler gcc. Can be installed by running:
  
    sudo apt-get install libopenmpi-dev
    
  </li>
  <li>gsl/gsl_cblas library: needed for matrix calculations. Can be installed by running:
  
    sudo apt-get install libgsl-dev
    
  </li>
</ul>

<h2>Running</h2>
<ul>
  <li>Clone repo and run make</li>
  <li>fileProducer is used to produce random data sets to test the programm. Usage:

    ./fileProducer <outputfilename> <numberOfPoints> <numberOfDimensions>

  e.g.
  
    ./fileProducer data.txt 1000 100

Produces a file with 1000 random points with 100 dimensions. The output file format (and the expected from the programm) is:

    <numberOfPoints> <numberOfDimensions>
    d1 d2 ... dn
    d1 d2 ... dn 
    .
    .
    .
    d1 d2 ... dn
   
  </li>
  <li>mainSeq is the sequential implementation of the algorithm and can be run like this:
  
    ./mainSeq <dataFile> <numberOfNeighbors>

  </li>
  <li>mainMPI is the distributed implementation of the algorithm and can be run like this:
  
    mpirun -np <numberOfProcesses> ./mainMPI <dataFile> <numberOfNeighbors>

  or
  
    mpirun --use-hwthread-cpus ./mainMPI <dataFile> <numberOfNeighbors>

  </li>
</ul>
