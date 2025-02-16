#pragma once
#include "cglm/cglm.h"
#include "core/result.h"

struct Camera {
    vec3 position;
    vec3 front;
    vec3 up;
    vec3 right;
    vec3 worldUp;

    float yaw;
    float pitch;

    float movementSpeed;
    float mouseSensitivity;

    // float zoom;
};

struct CameraCreateOptions {
    vec3 position;
    vec3 front;
    vec3 up;
    float yaw;
    float pitch;
    float movementSpeed;
    float mouseSensitivity;
};

struct Result _cameraCreateDefault(struct Camera* dest);

struct Result _cameraCreate(struct CameraCreateOptions options, struct Camera* dest);

#define mCameraCreate(...) GET_MACRO2(__VA_ARGS__,_cameraCreate,_cameraCreateDefault)(__VA_ARGS__)

struct Result cameraLookAt(struct Camera src, mat4 dest);

struct Result cameraUpdateDirection(struct Camera* camera, float xoffset, float yoffset);

