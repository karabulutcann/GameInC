#include "camera.h"
#include "world/world.h"
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
            .position = {2.0f, 50.0f, 2.0f}, 
            .front = {-8.0f, 0.0f, -1.0f},
            .up = {0.0f, 1.0f, 0.0f},
            .yaw = 90.0f,
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


RaycastHit RaycastBlock(vec3 origin, vec3 direction) {
    float maxDistance = 20.0f;
    IVec3 mapCheck = { (i4)floor(origin[0] / (cubeSize)), (i4)floor(origin[1] / (cubeSize)), (i4)floor(origin[2] / (cubeSize)) };
    IVec3 step = {direction[0] > 0 ? 1 : -1, direction[1] > 0 ? 1 : -1, direction[2] > 0 ? 1 : -1};

    vec3 delta = {
        fabsf(1.0f / direction[0]),
        fabsf(1.0f / direction[1]),
        fabsf(1.0f / direction[2])
    };

    Vec3 rayLength1D = {
        (step.x > 0 ? (ceilf(origin[0]) - origin[0]) : (origin[0] - floorf(origin[0]))) / fabsf(direction[0]),
        (step.y > 0 ? (ceilf(origin[1]) - origin[1]) : (origin[1] - floorf(origin[1]))) / fabsf(direction[1]),
        (step.z > 0 ? (ceilf(origin[2]) - origin[2]) : (origin[2] - floorf(origin[2]))) / fabsf(direction[2])
    };

    float radius = maxDistance / glm_vec3_norm(direction);

    Bool isInRadius = TRUE;

    while (isInRadius) {
        // Move along the shortest path
        if (rayLength1D.x < rayLength1D.y && rayLength1D.x < rayLength1D.z && rayLength1D.x < radius) {
            mapCheck.x += step.x;
            rayLength1D.x += delta[0];
        } else if (rayLength1D.y < rayLength1D.z && rayLength1D.y < radius) {
            mapCheck.y += step.y;
            rayLength1D.y += delta[1];
        } else if(rayLength1D.z < radius) {
            mapCheck.z += step.z;
            rayLength1D.z += delta[2];
        }else{
            isInRadius = FALSE;
        }

        // Get block at position
        int index = (mapCheck.y * CHUNK_SIZE_X * CHUNK_SIZE_Z) + (mapCheck.z * CHUNK_SIZE_X) + mapCheck.x;

        if (staticWorldGetBlock((i4[3]){mapCheck.x,mapCheck.y,mapCheck.z}) != 0) {
            // Determine which face was hit
            enum Face hitFace = -1; // Default face is none
            IVec3 adjacentPos = {mapCheck.x, mapCheck.y, mapCheck.z};
            if (rayLength1D.x < rayLength1D.y && rayLength1D.x < rayLength1D.z) {
                // X-axis face
                hitFace = (step.x > 0) ? RIGHT : LEFT;
                if(step.x > 0){
                    adjacentPos.x += 1;
                }else{
                    adjacentPos.x -= 1;
                }
            } else if (rayLength1D.y < rayLength1D.z) {
                // Y-axis face
                hitFace = (step.y > 0) ? TOP : BOTTOM;
                if(step.y > 0){
                    adjacentPos.y += 1;
                }else{
                    adjacentPos.y -= 1;
                }
            } else {
                // Z-axis face
                hitFace = (step.z > 0) ? FRONT : BACK;
                if(step.z > 0){
                    adjacentPos.z += 1;
                }else{
                    adjacentPos.z -= 1;
                }
            } 
            return (RaycastHit){
                .hit = true,
                .blockPos = mapCheck,
                .index = index,
                .adjacentPos = adjacentPos,
            };
        }

    }

    return (RaycastHit){ .hit = FALSE };
}


