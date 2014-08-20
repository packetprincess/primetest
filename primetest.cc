/* This program generates a list of the first N primes. 
 * 
 * Jamie Schwettmann
 * @complexbits
 * github.com/complexbits
 *
 */

#include <iostream>    // for printf
#include <stdio.h>
#include <stdlib.h>
#include <math.h>      // for log op
#include <unistd.h>    // for parsing argumnents
#include <vector>      // for vectors
#include <utility>     // for vector pairs
#include <string>      // for strings
using namespace std;

#define mpf(x,...) ((mtr == 1 || dbg == 1 || ver == 1)?printf(x,__VA_ARGS__):0)
#define vpf(x,...) ((ver == 1 || dbg == 1)?printf(x,__VA_ARGS__):0)
#define dpf(x,...) ((dbg == 1)?printf(x,__VA_ARGS__):0)

void usage_msg(char* thisprog){
  printf ("Usage: %s <opts>\n\nOptions:\n",thisprog);
  printf ("\t-n <num> Number of primes to seek\n");
  printf ("\t-p <num> Interval for printing primes. Defaults to auto.\n");
  printf ("\t-m Print out metrics at the end.\n");
  printf ("\t-v Verbose output. Track progress.\n");
  printf ("\t-d Set the debug flag. Maximum verbosity.\n\n");
  
}

int main(int argc, char* argv[]){

  /* Parse arguments */
  int i = 0;
  int j = 0;
  int num = 10; // set default values for num and prnum
  int prnum = 10;
  bool dbg,mtr,ver = false; //initialize flags

  while ((i = getopt(argc,argv,"mvdn:p")) != -1)
    switch (i){
    case 'm':
      mtr = true; // Set the metrics flag
      break;
    case 'v':
      ver = true; // Set the verbosity flag
      break;
    case 'd':
      dbg = true; // Set the debug flag
      break;
    case 'n': // Set number of primes to seek
      num = atoi(optarg);
      break;
    case 'p': // Set printing interval. Defaults to "auto"
      prnum = atoi(optarg);
      break;
    default: // Set defaults if the previous two options are unset
      usage_msg(argv[0]);
      return 1;
    }
  
  dpf("\nDebug flag set.\n",0);
  
  /* Check values: */
  if ( num < 5 ) { 
    printf("\n**Error: Number of primes to seek must be at least 5. Exiting.\n");
    // 5 is the minimum for the bound limit n*log(n*log(n))
    dpf("\nnum=%d\n",num);
    usage_msg(argv[0]);
    return 1;
  }
  
  /* Initialize computation */
  vpf("\nInitializing...\n",0);
  int bound = num * log(num*log(num));   // upper bound on prime candidates
  auto pcandi = new vector<bool>(bound, false); // initialize candidate vector bitfield with bools set to false
  int * primes = new int[num];         // initialize primes array with ints
  int mult;
  
  /* Metrics initialization */
  auto metric = new string[20];
  metric[0] = "Init";
  metric[1] = "Sieve";
  metric[2] = "No Sieve";
  metric[3] = "Tot Sieve";
  metric[4] = "Primes";
  metric[5] = "No Primes";
  metric[6] = "Tot Primes";
  metric[7] = "Printing";
  metric[8] = "Total";
  /* These arrays correspond to the string names above */
  int count[20] = {0};
  float nfact[20] = {0};
  float fracops[20] = {0};

  int value;
  if(dbg == true){
    for (i = 0; i < bound; i++){
      value = static_cast<int>(pcandi->at(i));  
      dpf("\npcandi[%d] = %d", i, value);
      count[0]++; // increment initialization counter
    }
  }
  vpf("\nSieving...\n",0);
  
  /* Prime Sieve: */
  /* iterate through canidates array up to sqrt(candidates) */
  for (i = 2; i*i < bound; i++){
    value = static_cast<int>(pcandi->at(i));
     dpf("\n%d:%d",i,value);
    
    /* Check whether this candidate has already been ruled out */
    if (pcandi->at(i)==false){
      dpf("\npcandi[%d]: ",i);
    
      /* Rule out its multiples */
      for (j = 2; j < bound/i; j++){
	mult=i*j;
	pcandi->at(mult) = true;
	dpf(" %d",mult);
	count[1]++; // increment sieve counter
      }

    }else{
      count[2]++; // increment no-sieve counter
    }
  }
  count[3] = count[1] + count[2];

  /* Sieving done, fill the primes array */
  for (i=2, j=0; i < bound && j < num; i++){
    if ( !pcandi->at(i) ){
      primes[j] = i;
      j++;
      count[4]++; // increment prime counter
    }else{
      count[5]++; // increment no-prime counter
    }
  }
  count[6] = count[4] + count[5];

 // print list of primes
 vpf("\nFirst %d primes:\n",num);
  for (i = 0; i < num; i++){
    vpf("%d\t%d\n",i,primes[i]);
    count[7]++; // increment print counter
  }
 
  count[8] = count[0] + count[3] + count[6] + count[7];
  
  // print the largest prime found
  printf("\nThe \x1b[1m%dth\x1b[0m prime is \x1b[1m%d\x1b[0m! Wow!\n",num,primes[num-1]);
  
  
  // Compute N-Factors & Fractional Operations:
  float fcount;
  for (i=0; i<9; i++){
    fcount = static_cast<float>(count[i]);
    nfact[i] = log(fcount)/log(num);
    fracops[i] = fcount/count[8];
  }
  
  vpf("\nComplete!",0);
  
  /* Print Metrics */ 
  mpf("\n\x1b[1m%12s\t%12s%12s%12s\x1b[0m\n","Metric","Operations","Fraction","log_n(Ops)");
  for (i=1; i<9; i++){
    mpf("%12s\t%12d\t%7.5f   %7.5f\n",metric[i].c_str(),count[i],fracops[i],nfact[i]);
  }

  //mpf("\x1b[1m%12s\t%12d\t%7.5f\x1b[0m\n",metric[8].c_str(),count[8],nfact[8]);

  /* Free memory */
  delete pcandi; 
  delete[] primes;
  
  return 0;
 
}




