#include "netgame.h"

static const char* skip_whitespace(const char* str) {
    while (*str && (*str == ' ' || *str == '\t' || *str == '\n' || *str == '\r'))
        str++;
    return str;
}

static char* parse_string(const char** str) {
    if (**str != '"') return NULL;
    (*str)++;

    const char* start = *str;
    while (**str && **str != '"') {
        if (**str == '\\') (*str)++;
        (*str)++;
    }

    if (**str != '"') return NULL;

    size_t length = *str - start;
    char* result = (char*)malloc(length + 1);
    if (!result) return NULL;

    strncpy(result, start, length);
    result[length] = '\0';
    (*str)++;

    return result;
}

static json_value_t* parse_value(const char** str);

static json_value_t* parse_array(const char** str) {
    if (**str != '[') return NULL;
    (*str)++;

    json_value_t* array = (json_value_t*)malloc(sizeof(json_value_t));
    if (!array) return NULL;

    array->type = JSON_ARRAY;
    array->data.array.values = NULL;
    array->data.array.count = 0;

    *str = skip_whitespace(*str);
    if (**str == ']') {
        (*str)++;
        return array;
    }

    while (1) {
        json_value_t* value = parse_value(str);
        if (!value) {
            json_free(array);
            return NULL;
        }

        array->data.array.count++;
        array->data.array.values = realloc(array->data.array.values,
            array->data.array.count * sizeof(json_value_t));
        array->data.array.values[array->data.array.count - 1] = *value;
        free(value);

        *str = skip_whitespace(*str);
        if (**str == ']') {
            (*str)++;
            break;
        }
        if (**str != ',') {
            json_free(array);
            return NULL;
        }
        (*str)++;
        *str = skip_whitespace(*str);
    }

    return array;
}

static json_value_t* parse_object(const char** str) {
    if (**str != '{') return NULL;
    (*str)++;

    json_value_t* object = (json_value_t*)malloc(sizeof(json_value_t));
    if (!object) return NULL;

    object->type = JSON_OBJECT;
    object->data.object.keys = NULL;
    object->data.object.values = NULL;
    object->data.object.count = 0;

    *str = skip_whitespace(*str);
    if (**str == '}') {
        (*str)++;
        return object;
    }

    while (1) {
        *str = skip_whitespace(*str);
        char* key = parse_string(str);
        if (!key) {
            json_free(object);
            return NULL;
        }

        *str = skip_whitespace(*str);
        if (**str != ':') {
            free(key);
            json_free(object);
            return NULL;
        }
        (*str)++;

        json_value_t* value = parse_value(str);
        if (!value) {
            free(key);
            json_free(object);
            return NULL;
        }

        object->data.object.count++;
        object->data.object.keys = realloc(object->data.object.keys,
            object->data.object.count * sizeof(char*));
        object->data.object.values = realloc(object->data.object.values,
            object->data.object.count * sizeof(json_value_t));

        object->data.object.keys[object->data.object.count - 1] = key;
        object->data.object.values[object->data.object.count - 1] = *value;
        free(value);

        *str = skip_whitespace(*str);
        if (**str == '}') {
            (*str)++;
            break;
        }
        if (**str != ',') {
            json_free(object);
            return NULL;
        }
        (*str)++;
    }

    return object;
}

static json_value_t* parse_value(const char** str) {
    *str = skip_whitespace(*str);

    json_value_t* value = (json_value_t*)malloc(sizeof(json_value_t));
    if (!value) return NULL;

    if (**str == '"') {
        value->type = JSON_STRING;
        value->data.string = parse_string(str);
        if (!value->data.string) {
            free(value);
            return NULL;
        }
    } else if (**str == '[') {
        json_value_t* array = parse_array(str);
        if (!array) {
            free(value);
            return NULL;
        }
        *value = *array;
        free(array);
    } else if (**str == '{') {
        json_value_t* object = parse_object(str);
        if (!object) {
            free(value);
            return NULL;
        }
        *value = *object;
        free(object);
    } else if (strncmp(*str, "null", 4) == 0) {
        value->type = JSON_NULL;
        *str += 4;
    } else if (strncmp(*str, "true", 4) == 0) {
        value->type = JSON_BOOL;
        value->data.boolean = 1;
        *str += 4;
    } else if (strncmp(*str, "false", 5) == 0) {
        value->type = JSON_BOOL;
        value->data.boolean = 0;
        *str += 5;
    } else {
        value->type = JSON_NUMBER;
        char* end;
        value->data.number = strtod(*str, &end);
        if (end == *str) {
            free(value);
            return NULL;
        }
        *str = end;
    }

    return value;
}

json_value_t* json_parse(const char* json_str) {
    return parse_value(&json_str);
}

void json_free(json_value_t* value) {
    if (!value) return;

    switch (value->type) {
        case JSON_STRING:
            free(value->data.string);
            break;
        case JSON_ARRAY:
            for (int i = 0; i < value->data.array.count; i++) {
                json_free(&value->data.array.values[i]);
            }
            free(value->data.array.values);
            break;
        case JSON_OBJECT:
            for (int i = 0; i < value->data.object.count; i++) {
                free(value->data.object.keys[i]);
                json_free(&value->data.object.values[i]);
            }
            free(value->data.object.keys);
            free(value->data.object.values);
            break;
        default:
            break;
    }
}
