#include "camera.h"
#include <math.h>

static struct Camera* staticCamera;

struct Camera* cameraGet(){
    return staticCamera;
}

struct Result _cameraCreate(struct CameraCreateOptions options, struct Camera* dest){
    if (!dest) return mErr("Destination cant be null");

    glm_vec3_copy(options.position, dest->position);
    glm_vec3_copy(options.front, dest->front);
    glm_vec3_copy(options.up, dest->up);
    dest->yaw = options.yaw;
    dest->pitch = options.pitch;
    dest->movementSpeed = options.movementSpeed;
    dest->mouseSensitivity = options.mouseSensitivity;

    staticCamera = dest;

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


RaycastHit RaycastBlock(vec3 origin, vec3 direction,struct Chunk* chunk) {
    IVec3 mapCheck = { (i4)floor(origin[0] / cubeSize), (i4)floor(origin[1] / cubeSize), (i4)floor(origin[2] / cubeSize) };
   
    IVec3 step = {direction[0] > 0 ? 1 : -1, direction[1] > 0 ? 1 : -1, direction[2] > 0 ? 1 : -1};

    Vec3 rayUnitStepSize = {
        fabsf(1.0f / direction[0]),
        fabsf(1.0f / direction[1]),
        fabsf(1.0f / direction[2])
    };

    Vec3 rayLength1D = {
        (step.x > 0 ? (mapCheck.x + 1) * cubeSize - origin[0] : origin[0] - mapCheck.x * cubeSize) * rayUnitStepSize.x,
        (step.y > 0 ? (mapCheck.y + 1) * cubeSize - origin[1] : origin[1] - mapCheck.y * cubeSize) * rayUnitStepSize.y,
        (step.z > 0 ? (mapCheck.z + 1) * cubeSize - origin[2] : origin[2] - mapCheck.z * cubeSize) * rayUnitStepSize.z
    };

    // Raycasting loop
    float maxDistance = 8.0f; // Max block reach
    float distance = 0.0f;
    // IVec3 lastValidPos = mapCheck;

    while (distance < maxDistance) {
        // Move along the shortest path
        if (rayLength1D.x < rayLength1D.y && rayLength1D.x < rayLength1D.z) {
            distance = rayLength1D.x;
            mapCheck.x += step.x;
            rayLength1D.x += rayUnitStepSize.x;
        } else if (rayLength1D.y < rayLength1D.z) {
            distance = rayLength1D.y;
            mapCheck.y += step.y;
            rayLength1D.y += rayUnitStepSize.y;
        } else {
            distance = rayLength1D.z;
            mapCheck.z += step.z;
            rayLength1D.z += rayUnitStepSize.z;
        }

        // Ensure it's within chunk bounds
        if (mapCheck.x < 0 || mapCheck.x >= CHUNK_SIZE_X ||
            mapCheck.y < 0 || mapCheck.y >= CHUNK_SIZE_Y ||
            mapCheck.z < 0 || mapCheck.z >= CHUNK_SIZE_Z) {
            return (RaycastHit){ .hit = FALSE };
        }

        // Get block at position
        int index = (mapCheck.y * CHUNK_SIZE_X * CHUNK_SIZE_Z) + (mapCheck.z * CHUNK_SIZE_X) + mapCheck.x;
    
        if (chunk->blockTypeArr[index] != 0) {
            return (RaycastHit){
                .hit = true,
                .blockPos = mapCheck,
                .index = index,
            };
        }

    }

    return (RaycastHit){ .hit = FALSE };
}
