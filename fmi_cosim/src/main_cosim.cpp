/* ------------------------------------------------------------------------- 
 * main.cpp
 * Implements simulation of a single FMU instance 
 * that implements the "FMI for Co-Simulation 1.0" interface.
 * Command syntax: see printHelp()
 * Simulates the given FMU from t = 0 .. tEnd with fixed step size h and 
 * writes the computed solution to file 'result.csv'.
 * The CSV file (comma-separated values) may e.g. be plotted using 
 * OpenOffice Calc or Microsoft Excel. 
 * This progamm demonstrates basic use of an FMU.
 * Real applications may use advanced master algorithms to cosimulate 
 * many FMUs, limit the numerical error using error estimation
 * and back-stepping, provide graphical plotting utilities, debug support, 
 * and user control of parameter and start values, or perform a clean
 * error handling (e.g. call freeSlaveInstance when a call to the fmu
 * returns with error). All this is missing here.
 *
 * Revision history
 *  22.08.2011 initial version released in FMU SDK 1.0.2
 *     
 * Free libraries and tools used to implement this simulator:
 *  - header files from the FMU specification
 *  - eXpat 2.0.1 XML parser, see http://expat.sourceforge.net
 *  - 7z.exe 4.57 zip and unzip tool, see http://www.7-zip.org
 * Author: Jakob Mauss
 * Copyright 2011 QTronic GmbH. All rights reserved. 
 * -------------------------------------------------------------------------
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <string>

#include <config_cosim.h>
#include <fmi_cosim.h>

#include <support_cosim.h>

using namespace std;

extern "C" {

char* ldFMU(char *path, FMU *fmu);
}

FMU fmu; // the fmu to simulate

// simulate the given FMU using the forward euler method.
// time events are processed by reducing step size to exactly hit tNext.
// state events are checked and fired only at the end of an Euler step. 
// the simulator may therefore miss state events and fires state events typically too late.
static int simulate(FMU* fmu, double tEnd, double h, fmiBoolean loggingOn,
		char separator) {
	double time;
	double tStart = 0;               // start time
	const char* guid;                // global unique id of the fmu
	fmiComponent c;                  // instance of the fmu
	fmiStatus fmiFlag;               // return code of the fmu functions
	const char* fmuLocation = NULL; // path to the fmu as URL, "file://C:\QTronic\sales"
	const char* mimeType = "application/x-fmu-sharedlibrary"; // denotes tool in case of tool coupling
	fmiReal timeout = 1000; // wait period in milli seconds, 0 for unlimited wait period"
	fmiBoolean visible = fmiFalse;   // no simulator user interface
	fmiBoolean interactive = fmiFalse; // simulation run without user interaction
	fmiCallbackFunctions callbacks;  // called by the model during simulation
	ModelDescription* md;            // handle to the parsed XML file
	int nSteps = 0;
	FILE* file;

	// instantiate the fmu
	md = fmu->modelDescription;
	guid = getString(md, att_guid);
	callbacks.logger = fmuLogger;
	callbacks.allocateMemory = calloc;
	callbacks.freeMemory = free;
	callbacks.stepFinished = NULL; // fmiDoStep has to be carried out synchronously
	c = fmu->instantiateSlave(getModelIdentifier(md), guid, fmuLocation,
			mimeType, timeout, visible, interactive, callbacks, loggingOn);
	if (!c)
		return error("could not instantiate model");

	// open result file
	if (!(file = fopen(RESULT_FILE, "w"))) {
		printf("could not write %s because:\n", RESULT_FILE);
		printf("    %s\n", strerror(errno));
		return 0; // failure
	}

	//setting a parameter before initializeSlave

	fmiBoolean select1_k = 0;
	fmiBoolean select1_y;
	fmiValueReference select1_k_vr = 536870954;
	fmiValueReference select1_y_vr = 536870964;

	fmiFlag = fmu->setBoolean(c, &select1_k_vr, 1, &select1_k);
	fmiFlag = fmu->getBoolean(c, &select1_y_vr, 1, &select1_y);
	printf("\nvalue of scalar variable select1.y is %d\n", select1_y);

	// StopTimeDefined=fmiFalse means: ignore value of tEnd
	fmiFlag = fmu->initializeSlave(c, tStart, fmiTrue, tEnd);
	if (fmiFlag > fmiWarning)
		return error("could not initialize model");

#ifdef TRACE_LOG
	// output solution for time t0
	outputRow(fmu, c, tStart, file, separator, TRUE); // output column names
	outputRow(fmu, c, tStart, file, separator, FALSE); // output values
#endif

// enter the simulation loop
	time = tStart;
	while (time < tEnd) {
		fmiFlag = fmu->doStep(c, time, h, fmiTrue);
		if (fmiFlag != fmiOK)
			return error("could not complete simulation of the model");
		time += h;
#ifdef TRACE_LOG
		outputRow(fmu, c, time, file, separator, FALSE); // output values for this step
#endif
		nSteps++;
	}

// end simulation
	fmiFlag = fmu->terminateSlave(c);
	fmu->freeSlaveInstance(c);
	fclose(file);

// print simulation summary
	printf("Simulation from %g to %g terminated successful\n", tStart, tEnd);
	printf("  steps ............ %d\n", nSteps);
	printf("  fixed step size .. %g\n", h);
	return 1; // success
}

#ifdef FMU_TESTER
int main(int argc, char *argv[]) {
	char* fmuFileName;
	const char* fmt_cmd = "rm -rf";
	char rmcmd[50];

// parse command line arguments and load the FMU
	double tEnd = 1.0;
	double h = 0.1;
	int loggingOn = 0;
	char csv_separator = ' ';
	char* tmpPath;
	parseArguments(argc, argv, &fmuFileName, &tEnd, &h, &loggingOn,
			&csv_separator);
	tmpPath = loadFMU(fmuFileName);
// *rmcmd= (char*)calloc(sizeof(char), strlen(tmpPath)+strlen(fmt_cmd)+1);
	printf("\n***tmp path : %s \n", tmpPath);
// run the simulation
	printf(
			"FMU Simulator: run '%s' from t=0..%g with step size h=%g, loggingOn=%d, csv separator='%c'\n",
			fmuFileName, tEnd, h, loggingOn, csv_separator);
	simulate(&fmu, tEnd, h, loggingOn, csv_separator);
	printf("CSV file '%s' written\n", RESULT_FILE);

// release FMU
#ifdef _MSC_VER
	FreeLibrary(fmu.dllHandle);
#else
	dlclose(fmu.dllHandle);
#endif
	freeElement(fmu.modelDescription);
	sprintf(rmcmd, "%s %s", fmt_cmd, tmpPath);/*safe*/

