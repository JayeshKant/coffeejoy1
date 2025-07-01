#pragma once

enum class buttons {
    start,
    abort,
    maintenance,
    decalcification,
    crema,
    espresso,
    cappuccino,
    light,
    medium,
    strong,
    shutdown,
    undefined
};

struct Buttons {
    int beginX;
    int endX;
    int beginY;
    int endY;
    buttons name;
};
