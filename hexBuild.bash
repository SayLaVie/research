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

if [[ ! -f "${challenge}.h" || ! -f "${challenge}.cpp" || ! -f "${challenge}Main.cpp" ]]
then
   echo " * ${challenge}.h, ${challenge}.cpp and ${challenge}Main.cpp are required."
   echo " * Build failed."
   exit 1
fi

for fileName in ${challenge}Agent?*.cpp
do
   funcName="${fileName%.cpp}"
   agentName="${funcName#${challenge}Agent}"
   validAgentNameFormat='^[A-Z][0-9A-Za-z]*$'
   if [[ "${#agentName}" -le 20 && "${agentName}" =~ ${validAgentNameFormat} ]]
   then
      if ! grep "#include \"${challenge}.h\"" "${fileName}" > /dev/null
      then
         echo " * ${fileName}: No #include \"${challenge}.h\" found; skipping compilation."
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
   echo " * At least two agents (e.g., ${challenge}AgentSmith.cpp) is needed."
   echo " * Build failed."
   rm *.o
   exit 2
fi

cat >| "${challenge}Funcs.cpp" << END
#include "${challenge}.h"

END
for funcName in ${funcNames}
do
   cat >> "${challenge}Funcs.cpp" << END
extern ${funcSignature/_/${funcName}};
END
done
cat >> "${challenge}Funcs.cpp" << END

extern const int numAgents = ${numAgents};

${funcSignature/_/(*agentFunc[numAgents])} = {
END
for funcName in ${funcNames}
do
   cat >> "${challenge}Funcs.cpp" << END
   &${funcName},
END
done
cat >> "${challenge}Funcs.cpp" << END
};

string agentStr[numAgents] = {
END
for funcName in ${funcNames}
do
   cat >> "${challenge}Funcs.cpp" << END
   "${funcName#${challenge}Agent}",
END
done
cat >> "${challenge}Funcs.cpp" << END
};
END

echo " * ${challenge}.cpp: Compiling . . ."
if ${CXX} ${CXXFLAGS} -c "${challenge}.cpp"
then
   echo " * ${challenge}.cpp: Compilation succeeded."
else
   echo " * ${challenge}.cpp: Compilation failed."
   echo " * Build failed."
   rm *.o
   exit 3
fi

echo " * ${challenge}Funcs.cpp: Compiling . . ."
if ${CXX} ${CXXFLAGS} -c "${challenge}Funcs.cpp"
then
   echo " * ${challenge}Funcs.cpp: Compilation succeeded."
   rm -f "${challenge}Funcs.cpp"
else
   echo " * ${challenge}Funcs.cpp: Compilation failed."
   echo " * Build failed."
   rm *.o
   exit 4
fi

echo " * ${challenge}Main.cpp: Compiling . . ."
if ${CXX} ${CXXFLAGS} -c "${challenge}Main.cpp"
then
   echo " * ${challenge}Main.cpp: Compilation succeeded."
else
   echo " * ${challenge}Main.cpp: Compilation failed."
   echo " * Build failed."
   rm *.o
   exit 5
fi

echo " * Linking . . ."
execFile="${challenge}RunSim"
rm -f "${execFile}"
if ${CXX} ${LDLIBS} -o "${execFile}" "${challenge}Main.o" "${challenge}Funcs.o" "${challenge}.o" ${objectFiles}
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
