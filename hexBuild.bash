#!/bin/bash

# CS 4318, spring 2017
# Agent Challenge 3: Off-road rally

challenge='hex'
funcSignature='int _(const Board &, player whichPlayer)'
funcNames=''
objectFiles=''
declare -i numAgents
(( numAgents = 0 ))

CXX='g++'
CXXFLAGS='-O3 -Wall'   # Change -O3 to -g to get debug info.
LDLIBS='-lm'

if [[ ! -f "src/hex.h" || ! -f "src/hex.cpp" || ! -f "src/hexMain.cpp" ]]
then
   echo " * src/hex.h, src/hex.cpp and src/hexMain.cpp are required."
   echo " * Build failed."
   exit 1
fi

for fileName in agents/hexAgent?*.cpp
do
   funcName="${fileName#*s/}"
   funcName="${funcName%.cpp}"
   agentName="${funcName#hexAgent}"
   validAgentNameFormat='^[A-Z][0-9A-Za-z]*$'
   if [[ "${#agentName}" -le 20 && "${agentName}" =~ ${validAgentNameFormat} ]]
   then
      if ! grep "#include \"../src/hex.h\"" "${fileName}" > /dev/null
      then
         echo " * ${fileName}: No #include \"../src/hex.h\" found; skipping compilation."
      elif ! grep -w "${funcName}" "${fileName}" > /dev/null
      then
         echo " * ${fileName}: No ${funcName} function found; skipping compilation."
      else
         echo " * ${fileName}: Compiling . . ."
         if ${CXX} ${CXXFLAGS} -c "${fileName}"
         then
            echo " * ${fileName}: Compilation succeeded."
            funcNames="${funcNames} ${funcName}"
            objectFiles="${objectFiles} ${funcName}.o"
            (( numAgents += 1 ))
         else
            echo " * ${fileName}: Compilation failed; skipping agent ${agentName}."
         fi
      fi
   else
      echo " * ${fileName}: Invalid agent name; skipping compilation."
   fi
done

echo " * ${numAgents} agents found."
if (( numAgents < 2 ))
then
   echo " * At least two agents (e.g., hexAgentSmith.cpp) is needed."
   echo " * Build failed."
   rm *.o
   exit 2
fi

cat >| "hexFuncs.cpp" << END
#include "src/hex.h"

END
for funcName in ${funcNames}
do
   cat >> "hexFuncs.cpp" << END
extern ${funcSignature/_/${funcName}};
END
done
cat >> "hexFuncs.cpp" << END

extern const int numAgents = ${numAgents};

${funcSignature/_/(*agentFunc[numAgents])} = {
END
for funcName in ${funcNames}
do
   cat >> "hexFuncs.cpp" << END
   &${funcName},
END
done
cat >> "hexFuncs.cpp" << END
};

string agentStr[numAgents] = {
END
for funcName in ${funcNames}
do
   cat >> "hexFuncs.cpp" << END
   "${funcName#hexAgent}",
END
done
cat >> "hexFuncs.cpp" << END
};
END

echo " * hex.cpp: Compiling . . ."
if ${CXX} ${CXXFLAGS} -c "src/hex.cpp"
then
   echo " * hex.cpp: Compilation succeeded."
else
   echo " * hex.cpp: Compilation failed."
   echo " * Build failed."
   rm *.o
   exit 3
fi

echo " * hexFuncs.cpp: Compiling . . ."
if ${CXX} ${CXXFLAGS} -c "hexFuncs.cpp"
then
   echo " * hexFuncs.cpp: Compilation succeeded."
   rm -f "hexFuncs.cpp"
else
   echo " * hexFuncs.cpp: Compilation failed."
   echo " * Build failed."
   rm *.o
   exit 4
fi

echo " * hexMain.cpp: Compiling . . ."
if ${CXX} ${CXXFLAGS} -c "src/hexMain.cpp"
then
   echo " * hexMain.cpp: Compilation succeeded."
else
   echo " * hexMain.cpp: Compilation failed."
   echo " * Build failed."
   rm *.ohex
   exit 5
fi

echo " * Linking . . ."
execFile="hexRunSim"
rm -f "${execFile}"
if ${CXX} ${LDLIBS} -o "${execFile}" "hexMain.o" "hexFuncs.o" "hex.o" ${objectFiles}
then
   echo " * Linking succeeded."
   echo " * Executable file ./${execFile} is ready to run!"
else
   echo " * Linking failed."
   echo " * Build failed."
   rm *.o
   exit 6
fi

rm *.o
