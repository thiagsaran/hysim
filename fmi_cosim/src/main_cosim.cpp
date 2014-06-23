

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

char* loadFMU(char *path, FMU *fmu);

 void replaceRefsInMessage(const char* msg, char* buffer, int nBuffer,
		FMU* fmu);
 const char* fmiStatusToString(fmiStatus status);

}

//FMU fmu; // the fmu to simulate
#define MAX_MSG_SIZE 1000
static void fmiLog(fmiComponent c, fmiString instanceName, fmiStatus status,
		fmiString category, fmiString message, ...) {
	char msg[MAX_MSG_SIZE];
	va_list argp;
	va_start(argp, message);
	vsprintf(msg, message, argp);
	printf("fmiStatus = %d;  %s (%s): %s\n", status, instanceName, category,
			msg);
}

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
	int initFMU(double currTime, double endTime);
	fmiStatus setInput(var* inVar);
	fmiStatus getInput(var* inVar);

	fmiStatus setParam(var* inParam);
	fmiStatus getParam(var* outParam);

	fmiStatus getOutput(var* outVar);

	fmiStatus unldFMU();

	char* buildFMU(char* FMU_Path) {
		return loadFMU(FMU_Path, &fmu_g);
	}

	void fmuLogger(fmiComponent c, fmiString instanceName, fmiStatus status,
			fmiString category, fmiString message, ...);
	const char* tmp_FMU_Path;
	fmiReal T_curr, T_delta;
	void rm_tmpFMU(const char*);

};

fmiStatus fmi_cosim::unldFMU() {
#ifdef _MSC_VER
	FreeLibrary(fmu.dllHandle);
#else

	fmu_g.terminateSlave(c);
	fmu_g.freeSlaveInstance(c);
	dlclose(fmu_g.dllHandle);
	rm_tmpFMU(tmp_FMU_Path);
#endif
	return fmiOK;
}

fmi_cosim::~fmi_cosim() {

}
;

fmiStatus fmi_cosim::setInput(var* tmp_in) {

	ScalarVariable** vars = fmu_g.modelDescription->modelVariables;
	fmiValueReference vr;
	ScalarVariable* sv;
	if (tmp_in->variableParsed == false) {
		for (int k = 0; vars[k]; k++) {
			sv = vars[k];
			char* s = strdup(getName(sv));
			if (strcmp(s, tmp_in->name) == 0) {
				tmp_in->vr = getValueReference(sv);
				tmp_in->variableParsed = true;
				tmp_in->type = sv->typeSpec->type;
			}
		}
	}
	switch (tmp_in->type) {
	case elm_Real:
		tmp_in->stat = fmu_g.setReal(c, &tmp_in->vr, 1, &tmp_in->value.r);
		break;
	case elm_Integer:
	case elm_Enumeration:
		tmp_in->stat = fmu_g.setInteger(c, &tmp_in->vr, 1, &tmp_in->value.i);
		break;
	case elm_Boolean:
		tmp_in->stat = fmu_g.setBoolean(c, &tmp_in->vr, 1, &tmp_in->value.b);
		break;
	case elm_String:
		tmp_in->stat = fmu_g.setString(c, &tmp_in->vr, 1, &tmp_in->value.s);
		break;
	default:
		printf("Unexpected Type error %d", sv->typeSpec->type);

	}
	return fmiOK;
}
fmiStatus fmi_cosim::getInput(var *tmp_in) {
	return fmi_cosim::getOutput(tmp_in);
}

fmiStatus fmi_cosim::setParam(var *tmp_in) {
	return fmi_cosim::setInput(tmp_in);
}

fmiStatus fmi_cosim::getParam(var *tmp_in) {
	return fmi_cosim::getOutput(tmp_in);
}
fmiStatus fmi_cosim::getOutput(var* tmp_in) {

	ScalarVariable** vars = fmu_g.modelDescription->modelVariables;
	fmiValueReference vr;
	ScalarVariable* sv;
	if (tmp_in->variableParsed == false) {
		for (int k = 0; vars[k]; k++) {
			sv = vars[k];
			char* s = strdup(getName(sv));
			if (strcmp(s, tmp_in->name) == 0) {
				tmp_in->vr = getValueReference(sv);
				tmp_in->variableParsed = true;
				tmp_in->type = sv->typeSpec->type;
			}
		}
	}
	switch (tmp_in->type) {
	case elm_Real:
		tmp_in->stat = fmu_g.getReal(c, &tmp_in->vr, 1, &tmp_in->value.r);
		break;
	case elm_Integer:
	case elm_Enumeration:
		tmp_in->stat = fmu_g.getInteger(c, &tmp_in->vr, 1, &tmp_in->value.i);
		break;
	case elm_Boolean:
		tmp_in->stat = fmu_g.getBoolean(c, &tmp_in->vr, 1, &tmp_in->value.b);
		break;
	case elm_String:
		tmp_in->stat = fmu_g.getString(c, &tmp_in->vr, 1, &tmp_in->value.s);
		break;
	default:
		printf("Unexpected Type error %d", sv->typeSpec->type);
	}
	return fmiOK;
}

int fmi_cosim::initFMU(double currTime, double endTime) {

	const char* guid;                // global unique id of the fmu

	fmiStatus fmiFlag;               // return code of the fmu functions
	const char* fmuLocation = NULL; // path to the fmu as URL, "file://C:\QTronic\sales"
	const char* mimeType = "application/x-fmu-sharedlibrary"; // denotes tool in case of tool coupling
	fmiReal timeout = 1000; // wait period in milli seconds, 0 for unlimited wait period"
	fmiBoolean visible = fmiFalse;   // no simulator user interface
	fmiBoolean interactive = fmiFalse; // simulation run without user interaction
	fmiCallbackFunctions callbacks;  // called by the model during simulation

// instantiate and initialize the fmu
	md = fmu_g.modelDescription;
	guid = getString(md, att_guid);
	callbacks.logger = fmiLog;
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
	return fmiOK;




}

int fmi_cosim::simulateFMU(double currTime, double deltaTime, double endTime) {
	fmiStatus fmiFlag;
//simulate FMU
	fmiFlag = fmu_g.doStep(c, currTime, deltaTime, fmiTrue);
	if (fmiFlag != fmiOK)
		return error("could not complete simulation of the model");

// print simulation summary
	return fmiOK; // success
}


void fmi_cosim::rm_tmpFMU(const char* tmpPath) {
	const char* fmt_cmd = "rm -rf";
	char rmcmd[50];
	sprintf(rmcmd, "%s %s", fmt_cmd, tmpPath);/*safe*/
	system(rmcmd);
	printf("\n temporary folder for FMU unzip is removed\n");
}

var var1("heatingResistor.R");

int main() {

	char a[] = "models/ControlledTemperature.fmu";
	fmiReal tol = .0002;
	fmi_cosim fmu1(a, 1, 0.001);
	int s1 = fmu1.initFMU(0, 10);
	int s2;

	for (fmiReal i = 0; i < 10; i += tol) {
		s2 = fmu1.simulateFMU(i, tol, 1);
		s1 = fmu1.getOutput(&var1);
		printf("%f : %s ,%d %d %s %d %f \n", i, fmu1.tmp_FMU_Path, s1, s2,
				var1.name, var1.vr, var1.value.r);
	}
	fmu1.unldFMU();
	cout << "done";
	return 0;
}

