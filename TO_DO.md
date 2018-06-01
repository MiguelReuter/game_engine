# Refactor

+ Classes Body, DynamicBody... : remove diamond inheritance. Set Sprite class for each object and add a property "physics" which includes its physics behaviour
+ Limit constructor parameters count to 6 (?)
+ Game_Types : set default parameters (friction_coefficient, life_time, mass, width, height, air_friction_coefficient, max_vel_x, max_vel_y, ghost_* ...)


# Fix

+ RiggedSprite does not work properly


# Features

+ Particles generator
+ Add circle collision
+ Finite State Machine (for Character class...)
+ Sound engine
