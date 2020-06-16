#!/bin/bash

if [ "$#" -ne 2 ]; then
  echo "Illegal number of parameters!"
  exit
fi
re='^[0-9]+$'
if ! [[ $1 =~ $re ]] ; then
   echo "Error: Parameter 1 (normal iterations) is not a number!" >&2; exit 1
fi
re='^[0-9]+$'
if ! [[ $2 =~ $re ]] ; then
   echo "Error: Parameter 2 (Monte Carlo iterations) is not a number!" >&2; exit 1
fi
# Number of iterations set by parameter                    
ITERATIONS=$1                   
# Number of approaches per iterations set by parameter
MONTECARLO=$2         

# If we don't already have a parameter file
if [ ! -f "parameter.txt" ]
then
  echo -e "500\n50\n50\n0.6\n0.1\n10\n10\n100\n6\n1\n5\n30\n10\n1\n25\n20\n0.01\n0.01\n0.01\n0.01\n0.01\n0.05\n0.01\n0.01\n10\n20\n20\n50\n30\n10\n60\n30\n1\n0\n2\n5\n2\n1\n0\n2\n0.33\n0.33\n0.33\n1\n1\n1\n1\n0.33\n40\n20\n1\n1\n25\n20\n2.5\n0.01\n0.01\n0.04\n0.05\n0.01\n0.01\n0.01\n100\n20\n10\n12\n60\n30\n10\n5\n5\n1\n1\n0\n2\n2\n0.33\n1\n1\n0.33\n1\n0.33\n0.5\n4\n4\n5\n5\n1\n1\n150\n100\n0.2\n0.7\n0.35\n0\n1\n0\n0\n1" > parameter.txt
fi

# Load Parameter from file
PARAMETER=( $( cat parameter.txt ) )

# Set indexes that enhance plants
PROPLANTLEN=16
PROPLANT=( $( echo -e "8\n9\n10\n11\n12\n13\n15\n16\n17\n18\n19\n20\n21\n23\n85\n86" ) )
# Set step for these parameter
PROPLANTSTEP=( $( echo -e "1\n1\n1\n1\n1\n1\n1\n0.01\n0.01\n0.01\n0.01\n0.01\n0.01\n0.01\n0.05\n0.05" ) )
# Set bounds for these parameter
PROPLANTMAX=( $( echo -e "20.0\n20.0\n20.0\n50.0\n30.0\n10.0\n30.0\n1.0\n1.0\n1.0\n1.0\n1.0\n1.0\n1.0\n20\n20" ) )
PROPLANTMIN=( $( echo -e "1\n0\n2\n5\n2\n1\n2\n0.01\n0.01\n0.01\n0.01\n0.01\n0.01\n0.01\n0.05\n0.05" ) )

# Set indexes that enhance plants
PROHERBIVORELEN=14
PROHERBIVORE=( $( echo -e "48\n49\n50\n51\n53\n54\n55\n56\n57\n58\n60\n61\n83\n84" ) )
# Set step for these parameter
PROHERBIVORESTEP=( $( echo -e "1\n1\n1\n1\n1\n1\n0.01\n0.01\n0.01\n0.01\n0.01\n0.01\n0.05\n0.05" ) )
# Set bounds for these parameter
PROHERBIVOREMAX=( $( echo -e "100\n20\n10\n12\n30\n10\n1\n1\n1\n1\n1\n1\n20\n20" ) )
PROHERBIVOREMIN=( $( echo -e "5\n5\n1\n1\n2\n2\n0.01\n0.01\n0.01\n0.01\n0.01\n0.01\n0.05\n0.05" ) )


