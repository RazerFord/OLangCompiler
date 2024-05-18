#include <stdlib.h>

struct Boolean {
    int* vtable;
    char value;
};

struct  Integer {
    int* vtable;
    int value;
};

struct Real {
    int* vtable;
    float value;
};

// class Integer
struct Integer* UnaryMinusI(struct Integer* a) {
    a->value = -a->value;
    return a;
}

struct Real* toRealI(struct Integer* a) {
    struct Real* res = malloc(sizeof(struct Real));
    res->value = (float) a->value;
    return res;
}

struct Integer* PlusII(struct Integer* a, struct Integer* other) {
    struct Integer* res = malloc(sizeof(struct Integer));
    res->value = a->value + other->value;
    return res;
}

struct Real* PlusIR(struct Integer* a, struct Real* other) {
    struct Real* res = malloc(sizeof(struct Real));
    res->value = (float) a->value + other->value;
    return res;
}

struct Integer* MinusII(struct Integer* a, struct Integer* other) {
    struct Integer* res = malloc(sizeof(struct Integer));
    res->value = a->value - other->value;
    return res;
}

struct Real* MinusIR(struct Integer* a, struct Real* other) {
    struct Real* res = malloc(sizeof(struct Real));
    res->value = (float) a->value - other->value;
    return res;
}

struct Integer* MultII(struct Integer* a, struct Integer* other) {
    struct Integer* res = malloc(sizeof(struct Integer));
    res->value = a->value * other->value;
    return res;
}

struct Real* MultIR(struct Integer* a, struct Real* other) {
    struct Real* res = malloc(sizeof(struct Real));
    res->value = (float) a->value * other->value;
    return res;
}

struct Integer* DivII(struct Integer* a, struct Integer* other) {
    struct Integer* res = malloc(sizeof(struct Integer));
    res->value = a->value / other->value;
    return res;
}

struct Real* DivIR(struct Integer* a, struct Real* other) {
    struct Real* res = malloc(sizeof(struct Real));
    res->value = (float) a->value / other->value;
    return res;
}

struct Boolean* LessII(struct Integer* a, struct Integer* other) {
    struct Boolean* res = malloc(sizeof(struct Boolean));
    res->value = (char) (a->value < other->value);
    return res;
}

struct Boolean* LessIR(struct Integer* a, struct Real* other) {
    struct Boolean* res = malloc(sizeof(struct Boolean));
    res->value = (char) ((float) a->value < other->value);
    return res;
}

struct Boolean* LessEqII(struct Integer* a, struct Integer* other) {
    struct Boolean* res = malloc(sizeof(struct Boolean));
    res->value = (char) (a->value <= other->value);
    return res;
}

struct Boolean* LessEqIR(struct Integer* a, struct Real* other) {
    struct Boolean* res = malloc(sizeof(struct Boolean));
    res->value = (char) ((float) a->value <= other->value);
    return res;
}

struct Boolean* GreaterII(struct Integer* a, struct Integer* other) {
    struct Boolean* res = malloc(sizeof(struct Boolean));
    res->value = (char) (a->value > other->value);
    return res;
}

struct Boolean* GreaterIR(struct Integer* a, struct Real* other) {
    struct Boolean* res = malloc(sizeof(struct Boolean));
    res->value = (char) ((float) a->value > other->value);
    return res;
}

struct Boolean* GreaterEqII(struct Integer* a, struct Integer* other) {
    struct Boolean* res = malloc(sizeof(struct Boolean));
    res->value = (char) (a->value >= other->value);
    return res;
}

struct Boolean* GreaterEqIR(struct Integer* a, struct Real* other) {
    struct Boolean* res = malloc(sizeof(struct Boolean));
    res->value = (char) ((float) a->value >= other->value);
    return res;
}


struct Boolean* EqII(struct Integer* a, struct Integer* other) {
    struct Boolean* res = malloc(sizeof(struct Boolean));
    res->value = (char) (a->value == other->value);
    return res;
}

struct Boolean* EqIR(struct Integer* a, struct Real* other) {
    struct Boolean* res = malloc(sizeof(struct Boolean));
    res->value = (char) ((float) a->value == other->value);
    return res;
}


// class Real

struct Real* UnaryMinusR(struct Real* a) {
    a->value = -a->value;
    return a;
}

struct Integer* toIntegerR(struct Real* a) {
    struct Integer* res = malloc(sizeof(struct Integer));
    res->value = (int) a->value;
    return res;
}