// printf("\n***tmp path : %s \n",rmcmd);
	system(rmcmd);
	printf("\ntmp folder removed\n");

	return EXIT_SUCCESS;
}
#endif

#ifndef FMI_WRAP

struct var {

	fmiString name;
	union {
		fmiReal r;
		fmiInteger i;
		fmiString s;
		fmiBoolean b;
	} value;
	fmiStatus stat;
	fmiValueReference vr;

	bool variableParsed = false;

	var(){};
	var(fmiString varname) {
		name = varname;
	};
};


class fmi_cosim {
	var tmp_in;
	var tmp_out, tmp_par;
public:
	fmi_cosim(char* FMU_Path, fmiReal Tcurr, fmiReal Tdelta) {
		T_curr = Tcurr;
		T_delta = Tdelta;
		tmp_FMU_Path = buildFMU(FMU_Path);
	}
	~fmi_cosim();

	FMU fmu_g;
	fmiComponent c;                  // instance of the fmu
	ModelDescription *md;            // handle to the parsed XML file
	int simulateFMU(double currTime, double deltaTime, double endTime);
	var* setInput(char* varName, var* inVar);
	var* getInput(char* varName, var* outVar);

	var* setParam(char* varName, var* inParam);
	var* getParam(char* varName, var* outParam);

	var* getOutput(char* inVar, var* outVar);

	fmiStatus unldFMU();

	char* buildFMU(char* FMU_Path) {
		return ldFMU(FMU_Path, &fmu_g);
	}
//	char* ldFMU(const char *,FMU *);

	const char* tmp_FMU_Path;
	fmiReal T_curr, T_delta;
	void rm_tmpFMU(const char*);
	// end simulation
};

fmiStatus fmi_cosim::unldFMU() {
#ifdef _MSC_VER
	FreeLibrary(fmu.dllHandle);
#else
	dlclose(fmu_g.dllHandle);
	//fmu_g.terminateSlave(c);
	//fmu_g.freeSlaveInstance(c);
	rm_tmpFMU(tmp_FMU_Path);
#endif
	return fmiOK;
}

fmi_cosim::~fmi_cosim() {

}
;

var* fmi_cosim::setInput(char *inVar, var* tmp_in) {

	ScalarVariable** vars = fmu.modelDescription->modelVariables;
	fmiValueReference vr;
	ScalarVariable* sv;
	if (tmp_in->variableParsed == false) {
		for (int k = 0; vars[k]; k++) {
			sv = vars[k];
			char* s = strdup(getName(sv));
			if (strcmp(s, tmp_in->name)) {
				tmp_in->vr = getValueReference(sv);
				tmp_in->variableParsed = true;
			}
		}
	}
	switch (sv->typeSpec->type) {
	case elm_Real:
		tmp_in->stat = fmu.setReal(c, &tmp_in->vr, 1, &tmp_in->value.r);
		break;
	case elm_Integer:
	case elm_Enumeration:
		tmp_in->stat = fmu.setInteger(c, &tmp_in->vr, 1, &tmp_in->value.i);
		break;
	case elm_Boolean:
		tmp_in->stat = fmu.setBoolean(c, &tmp_in->vr, 1, &tmp_in->value.b);
		break;
	case elm_String:
		tmp_in->stat = fmu.setString(c, &tmp_in->vr, 1, &tmp_in->value.s);
		break;
	default:
		printf("Unexpected Type error %d", sv->typeSpec->type);

	}
	return tmp_in;
}
var* fmi_cosim::getInput(char *inVar, var *tmp_in) {
	return fmi_cosim::getOutput(inVar, tmp_in);
}

