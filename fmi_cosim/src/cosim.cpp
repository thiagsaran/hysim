/*
 * cosim.cpp
 *
 *  Created on: 25.06.2014
 *      Author: thiag
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <string>
#include <cosim.hpp>


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



