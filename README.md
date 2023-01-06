# KNN Algorithm with MPI
<p>Implements kNN algorith both sequentially and with MPI for distributed systems.</p>

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
<p>Clone repo and run make</p>

<p>fileProducer is used to produce random data sets to test the programm. Usage:</p>

  ./fileProducer <outputfilename> <numberOfPoints> <numberOfDimensions>

<p>
e.g.
  
  ./fileProducer data.txt 1000 100

</p>
<p>
Produces a file with 1000 random points with 100 dimensions. The output file format (and the expected from the programm) is:

   <numberOfPoints> <numberOfDimensions>
   d1 d2 ... dn
   d1 d2 ... dn 
   .
   .
   .
   d1 d2 ... dn
</p>
  
<p>mainSeq is the sequential implementation of the algorithm and can be run like this:</p>
  
  ./mainSeq <dataFile> <numberOfNeighbors>

<p>mainMPI is the distributed implementation of the algorithm and can be run like this:</p>
  
  mpirun -np <numberOfProcesses> ./mainMPI <dataFile> <numberOfNeighbors>

or
  
  mpirun --use-hwthread-cpus ./mainMPI <dataFile> <numberOfNeighbors>
