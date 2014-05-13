In [5]: from pymodelica import compile_fmu

In [6]: model_name='SineSource'

In [7]: mo_file='SineSource.mo'

In [8]: my_fmu = compile_fmu(model_name, mo_file,target='cs')
JVM started.
clang: warning: argument unused during compilation: '-pthread'

In [9]: ControlledTemperature.fmu  SineSource.mo
HeatResPICtrl.mo           jmodelica_cmd.txt
SineSource.fmu

#############################################################

Type above commands in Jmodelica to export a modelica based model.

this creates a fmu file for cosimulation

#############################################################

time ../bin/fmi_cosim ../models/SineSource.fmu 10 0.0001 1
cmd='7z x -aoa -ofmuTmpWFtxQl/ "../models/SineSource.fmu" > /dev/null'
fmiModelDescription
  fmiVersion=1.0
  modelName=SineSource
  modelIdentifier=SineSource
  guid=67654bb5c1b12cfd2fa1b00b8c90ab01
  generationTool=JModelica.org
  generationDateAndTime=2014-05-13T13:10:47
  variableNamingConvention=structured
  numberOfContinuousStates=0
  numberOfEventIndicators=0
Capabilities
  canHandleVariableCommunicationStepSize=true
  canHandleEvents=true
  canRejectSteps=false
  canInterpolateInputs=true
  maxOutputDerivativeOrder=0
  canRunAsynchronuously=false
  canSignalEvents=false
  canBeInstantiatedOnlyOncePerProcess=true
  canNotUseMemoryManagementFunctions=false
dllPath = fmuTmpWFtxQl/binaries/darwin64/SineSource.dylib

***tmp path : fmuTmpWFtxQl/ 
FMU Simulator: run '../models/SineSource.fmu' from t=0..10 with step size h=0.0001, loggingOn=1, csv separator=' '
Simulation from 0 to 10 terminated successful
  steps ............ 100001
  fixed step size .. 0.0001
CSV file 'result.csv' written

tmp folder removed

real	0m1.902s
user	0m1.850s
sys	0m0.051s
