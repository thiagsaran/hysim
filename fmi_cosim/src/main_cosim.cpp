#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <string>

#include <config_cosim.h>
#include <fmi_cosim.h>

#include <support_cosim.h>

using namespace std;


class fmi_cosim {
	var tmp_in;
	var tmp_out, tmp_par;
public:

	static FMU fmu_g;

	fmi_cosim(char* FMU_Path, fmiReal Tcurr, fmiReal Tdelta) {
		T_curr = Tcurr;
		T_delta = Tdelta;
		tmp_FMU_Path = buildFMU(FMU_Path);
	}
	~fmi_cosim();

	fmiComponent c;                  // instance of the fmu
	ModelDescription *md;            // handle to the parsed XML file
	int simulateFMU(double currTime, double deltaTime, double endTime);

	int initFMU(double currTime, double endTime);

	fmiStatus setInput(var* inVar);
	fmiStatus getInput(var* inVar);

	fmiStatus setParam(var* inParam);
	fmiStatus getParam(var* outParam);

	fmiStatus getOutput(var* outVar);

	fmiStatus unloadFMU();

	char* buildFMU(char* FMU_Path) {
		return loadFMU(FMU_Path, &fmu_g);
	}

	const char* tmp_FMU_Path;
	fmiReal T_curr, T_delta;
	void rm_tmpFMU(const char*);

	friend void fmuLogger(fmiComponent c, fmiString instanceName,
			fmiStatus status, fmiString category, fmiString message, ...);
	friend void replaceRefsInMessage(const char* msg, char* buffer, int nBuffer,
			FMU* fmu);
	friend ScalarVariable* getSV_CS(FMU* fmu, char type, fmiValueReference vr);
	friend const char* fmiStatusToString_CS(fmiStatus status);

};

fmiStatus fmi_cosim::unloadFMU() {
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

FMU fmi_cosim::fmu_g;

// replace e.g. #r1365# by variable name and ## by # in message
// copies the result to buffer
void replaceRefsInMessage(const char* msg, char* buffer, int nBuffer,
		FMU* fmu) {
	int i = 0; // position in msg
	int k = 0; // position in buffer
	int n;
	char c = msg[i];
	while (c != '\0' && k < nBuffer) {
		if (c != '#') {
			buffer[k++] = c;
			i++;
			c = msg[i];
		} else {
			char* end = strchr(msg + i + 1, '#');
			if (!end) {
				printf("unmatched '#' in '%s'\n", msg);
				buffer[k++] = '#';
				break;
			}
			n = end - (msg + i);
			if (n == 1) {
				// ## detected, output #
				buffer[k++] = '#';
				i += 2;
				c = msg[i];
			} else {
				char type = msg[i + 1]; // one of ribs
				fmiValueReference vr;
				int nvr = sscanf(msg + i + 2, "%u", &vr);
				if (nvr == 1) {
					// vr of type detected, e.g. #r12#
					ScalarVariable* sv = getSV_CS(fmu, type, vr);
					const char* name = sv ? getName(sv) : "?";
					sprintf(buffer + k, "%s", name);
					k += strlen(name);
					i += (n + 1);
					c = msg[i];
				} else {
					// could not parse the number
					printf("illegal value reference at position %d in '%s'\n",
							i + 2, msg);
					buffer[k++] = '#';
					break;
				}
			}
		}
	} // while
	buffer[k] = '\0';
}

#define MAX_MSG_SIZE 1000
void fmuLogger(fmiComponent c, fmiString instanceName, fmiStatus status,
		fmiString category, fmiString message, ...) {
	char msg[MAX_MSG_SIZE];
	char* copy;
	va_list argp;

	// replace C format strings
	va_start(argp, message);
	vsprintf(msg, message, argp);

	// replace e.g. ## and #r12#
	copy = strdup(msg);

	replaceRefsInMessage(copy, msg, MAX_MSG_SIZE, &fmi_cosim::fmu_g);
	free(copy);

	// print the final message
	if (!instanceName)
		instanceName = "?";
	if (!category)
		category = "?";
	printf("%s %s (%s): %s\n", fmiStatusToString_CS(status), instanceName,
			category, msg);
}

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

	callbacks.logger = (fmiCallbackLogger) (&fmuLogger);
	callbacks.allocateMemory = calloc;
	callbacks.freeMemory = free;
	callbacks.stepFinished = NULL; // fmiDoStep has to be carried out synchronously
	c = fmu_g.instantiateSlave(getModelIdentifier(md), guid, fmuLocation,
			mimeType, timeout, visible, interactive, callbacks, fmiTrue);
	if (!c)
		return error("could not instantiate model");

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
var var2("and1.u2");
var var3("onOffController.reference");
var var4("onOffController.reference");

int main() {

	var2.value.b = false;
	var3.value.r = 100;
	char a[] = "models/ControlledTemperature.fmu";
	fmiReal tol = .2;
	fmi_cosim fmu1(a, 1, 0.001);
	int s1 = fmu1.initFMU(0, 10);
	int s2;

	for (fmiReal i = 0; i < 10; i += tol) {
		s2 = fmu1.simulateFMU(i, tol, 1);
		s1 = fmu1.getOutput(&var1);
		s2 = fmu1.setParam(&var2);

		cout << "input setting \n" << fmu1.setInput(&var3) << endl;
		cout << "input getting \n" << fmu1.getInput(&var4) << var4.value.r
				<< endl;
		printf("%f : %s ,%d %d %s %d %f \n", i, fmu1.tmp_FMU_Path, s1, s2,
				var1.name, var1.vr, var1.value.r);
	}
	fmu1.unloadFMU();
	cout << "done";
	return 0;
}

