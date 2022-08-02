/**
 * Eva value
 */

#ifndef EVA_VALUE__H
#define EVA_VALUE__H

/**
 * Eva value type
 */
enum class EvaValueType {
  NUMBER,
};

/**
 * Eva value (tagged union)
 */
struct EvaValue {
  EvaValueType type;
  union {
    double number;
  };
};

// ----------------------------------------------------------------------
// Constructor

#define NUMBER(value)                                                          \
  ((EvaValue){.type = EvaValueType::NUMBER, .number = value})

// ----------------------------------------------------------------------
// Accessors

#define AS_NUMBER(evaValue) ((double)(evaValue).number)

#endif
