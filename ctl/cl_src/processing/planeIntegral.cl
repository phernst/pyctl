// patch size
#define PATCH_SIZE 16

// interpolating sampler with `0` as boundary color
__constant sampler_t samp = CLK_NORMALIZED_COORDS_FALSE | CLK_ADDRESS_CLAMP | CLK_FILTER_LINEAR;

// the kernel
__kernel void planeInt( __constant float16* homography,
                        __constant float3* distanceShift,
                        __constant float* distances,
                        uint nbDist,
                        __global float* patchResults,
                        __read_only image3d_t volume )
{
    // create local memory
    local float patch[PATCH_SIZE][PATCH_SIZE];
    local float rowSums[PATCH_SIZE];

    // get ids
    const uint x = get_global_id(0);
    const uint y = get_global_id(1);
    const uint xPatch = get_local_id(0);
    const uint yPatch = get_local_id(1);
    const uint groupIDX = get_group_id(0);
    const uint groupIDY = get_group_id(1);
    const uint numGroupsX = get_num_groups(0);
    const uint numGroupsY = get_num_groups(1);

    // ny': sample of "template hyperplane"
    const float16 centerHomo = *homography;
    const float4 sliceCoord = (float4)((float)x, (float)y, 0.0f, 1.0f);

    for(uint d = 0; d < nbDist; ++d)
    {
        float16 finalHomo = centerHomo;
        finalHomo.s37b += distances[d] * (*distanceShift);
        // H^-1 * ny'
        const float4 voxToRead = (float4)(dot(finalHomo.s0123, sliceCoord),
                                          dot(finalHomo.s4567, sliceCoord),
                                          dot(finalHomo.s89ab, sliceCoord),
                                          0.0f);

        patch[yPatch][xPatch] = read_imagef(volume, samp, voxToRead).x;

        barrier(CLK_LOCAL_MEM_FENCE);

        if(xPatch == 0)
        {
            float16 sum = vload16(0, patch[yPatch]);

            rowSums[yPatch] = dot(sum.s0123, (float4)1.0f) +
                              dot(sum.s4567, (float4)1.0f) +
                              dot(sum.s89ab, (float4)1.0f) +
                              dot(sum.scdef, (float4)1.0f);
        }

        barrier(CLK_LOCAL_MEM_FENCE);

        if(xPatch == 0 && yPatch == 0)
        {
            float16 tmp = vload16(0, rowSums);
            float totalSum =  dot(tmp.s0123, (float4)1.0f) +
                              dot(tmp.s4567, (float4)1.0f) +
                              dot(tmp.s89ab, (float4)1.0f) +
                              dot(tmp.scdef, (float4)1.0f);
            patchResults[d * numGroupsX * numGroupsY + groupIDY * numGroupsX + groupIDX] = totalSum;
        }
    }
}
