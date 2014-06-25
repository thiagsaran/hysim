#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <string>
// project includes
#include <fmi_cosim.h>
#include <support_cosim.hpp>
#include <cosim.hpp>

using namespace std;



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

