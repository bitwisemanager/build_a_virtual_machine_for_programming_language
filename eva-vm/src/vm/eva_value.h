/**
 * Eva value
 */

#ifndef EVA_VALUE__H
#define EVA_VALUE__H

#include <string>
#include <vector>

#include "logger.h"

/**
 * Eva value type
 */
enum class EvaValueType {
  NUMBER,
  BOOLEAN,
  OBJECT,
};

/**
 * Object type
 */
enum class ObjectType {
  STRING,
  CODE,
};

/**
 * Base object
 */
struct Object {
  Object(ObjectType type) : type(type) {}
  ObjectType type;
};

struct StringObject : public Object {
  StringObject(const std::string &str)
      : Object(ObjectType::STRING), string(str) {}
  std::string string;
};

/**
 * Eva value (tagged union)
 */
struct EvaValue {
  EvaValueType type;
  union {
    double number;
    bool boolean;
    Object *object;
  };
};

/**
 * Code object
 */
struct CodeObject : public Object {
  CodeObject(const std::string &name) : Object(ObjectType::CODE), name(name) {}

  /**
   * Name of the unit (usually function name)
   */
  std::string name;

  /**
   * Constant pool
   */
  std::vector<EvaValue> constants;

  /**
   * Bytecode
   */
  std::vector<uint8_t> code;
};

// ----------------------------------------------------------------------
// Constructor

#define NUMBER(value)                                                          \
  ((EvaValue){.type = EvaValueType::NUMBER, .number = value})

#define BOOLEAN(value)                                                         \
  ((EvaValue){.type = EvaValueType::BOOLEAN, .boolean = value})

#define ALLOC_STRING(value)                                                    \
  ((EvaValue){.type = EvaValueType::OBJECT,                                    \
              .object = (Object *)new StringObject(value)})

#define ALLOC_CODE(name)                                                       \
  ((EvaValue){.type = EvaValueType::OBJECT,                                    \
              .object = (Object *)new CodeObject(name)})

// ----------------------------------------------------------------------
// Accessors

#define AS_NUMBER(evaValue) ((double)(evaValue).number)
#define AS_BOOLEAN(evaValue) ((bool)(evaValue).boolean)
#define AS_OBJECT(evaValue) ((Object *)(evaValue).object)
#define AS_CODE(evaValue) ((CodeObject *)(evaValue).object)

#define AS_STRING(evaValue) (((StringObject *)evaValue.object))
#define AS_CPPSTRING(evaValue) (AS_STRING(evaValue)->string)

// ----------------------------------------------------------------------
// Testers:

#define IS_NUMBER(evaValue) (evaValue.type == EvaValueType::NUMBER)
#define IS_BOOLEAN(evaValue) (evaValue.type == EvaValueType::BOOLEAN)
#define IS_OBJECT(evaValue) (evaValue.type == EvaValueType::OBJECT)

#define IS_OBJECT_TYPE(evaValue, objectType)                                   \
  (IS_OBJECT(evaValue) && AS_OBJECT(evaValue)->type == objectType)

#define IS_STRING(evaValue) IS_OBJECT_TYPE(evaValue, ObjectType::STRING)
#define IS_CODE(evaValue) IS_OBJECT_TYPE(evaValue, ObjectType::CODE)

/**
 * String representation used in constants for debug
 */
std::string evaValueToTypeString(const EvaValue &evaValue) {
  if (IS_NUMBER(evaValue))
    return "NUMBER";

  if (IS_BOOLEAN(evaValue))
    return "BOOLEAN";

  if (IS_STRING(evaValue))
    return "STRING";

  if (IS_CODE(evaValue))
    return "CODE";

  DIE << "evaValueToTypeString: unknown type " << (int)evaValue.type;

  return ""; // Unrechable
}

/**
 * String representation used in constants for debug
 */
std::string evaValueToConstantString(const EvaValue &evaValue) {
  std::stringstream ss;
  if (IS_NUMBER(evaValue)) {
    ss << AS_NUMBER(evaValue);
  } else if (IS_BOOLEAN(evaValue)) {
    ss << (evaValue.boolean == true ? "true" : "false");
  } else if (IS_STRING(evaValue)) {
    ss << '"' << AS_CPPSTRING(evaValue) << '"';
  } else if (IS_CODE(evaValue)) {
    auto code = AS_CODE(evaValue);
    ss << "code" << code << ":" << code->name;
  } else {
    DIE << "evaValueToConstantString: unknown type " << (int)evaValue.type;
  }

  return ss.str();
}

/**
 * Output stream
 */
std::ostream &operator<<(std::ostream &os, const EvaValue &evaValue) {
  return os << "EvaValue(" << evaValueToTypeString(evaValue)
            << "): " << evaValueToConstantString(evaValue);
}

#endif
