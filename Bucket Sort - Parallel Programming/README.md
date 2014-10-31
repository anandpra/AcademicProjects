
Problem Statement:
          
          Simple Parallel Bucket Sorting using OpenMP and MPI: study of scalability.

Description: 

             In this assignment you will optimize parallel programs using OpenMP in a sharedmemory
          environment and using MPI in a distributed-memory environment. In particular, you will
          have to parallelize a simple bucket sorting algorithm.
          Start with a uniprocessor (sequential) program written in C for bucket sorting. Assume a range
          within a bucket. Since the numbers are not uniformly distributed, your buckets may be
          imbalanced when you distribute the buckets to different processors. Make sure you balance
          load on each processor to get good performance.
          • Your task is to parallelize the algorithm using the OpenMP API on the shared-memory
          nodes of CCR and MPI on the CCR cluster
          • The program takes two main parameters, which are read in from the command line:
          P: the number of processors and N: the problem size.
          • Use the provided programs to generate random numbers with a normal distribution.
          • In addition, the program takes arguments that will tell whether to output the original and
          sorted list, either to a file or standard out. This should assist you in ensuring that the
          parallel version of the algorithm works correctly.

Project Report:

        Please refer to "Parallel Bucket Sort - Project Report" present above.
