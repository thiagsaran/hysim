model ControlledTemperature "Control temperature of a resistor"
  extends Modelica.Icons.Example;
  parameter Modelica.SIunits.Temperature TAmb(displayUnit = "degC") = 293.15 "Ambient Temperature";
  parameter Modelica.SIunits.TemperatureDifference TDif = 2 "Error in Temperature";
  output Modelica.SIunits.Temperature TRes(displayUnit = "degC") = heatingResistor.T_heatPort "Resulting Temperature";
  Modelica.Electrical.Analog.Basic.HeatingResistor heatingResistor(R_ref = 10, T_ref = 293.15, alpha = 1 / 255) annotation(Placement(transformation(origin = {-30, -50}, extent = {{-10, 10}, {10, -10}}, rotation = 270)));
  Modelica.Electrical.Analog.Ideal.IdealOpeningSwitch idealSwitch annotation(Placement(transformation(extent = {{-70, -50}, {-50, -30}}, rotation = 0)));
  Modelica.Thermal.HeatTransfer.Celsius.TemperatureSensor temperaturesensor1 annotation(Placement(visible = true, transformation(origin = {20, -40}, extent = {{-10, -10}, {10, 10}}, rotation = 90)));
  Modelica.Thermal.HeatTransfer.Components.HeatCapacitor heatcapacitor1(C = 1) annotation(Placement(visible = true, transformation(origin = {20, -80}, extent = {{-10, -10}, {10, 10}}, rotation = 180)));
  Modelica.Thermal.HeatTransfer.Components.ThermalConductor thermalconductor1(G = 0.1) annotation(Placement(visible = true, transformation(origin = {56, -50}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
  Modelica.Thermal.HeatTransfer.Sources.FixedTemperature fixedtemperature2(T = TAmb) annotation(Placement(visible = true, transformation(origin = {88, -50}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
  Modelica.Electrical.Analog.Sources.ConstantVoltage constantVoltage(V = 10) annotation(Placement(visible = true, transformation(origin = {-88, -50}, extent = {{-10, -10}, {10, 10}}, rotation = 270)));
  Modelica.Electrical.Analog.Basic.Ground ground annotation(Placement(visible = true, transformation(origin = {-88, -80}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
  Modelica.Blocks.Logical.OnOffController onOffController(bandwidth = TDif) annotation(Placement(visible = true, transformation(origin = {20, 20}, extent = {{10, -10}, {-10, 10}}, rotation = 0)));
  Modelica.Blocks.Logical.Not logicalNot annotation(Placement(visible = true, transformation(origin = {-20, 20}, extent = {{10, -10}, {-10, 10}}, rotation = 0)));
  Modelica.Blocks.Logical.Or or1 annotation(Placement(visible = true, transformation(origin = {-60, -20}, extent = {{-5, -5}, {5, 5}}, rotation = -90)));
  Modelica.Blocks.Logical.And and1 annotation(Placement(visible = true, transformation(origin = {-44, -2}, extent = {{-6, -6}, {6, 6}}, rotation = -90)));
  Modelica.Blocks.Sources.Ramp ramp(height = 25, duration = 6, offset = 25, startTime = 2) annotation(Placement(visible = true, transformation(origin = {80, 60}, extent = {{10, -10}, {-10, 10}}, rotation = 0)));
  Modelica.Blocks.Sources.BooleanConstant select1(k = false) annotation(Placement(visible = true, transformation(origin = {-80, 0}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
  Modelica.Blocks.Sources.BooleanConstant select2(k = true) annotation(Placement(visible = true, transformation(origin = {-80, 40}, extent = {{-10, -10}, {10, 10}}, rotation = 0)));
equation
  connect(select2.y, and1.u2) annotation(Line(points = {{-69, 40}, {-48.8714, 40}, {-48.8714, 5.69185}, {-48.8714, 5.69185}}));
  connect(select1.y, or1.u2) annotation(Line(points = {{-69, 0}, {-64.1806, 0}, {-64.1806, -13.9352}, {-64.1806, -13.9352}}));
  connect(and1.y, or1.u1) annotation(Line(points = {{-44, -8.6}, {-44, -10.4024}, {-60.2552, -10.4024}, {-60.2552, -13.9352}, {-60.2552, -13.9352}}));
  connect(or1.y, idealSwitch.control) annotation(Line(points = {{-60, -25.5}, {-60, -32.8535}, {-60.2879, -32.8535}, {-60.2879, -32.8535}}));
  connect(temperaturesensor1.T, onOffController.u) annotation(Line(points = {{20, -30}, {20, -1.86283}, {55.2075, -1.86283}, {55.2075, 13.7172}, {32.1761, 13.7172}, {32.1761, 13.7172}}));
  connect(onOffController.reference, ramp.y) annotation(Line(points = {{32, 26}, {54.6994, 26}, {54.6994, 59.9492}, {69.2633, 59.9492}, {69.2633, 59.9492}}));
  connect(logicalNot.y, and1.u1) annotation(Line(points = {{-31, 20}, {-44.1998, 20}, {-44.1998, 5.58848}, {-44.1998, 5.58848}}));
  connect(onOffController.y, logicalNot.u) annotation(Line(points = {{9, 20}, {-8, 20}}, color = {255, 0, 255}));
  connect(constantVoltage.n, ground.p) annotation(Line(points = {{-88, -60}, {-88, -70}}, color = {0, 0, 255}));
  connect(constantVoltage.p, idealSwitch.p) annotation(Line(points = {{-88, -40}, {-70, -40}}, color = {0, 0, 255}));
  connect(constantVoltage.n, heatingResistor.n) annotation(Line(points = {{-88, -60}, {-30, -60}}, color = {0, 0, 255}));
  connect(fixedtemperature2.port, thermalconductor1.port_b) annotation(Line(points = {{78, -50}, {65.71939999999999, -50}, {65.71939999999999, -50.444}, {65.71939999999999, -50.444}}));
  connect(heatingResistor.heatPort, temperaturesensor1.port) annotation(Line(points = {{-20, -50}, {-20, -49.812}, {19.4697, -49.812}, {19.4697, -49.812}}));
  connect(thermalconductor1.port_a, temperaturesensor1.port) annotation(Line(points = {{46, -50}, {20.4811, -50}, {20.4811, -49.5591}, {20.4811, -49.5591}}));
  connect(heatcapacitor1.port, temperaturesensor1.port) annotation(Line(points = {{20, -70}, {19.9754, -70}, {19.9754, -50}, {20, -50}}));
  connect(idealSwitch.n, heatingResistor.p) annotation(Line(points = {{-50, -40}, {-30, -40}}, color = {0, 0, 255}));
  annotation(Documentation(info = "<HTML>
 <P>
 A constant voltage of 10 V is applied to a
 temperature dependent resistor of 10*(1+(T-20C)/(235+20C)) Ohms,
 whose losses v**2/r are dissipated via a
 thermal conductance of 0.1 W/K to ambient temperature 20 degree C.
 The resistor is assumed to have a thermal capacity of 1 J/K,
 having ambient temperature at the beginning of the experiment.
 The temperature of this heating resistor is held by an OnOff-controller
 at reference temperature within a given bandwidth +/- 1 K
 by switching on and off the voltage source.
 The reference temperature starts at 25 degree C
 and rises between t = 2 and 8 seconds linear to 50 degree C.
 An appropriate simulating time would be 10 seconds.
 </P>
 </html>"), experiment(StartTime = 0, StopTime = 10, Tolerance = 0.0001));
end ControlledTemperature;