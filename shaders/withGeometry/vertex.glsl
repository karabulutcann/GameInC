#version 330 core
layout (location = 0) in uint aChunkData;

out uint chunkData;

void main()
{
    chunkData = aChunkData;
    int blockX = gl_InstanceID % 16;
    int blockY = gl_InstanceID / 16 % 16;
    int blockZ = gl_InstanceID / 256;

    float blockSize = 1.0;

    gl_Position = vec4(blockSize * blockX , blockSize *  blockY, blockSize *  blockZ, 1.0);
}