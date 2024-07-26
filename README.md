# Game Template (Windows)

A simple framework for prototyping games. \
The framework provides a video buffer and simple keyboard and mouse input.

Game is based on ECS pattern.
Each object is represented by Unique Identifier `EntityId` and list of `Components` attached to it. Components represented as Unique Type and registered in `Registry` as template parameter.
Iterating through compomenst is sequentially linear. Accessing to component by `EntityId` has constant complexity.

Requirements:
- MSVS
- C++17

- Use `GameTemplate.sln` to configure and MSVS to build and run project.
- `GeometryWars.exe` will be built.

TODO: Removing Components and Entities are not implemented. But Entities can be marked with `IsActive` component. This approach eliminates any memory allocation in the main loop.


![DEMO](https://github.com/ArcasHH/Geometry-Wars/blob/ecs/demo.gif)
