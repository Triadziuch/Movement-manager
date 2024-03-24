# Movement manager
The MovementManager repository is a C++ project focused on facilitating seamless movement of SFML objects on-screen by implementing ease functions. It provides a comprehensive solution for managing motion dynamics within SFML-based applications.

# Project description
The MovementManager project manages the motion, scaling, and rotation of objects using routines. Each routine consists of an unlimited number of transformations (i.e., movement, scaling, and rotation instructions), where each transformation can utilize a different ease function and has its own duration. Additionally, transformations feature individual delays before and after completion of the motion.

MovementManager allows for creation of a single routine linked to multiple different objects. It utilizes highly efficient data structures for motion management, allowing MovementManager to effortlessly handle transformations for hundreds of thousands of objects simultaneously.

# Demo functionalities
The interactive demo of the project is available for download from the Releases tab. It includes four distinct interactive demonstrations: three showcasing the capabilities of MovementManager and one of a plot drawing class. Additionally, each demo incorporates the SideBar class, that utilizes MovementManager for seamless animation.

Movement Demo 1 is an interactive demonstration showcasing the basic capabilities of the MovementManager class. Simultaneously on the screen, three different objects move, scale, and rotate utilizing various ease functions - IN, OUT, and IN_OUT. The demo allows for changing the motion function to Sine, Quad, Cubic, Quart, Quint, Expo, Circ, Back, Elastic, or Bounce. Additionally, the duration of transformations can be dynamically adjusted without affecting the position, scale, or rotation of the objects. On the right side of the screen, there are plots displaying the currently used function. Moreover, the demo enables pausing any type of transformation dynamically.

![Movement 1](https://github.com/Triadziuch/Movement-manager/assets/75269577/53aaf46a-b63e-4035-ba67-ba47e83fcc5e)

Movement Demo 2 showcases the ability to adjust the current object transformation to the instructions contained within the routine. The first option, adjustStartToCurrentTransform, adjusts the position, scaling, or rotation of the first transformation in the routine to match the current object transformation. The second option, adjustAllToCurrentTransform, adjusts all transformations to match the current object transformation. The demo allows testing these functionalities and additionally presents a custom class, VertexArray2, tailored for motion and inheriting from the sf::VertexArray and sf::Transformable classes.

Movement Demo 3 demonstrates how MovementManager handles movement of tens of thousands of objects simultaneously. This demo allows for changing the number of objects, their sizes, and the motion functions they use. Each object has a unique routine, and each routine consists of 10 unique motion instructions. Additionally, the demo allows to turn on synchronization of all object movements, resulting in impressive. harmonized motions.

![Movement 2](https://github.com/Triadziuch/Movement-manager/assets/75269577/772ac88d-4d45-4ade-a3f8-4b975a8dd9b2)

# Examples of Implementation
MovementManager was used to create fluid animations for a clone of the game 2048. The repository showcasing the result of this implementation can be found [here](https://github.com/Triadziuch/2048).

## License
Copyright © 2024 by Triadziuch

This project is licensed under the MIT License - see the LICENSE file for details.

## Third-party Components
This project utilizes the SFML library, which is licensed under the zlib/png license.
