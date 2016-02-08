#pragma once

#include "ParticleSimulator.hpp"

#ifdef __APPLE__

#include <OpenCL/opencl.h>
#include <OpenGL/OpenGL.h>

#else
#include <CL/cl.hpp>
#endif

#ifdef _WIN32
#include <windows.h>
#endif

#include <vector>

class OpenClParticleSimulator : public ParticleSimulator {
public:
    ~OpenClParticleSimulator();

    void setupSimulation(const std::vector<glm::vec3> &particle_positions,
                         const std::vector<glm::vec3> &particle_velocities,
                         const GLuint &vbo_positions,
                         const GLuint &vbo_velocities);

    void updateSimulation(float dt_seconds);

private:
    int n_particles;

    void *cl_positions_buffer, *cl_velocities_buffer;

    cl_mem cl_positions, cl_velocities;

    cl_mem cl_positions_readonly, cl_velocities_readonly;

    cl_mem cl_positions_writeonly, cl_velocities_writeonly;

    cl_mem cl_voxel_grid;

    cl_int3 cl_voxel_grid_cell_count;

    std::vector<cl_platform_id> platformIds;

    std::vector<cl_device_id> deviceIds;

    int chosen_device_id;

    cl_context context;

    cl_command_queue command_queue;

    cl_mem cl_dt_obj;

    cl_mem cl_utility_particle_counter;

    void initOpenCL();

    void setupSharedBuffers(const GLuint &vbo_positions, const GLuint &vbo_velocities);

    void createAndBuildKernel(cl_kernel &kernel_out, std::string kernel_name, std::string kernel_file_name);

    void allocateVoxelGridBuffer();

    /* Kernels */

    /// A simple, stand-alone kernel that integrates the positions based on the velocities
    /// Is not a part of the fluid simulation processing chain
    void runSimpleIntegratePositionsKernel(float dt_seconds, std::vector<cl_event> &events);

    cl_kernel simple_integration = NULL;

    void runPopulateVoxelGridKernel(float dt_seconds, std::vector<cl_event> &events);

    cl_kernel populate_voxel_grid = NULL;

    void runCalculateParticleDensitiesKernel(float dt_seconds, std::vector<cl_event> &events);

    cl_kernel calculate_particle_densities = NULL;

    void runCalculateParticleForcesAndIntegrateStatesKernel(float dt_seconds, std::vector<cl_event> &events);

    cl_kernel calculate_particle_forces_integrate = NULL;

    void runMoveParticlesToOpenGlBufferKernel(float dt_seconds, std::vector<cl_event> &events);

    cl_kernel move_particles_to_ogl = NULL;
};