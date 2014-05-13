encapsulated model SineSource
  import Modelica.Math.sin;
  connector OutPort = output Real;
  OutPort y = sin(time);
  // Uses the built-in variable time.
end SineSource;

