# Robot Separador de Barquillos

Este proyecto consistió de realizar un clasificador de barquillos por medio de un manipulador rotacional. Para ello, la clasificación se realizaría al recoger un barquillos, y depositarlo en el agujero correspondiente, el cual tendría imanes para su detección. Este sensado se debía realizar con un sensor Hall.

Asi, se diseñó un reductor con un sistema planetario de engranajes para el motor Stepper, el cual se encarga de toda la carga del robot al ser la articulación rotacional en la base del mismo, asimismo, se diseñó un sistema piñón-cremallera para el efector final (garra), el cual se controla con un motor Servo. Además de esots disños, se hizo el modelado en 3D CAD utilizando SolidWorks para la impresión en 3D de todas las piezas del robot.

Se utilizó un driver para el motor Stepper, y se debió calibrar el sensor Hall y aplicar un filtro de promedio al mismo para eliminar lecturas falsas.

Finalmente, se utilizó un Arduino UNO para la coordinación de los actuadores y el sensor Hall.

En el repositorio se encuentra el código final de Arduino, funcional, presentado en el proyecto, asi como los modelos CAD de SolidWorks y el artículo de diseño que se entregó, el cual incluye planos normados.
