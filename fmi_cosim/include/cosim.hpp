/** @mainpage hysim hybrid h
* @file cosim.hpp
*
* @brief This file contains interfaces to communicate with FMI for Co-Simulation. Current version works for FMI - Version 1.0.
* This package is one of the different packages of hysim - hybrid simulation
*
* @author Thiyagarajan Purusothaman
*
* @date June, 25th 2014 - hysim v1.0
*
**/

#ifndef COSIM_HPP_
#define COSIM_HPP_

#include <fmi_cosim.h>
#include <support_cosim.hpp>

/**
 * @struct var
 *
 * @brief An interface variable between FMUs and master simulator.
 * <A data structure to hold the temporary variable in Master simulator
 * The different attributes are from the modelDescription.xml file. In this data structure
 * only required attributes for Co-simulation to execute is used>
 *
 */

struct var {

	fmiString name; //name of FMU Scalar variable
	union {
		fmiReal r;
		fmiInteger i;
		fmiString s;
		fmiBoolean b;
	} value; // type of the Scalar variable, defined as union to resolve at runtime
	Elm type;// type of the element, here its real,integer,string or boolean
	fmiStatus stat;// fmiStatus as a result of last operation over the variable
	fmiValueReference vr; // a unique variable reference number for the variable as parsed from modelDescription.xml file
	bool variableParsed;// a status flag to avoid repeated paring of the modelDescription.xml file
	// default constructor when variable name is not decided during declaration
	var() {
		name = "";
		stat = fmiOK;
		vr = 0;
		variableParsed = false;
		type = elm_ANY_TYPE;
	};
	// constructor when declared with a name
	var(fmiString varname) {
		name = varname;
		stat = fmiOK;
		vr = 0;
		variableParsed = false;
		type = elm_ANY_TYPE;
	}
	;
};

/**
 * @class fmi_cosim
 * @brief class for handling the FMU Co-simulation related activities
 *
 * <The scalar variable is passed as input and simulation is carried over and updated over time and time>
 */

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


#endif /* COSIM_HPP_ */
