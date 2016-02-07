/* Struct containing a fluid particle
Total memory size = 12 + 12 + 4 + 12 + 4 = 44 bytes */
typedef struct __attribute__ ((packed)) {
    float3 position;    // 12 bytes
    float3 velocity;    // 12 bytes

    float density;      // 4 bytes

    float3 force;       // 12 bytes
    float color_field;  // 4 bytes
} Particle;

typedef struct __attribute__ ((packed)) {
    uint particle_counter;
    Particle particles[4];
} VoxelCell;

 // the corner-most cell is voxel_grid_cells[x=0][y=0][z=0]
__kernel void populate_voxel_grid(__global VoxelCell* voxel_grid_cells,
                                 const float3 voxel_grid_origin_corner,
                                 const float voxel_grid_cell_dimensions,
                                 const int3 voxel_grid_cell_count,
                                 __global const float3* positions,
                                 __global const float3* velocities) {
    const int particle_id = get_global_id(0);

    // Retrieve the particle's position right away since it is needed in the voxel cell index calculations,
    // but wait with retrieving its velocity
    const float3 position = positions[particle_id];
    
    const int3 index = (int3)(floor((positions[particle_id] - voxel_grid_origin_corner) / voxel_grid_cell_dimensions));
    const int3 index_clamped = clamp(index, (int3)(0, 0, 0), voxel_grid_cell_count);

    if (index.x == index_clamped.x && index.y == index_clamped.y && index.z == index_clamped.z) {
        // The particle lies within the bounding volume, yay!

        // Atomical incrementation of the voxel cell's counter to ensure concurrent safety
        // TODO: investigate if the pointer to the particle counter actually works
        const int voxel_cell_index = index.x + index.y * voxel_grid_cell_count.x + index.z * voxel_grid_cell_count.x * voxel_grid_cell_count.y;

        const uint particle_index = atomic_inc(&(voxel_grid_cells[voxel_cell_index].particle_counter));

        // Store the particle at the index returned by the atomic increment (i.e. the old counter)
        voxel_grid_cells[voxel_cell_index].particles[particle_index].position = position;
        voxel_grid_cells[voxel_cell_index].particles[particle_index].velocity = velocities[particle_id];
    }
}