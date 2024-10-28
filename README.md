# ci4321-proyecto-1
Primer Proyecto de Computación Gráfica I (CI4321). Universidad Simón Bolívar, trimestre Sep-Dic 2024

# Integrantes
- Santiago Finamore, 18-10125
- Pedro Rodríguez, 15-11264

# API utilizada
- OpenGL 4.2

# OS utilizado
- Fedora Linux
- WSL Ubuntu

# Requisitos
- CMake 3.2
- OpenGL 4.2
- GLFW3
  - Paquetes Ubuntu: libglfw3, libglfw3-dev
  - Paquetes Fedora: glfw.x86_64, glfw.i686, glfw_devel.x86_64, glfw_devel.i686
- Nvidia RTX 4090 (jk)

# Instrucciones de build
1. Ubicarse en una carpeta a donde colocal el binario final
2. Utilizar el comando ```cmake <path_CMakeLists.txt_raiz_proyecto>```
3. Buildear el proyecto con el comando ```cmake --build .```
4. Correr el ejecutable ```proyecto1.o```

# Instrucciones de juego
- W: Mover vehículo hacia delante
- S: Movel vehículo en reversa
- D: Girar vehículo en sentido horario
- A: Girar vehículo en sentido anti horario
- I: Subir cañón
- K: Bajar cañón
- L: Girar torreta en sentido horario
- J: Girar torreta en sentido anti horario
- Space: Disparar proyectil con gravedad
- Left CTRL: Disparar proyectir rectilineo