curBestMCMonthCount=0
TESTPARAMETER=(${PARAMETER[*]})
# Run all iterations
for i in $(seq 1 1 $ITERATIONS)
do
  # Temporary variable for counting strong herbivore runs in one MC approach.
  testHerbivoreStrong=0
  # Temporary variable for counting strong plant runs in one MC approach.
  testPlantStrong=0
  # Temporary variable to count the duration of all runs
  tmpTotalMonthPerMC=0

  plantStrong=0
  herbivoreStrong=0

  # Run all MC iterations here
  for i in $(seq 1 1 $MONTECARLO)
  do
    # One single simulationRun
    python3 ../pythonSimulation/simulation.py ${PARAMETER[*]}
    case $? in
	  1) 
        echo " Herbivore distinct "
        testPlantStrong=`expr $testPlantStrong + 1`
        ;;
	  2) 
        echo " Plants distinct "
        testHerbivoreStrong=`expr $testHerbivoreStrong + 1`
        ;;
      *) 
        echo " TIE "
        ;;
    esac
    tmpRunMonthCount=`wc -l "plantHerbivoreAbundance.csv"|cut -f1 -d" "`
    tmpTotalMonthPerMC=`expr $tmpTotalMonthPerMC + $tmpRunMonthCount`
  done

  tmpTotalMonthPerMC=`echo "$tmpTotalMonthPerMC / $MONTECARLO" | bc`
  
  # If the runs are full length, increase duration for a run by 10%
  diffMaxMonth=`echo "${PARAMETER[0]} - $tmpTotalMonthPerMC + 1" | bc`
  if [ $diffMaxMonth -eq 0 ]
  then
    PARAMETER[0]=`echo "${PARAMETER[0]} * 1.10" | bc`
  fi

  # If the old run was worse than the new one, use new data
  if [ $curBestMCMonthCount -le $tmpTotalMonthPerMC ]
  then
    curBestMCMonthCount=$tmpTotalMonthPerMC
    PARAMETER=(${TESTPARAMETER[*]})
    plantStrong=$testPlantStrong
    herbivoreStrong=$testHerbivoreStrong
    # Save parameter to file!
    printf "%s\n" "${PARAMETER[@]}" > parameter.txt
  fi

  # From PARAMETER we are creating new test parameter (i.e. TESTPARAMETER)
  if [ $plantStrong -gt $herbivoreStrong ]
  then
    # Enhance herbivore or weaken plants
    if [ `echo "$RANDOM % 2" | bc` -eq 0 ]
    then
      # Enhance herviore
      # Pick a random parameter
	  ii=`echo "$RANDOM % $PROHERBIVORELEN" | bc`
      i=${PROHERBIVORE[$ii]}
      step=${PROHERBIVORESTEP[$ii]}
      max=${PROHERBIVOREMAX[$ii]}
      # Check for threshold
      if [ `echo "$max > ${PARAMETER[$i]}"|bc` -eq 1 ]
	  then
        TESTPARAMETER[$i]=`echo "${PARAMETER[$i]} + $step" | bc `
      fi
    else
      # Weaken plants
      # Pick a random parameter
	  ii=`echo "$RANDOM % $PROPLANTLEN" | bc`
      i=${PROPLANT[$ii]}
      step=${PROPLANTSTEP[$ii]}
      min=${PROPLANTMIN[$ii]}
      # Check for threshold
      if [ `echo "$min < ${PARAMETER[$i]}"|bc` -eq 1 ]
	  then
        TESTPARAMETER[$i]=`echo "${PARAMETER[$i]} - $step" | bc `
      fi
    fi
  else
    # Enhance plants or weaken herbivore
    if [ `echo "$RANDOM % 2" | bc` -eq 0 ]
    then
      # Enhance plants
      # Pick a random parameter
	  ii=`echo "$RANDOM % $PROPLANTLEN" | bc`
      i=${PROPLANT[$ii]}
      step=${PROPLANTSTEP[$ii]}
      max=${PROPLANTMAX[$ii]}
      # Check for threshold
      if [ `echo "$max > ${PARAMETER[$i]}"|bc` -eq 1 ]
	  then
        TESTPARAMETER[$i]=`echo "${PARAMETER[$i]} + $step" | bc `
      fi
    else
      # Weaken herviore
      # Pick a random parameter
	  ii=`echo "$RANDOM % $PROHERBIVORELEN" | bc`
      i=${PROHERBIVORE[$ii]}
      step=`echo "${PROHERBIVORESTEP[$ii]} * -1" | bc`
      min=${PROHERBIVOREMIN[$ii]}
      # Check for threshold
      if [ `echo "$min < ${PARAMETER[$i]}"|bc` -eq 1  ]
	  then
        TESTPARAMETER[$i]=`echo "${PARAMETER[$i]} - $step" | bc `
      fi
    fi
  fi
done