struct Real* PlusRR(struct Real* a, struct Real* other) {
    struct Real* res = malloc(sizeof(struct Real));
    res->value = a->value + other->value;
    return res;
}

struct Real* PlusRI(struct Real* a, struct Integer* other) {
    struct Real* res = malloc(sizeof(struct Real));
    res->value =  a->value + (float) other->value;
    return res;
}

struct Real* MinusRR(struct Real* a, struct Real* other) {
    struct Real* res = malloc(sizeof(struct Real));
    res->value = a->value - other->value;
    return res;
}

struct Real* MinusRI(struct Real* a, struct Integer* other) {
    struct Real* res = malloc(sizeof(struct Real));
    res->value =  a->value - (float) other->value;
    return res;
}

struct Real* MultRR(struct Real* a, struct Real* other) {
    struct Real* res = malloc(sizeof(struct Real));
    res->value = a->value * other->value;
    return res;
}

struct Real* MultRI(struct Real* a, struct Integer* other) {
    struct Real* res = malloc(sizeof(struct Real));
    res->value =  a->value * (float) other->value;
    return res;
}

struct Real* DivRR(struct Real* a, struct Real* other) {
    struct Real* res = malloc(sizeof(struct Real));
    res->value = a->value / other->value;
    return res;
}

struct Real* DivRI(struct Real* a, struct Integer* other) {
    struct Real* res = malloc(sizeof(struct Real));
    res->value =  a->value / (float) other->value;
    return res;
}

struct Boolean* LessRR(struct Real* a, struct Real* other) {
    struct Boolean* res = malloc(sizeof(struct Boolean));
    res->value = (char) (a->value < other->value);
    return res;
}

struct Boolean* LessRI(struct Real* a, struct Integer* other) {
    struct Boolean* res = malloc(sizeof(struct Boolean));
    res->value = (char) (a->value < (float) other->value);
    return res;
}

struct Boolean* LessEqRR(struct Real* a, struct Real* other) {
    struct Boolean* res = malloc(sizeof(struct Boolean));
    res->value = (char) (a->value <= other->value);
    return res;
}

struct Boolean* LessEqRI(struct Real* a, struct Integer* other) {
    struct Boolean* res = malloc(sizeof(struct Boolean));
    res->value = (char) (a->value <= (float) other->value);
    return res;
}

struct Boolean* GreaterRR(struct Real* a, struct Real* other) {
    struct Boolean* res = malloc(sizeof(struct Boolean));
    res->value = (char) (a->value > other->value);
    return res;
}

struct Boolean* GreaterRI(struct Real* a, struct Integer* other) {
    struct Boolean* res = malloc(sizeof(struct Boolean));
    res->value = (char) (a->value > (float) other->value);
    return res;
}

struct Boolean* GreaterEqRR(struct Real* a, struct Real* other) {
    struct Boolean* res = malloc(sizeof(struct Boolean));
    res->value = (char) (a->value >= other->value);
    return res;
}

struct Boolean* GreaterEqRI(struct Real* a, struct Integer* other) {
    struct Boolean* res = malloc(sizeof(struct Boolean));
    res->value = (char) (a->value >= (float) other->value);
    return res;
}

struct Boolean* EqRR(struct Real* a, struct Real* other) {
    struct Boolean* res = malloc(sizeof(struct Boolean));
    res->value = (char) (a->value == other->value);
    return res;
}

struct Boolean* EqRI(struct Real* a, struct Integer* other) {
    struct Boolean* res = malloc(sizeof(struct Boolean));
    res->value = (char) (a->value == (float) other->value);
    return res;
}


//class Boolean

struct Integer* toIntegerB(struct Boolean* b) {
    struct Integer* res = malloc(sizeof(struct Integer));
    res->value = (int) b->value;
    return res;
}

struct Boolean* OrB(struct Boolean* a, struct Boolean* b) {
    struct Boolean* res = malloc(sizeof(struct Boolean));
    res->value = (char) (a->value || b->value);
    return res;
}

struct Boolean* AndB(struct Boolean* a, struct Boolean* b) {
    struct Boolean* res = malloc(sizeof(struct Boolean));
    res->value = (char) (a->value && b->value);
    return res;
}

struct Boolean* XorB(struct Boolean* a, struct Boolean* b) {
    struct Boolean* res = malloc(sizeof(struct Boolean));
    res->value = (char) (a->value ^ b->value);
    return res;
}

struct Boolean* NotB(struct Boolean* a) {
    a->value = (char) !a->value;
    return a;
}