var* fmi_cosim::setParam(char *inVar, var *tmp_in) {
	return fmi_cosim::setInput(inVar, tmp_in);
}

var* fmi_cosim::getParam(char *inVar, var *tmp_in) {
	return fmi_cosim::getOutput(inVar, tmp_in);
}
var* fmi_cosim::getOutput(char *inVar, var* tmp_in) {

	ScalarVariable** vars = fmu.modelDescription->modelVariables;
	fmiValueReference vr;
	ScalarVariable* sv;
	if (tmp_in->variableParsed == false) {
		for (int k = 0; vars[k]; k++) {
			sv = vars[k];
			char* s = strdup(getName(sv));
			if (strcmp(s, tmp_in->name)) {
				tmp_in->vr = getValueReference(sv);
				tmp_in->variableParsed = true;
			}
		}
	}
	switch (sv->typeSpec->type) {
	case elm_Real:
		tmp_in->stat = fmu.getReal(c, &tmp_in->vr, 1, &tmp_in->value.r);
		break;
	case elm_Integer:
	case elm_Enumeration:
		tmp_in->stat = fmu.getInteger(c, &tmp_in->vr, 1, &tmp_in->value.i);
		break;
	case elm_Boolean:
		tmp_in->stat = fmu.getBoolean(c, &tmp_in->vr, 1, &tmp_in->value.b);
		break;
	case elm_String:
		tmp_in->stat = fmu.getString(c, &tmp_in->vr, 1, &tmp_in->value.s);
		break;
	default:
		printf("Unexpected Type error %d", sv->typeSpec->type);
	}
	return tmp_in;
}

void fmi_cosim::rm_tmpFMU(const char* tmpPath) {
	const char* fmt_cmd = "rm -rf";
	char rmcmd[50];
	sprintf(rmcmd, "%s %s", fmt_cmd, tmpPath);/*safe*/
	system(rmcmd);
	printf("\n temporary folder for FMU unzip is removed\n");
}

int fmi_cosim::simulateFMU(double currTime, double deltaTime, double endTime) {
	double time;
	const char* guid;                // global unique id of the fmu

	fmiStatus fmiFlag;               // return code of the fmu functions
	const char* fmuLocation = NULL; // path to the fmu as URL, "file://C:\QTronic\sales"
	const char* mimeType = "application/x-fmu-sharedlibrary"; // denotes tool in case of tool coupling
	fmiReal timeout = 1000; // wait period in milli seconds, 0 for unlimited wait period"
	fmiBoolean visible = fmiFalse;   // no simulator user interface
	fmiBoolean interactive = fmiFalse; // simulation run without user interaction
	fmiCallbackFunctions callbacks;  // called by the model during simulation

//	int nSteps = 0;

// instantiate the fmu
	md = fmu_g.modelDescription;
	guid = getString(md, att_guid);
	callbacks.logger = fmuLogger;
	callbacks.allocateMemory = calloc;
	callbacks.freeMemory = free;
	callbacks.stepFinished = NULL; // fmiDoStep has to be carried out synchronously
	c = fmu_g.instantiateSlave(getModelIdentifier(md), guid, fmuLocation,
			mimeType, timeout, visible, interactive, callbacks, fmiTrue);
	if (!c)
		return error("could not instantiate model");
// StopTimeDefined=fmiFalse means: ignore value of tEnd
	fmiFlag = fmu_g.initializeSlave(c, currTime, fmiTrue, endTime);
	if (fmiFlag > fmiWarning)
		return error("could not initialize model");
	fmiFlag = fmu_g.doStep(c, currTime, deltaTime, fmiTrue);
	if (fmiFlag != fmiOK)
		return error("could not complete simulation of the model");
//	nSteps++;

// print simulation summary
	return fmiOK; // success
}

int main() {

	char a[] = "models/ControlledTemperature.fmu";
	var var1("time");
	fmi_cosim fmu1(a, 1, 0.1);
	int s = fmu1.simulateFMU(1, 0.1, 2);
	printf("%s ,%d %s", fmu1.tmp_FMU_Path, s, var1.name);
	fmu1.unldFMU();
	cout << "done";
	return 0;
}

#endif
