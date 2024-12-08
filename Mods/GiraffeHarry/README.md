Giraffe Harry is not so much a mod (maybe a troll mod), rather than a demonstration
of runtime modification of Harry's pose.

Skeletal animations in videogames use "bones" to deform parts of a model, giving the
illusion of animals / characters / objects moving and gesturing.
Each bone has an associated 4x4 matrix which describes its position and orientation
in the game world. This is what we are altering in this script.

For some reason, this game uses two matrix collections that are updated one after
another, similarly to double-buffering in real-time rendering.

Fun fact : This can also be used on NPCs, not just Harry. You just need to find their
matrix palette.
