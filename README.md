# TSP - Held-Karp

A C implementation of the Held Karp algorithm to solve small TSP instances.

<h2><b>Algorithm:</b></h2>
<p>Held Karp is a dynamic programming algorithm used to effciently solve the Travelling Salesman Problem.
<br>By applying the divide-and-conquer principle, Held Karp calculates the path cost of subsets of increasing length.</p>
<p>For a more detailed explanation of the algorithm check the
<a href="https://en.wikipedia.org/wiki/Held%E2%80%93Karp_algorithm">Wikipedia Page</a>.
<brAlso check the <a href="https://github.com/AwardOfSky/Held-Karp">Java version</a>.</p>

<h2><b>Features:</b></h2>
<p> As of now, the program does not implement any kind of heuristics and will always return the optimal path.</p>
<p> As it's supposed to be a prove of concept, the matrix of distances is randomly generated before the computation for a given number of cities.
<p> This implementation makes use of multithreading to compute different iterations over city subsets of a given length.</p>

<h2><b>How to use:</b></h2>
<p>The project comes with a Linux object (compiled with GCC from the Makefile privided).
<p>Just download the object file, open up a terminal and type
<code>./heldkarp</code>.</p>
 
<h2><b>Benchmarks:</b></h2>
<p><b>All test were performed on a Intel Core i7 6700 @3.4Ghz - 16GB DDR4 RAM - Ubuntu 16.04 (64 bits).</b></p>
<p>23 cities: 2.482568 seconds<br>
  22 cities: 1.013295 seconds<br>
  21 cities: 0.464445 seconds<br>
  20 cities: 0.224266 seconds<br>
  19 cities: 0.105034 seconds<br>
  18 cities: 0.038902 seconds<br>
  17 cities: 0.016815 seconds<br>
  16 cities: 0.008512 seconds<br>
  15 cities: 0.004797 seconds<br>
  14 cities: 0.002704 seconds<br>
  13 cities: 0.001716 seconds<br>
  12 cities: 0.001120 seconds<br>
  11 cities: 0.000853 seconds<br>
  10 cities: 0.000689 seconds<br></p>

<h2><b>To be Implemented:</b></h2>
<p>1. Read input matrix from (.cvs)file.
<br>2. Improving thread management.
<br>3. Improving UI</p>
