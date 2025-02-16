#include "camera.h"

struct Result _cameraCreate(struct CameraCreateOptions options, struct Camera* dest){
    if (!dest) return mErr("Destination cant be null");

    glm_vec3_copy(options.position, dest->position);
    glm_vec3_copy(options.front, dest->front);
    glm_vec3_copy(options.up, dest->up);
    dest->yaw = options.yaw;
    dest->pitch = options.pitch;
    dest->movementSpeed = options.movementSpeed;
    dest->mouseSensitivity = options.mouseSensitivity;

    return ok();
}

struct Result _cameraCreateDefault(struct Camera* dest){
    
    return _cameraCreate(
        (struct CameraCreateOptions){
            .position = {0.0f, 0.0f, 3.0f},
            .front = {0.0f, 0.0f, -1.0f},
            .up = {0.0f, 1.0f, 0.0f},
            .yaw = -90.0f,
            .pitch = 0.0f,
            .movementSpeed = 5.0f,
            .mouseSensitivity = 0.1f
        },
        dest
    );

}

struct Result cameraLookAt(struct Camera src, mat4 dest){
    if (!dest) return mErr("Destination cant be null");

    vec3 tmpCenter = {0.0f, 0.0f, 0.0f};
    glm_vec3_add(src.position, src.front, tmpCenter);
    glm_lookat(src.position, tmpCenter, src.up, dest);
    return ok();
}


struct Result cameraUpdateDirection(struct Camera* camera, float xoffset, float yoffset){
    if (!camera) return mErr("Camera cant be null");

    camera->yaw += xoffset * camera->mouseSensitivity;
    camera->pitch += yoffset * camera->mouseSensitivity;

    if (camera->pitch > 89.0f)
        camera->pitch = 89.0f;
    if (camera->pitch < -89.0f)
        camera->pitch = -89.0f;

    vec3 tmpDirection = {0.0f, 0.0f, 0.0f};
    tmpDirection[0] = cos(glm_rad(camera->yaw)) * cos(glm_rad(camera->pitch));
    tmpDirection[1] = sin(glm_rad(camera->pitch));
    tmpDirection[2] = sin(glm_rad(camera->yaw)) * cos(glm_rad(camera->pitch));
    glm_normalize(tmpDirection);
    glm_vec3_copy(tmpDirection, camera->front);
    glm_vec3_cross(camera->front, camera->worldUp, camera->right);
    glm_normalize(camera->right);
    //bu calismiyor mouse hareket edince her sey kayboluyor
    // glm_vec3_cross(camera->right, camera->front, camera->up);
    return ok();
}
