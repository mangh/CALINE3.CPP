# C++ implementation of the [CALINE3 Model Algorithm](https://www.epa.gov/scram/air-quality-dispersion-modeling-alternative-models#caline3)

* Unlike the original, the algorithm applies units of measurement to the physical quantities used in the source code: each variable representing the physical quantity is assigned to the appropriate unit of measure, explicitly specified as a type of that variable. This allows [dimensional analysis](https://en.wikipedia.org/wiki/Dimensional_analysis) (the correctness of relationships between different physical quantities) to be carried out as part of syntax check during source code compilation.

* A C++ units of measurement library - suitable for CALINE3 applications - is provided by the [Metrology](https://github.com/mangh/CALINE3.CPP/tree/main/Metrology) project.
