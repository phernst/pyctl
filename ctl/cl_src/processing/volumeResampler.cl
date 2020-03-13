// interpolating sampler with `0` as boundary color
__constant sampler_t samp = CLK_NORMALIZED_COORDS_FALSE | CLK_ADDRESS_CLAMP | CLK_FILTER_LINEAR;

// the kernel
__kernel void resample( __constant float2* range1,
                        __constant float2* range2,
                        __constant float2* range3,
                        __constant float* samplePts1,
                        __constant float* samplePts2,
                        __constant float* samplePts3,
                        __read_only image3d_t volume,
                        __global float* resampledVol)
{
    // Output sample IDs
    const uint sampleID1 = get_global_id(0);
    const uint sampleID2 = get_global_id(1);
    const uint sampleID3 = get_global_id(2);
    // dimension of input image
    const int4 volDim = get_image_dim(volume);

    // scales from unit (defined by range) to voxel number
    const float scaleX = ((float)volDim.x - 1.0f) / ((*range1).s1 - (*range1).s0);
    const float scaleY = ((float)volDim.y - 1.0f) / ((*range2).s1 - (*range2).s0);
    const float scaleZ = ((float)volDim.z - 1.0f) / ((*range3).s1 - (*range3).s0);
    // calculate voxel coordinate of input image for current sample IDs
    float4 voxCoord;
    voxCoord.x = scaleX * (samplePts1[sampleID1] - (*range1).s0);
    voxCoord.y = scaleY * (samplePts2[sampleID2] - (*range2).s0);
    voxCoord.z = scaleZ * (samplePts3[sampleID3] - (*range3).s0);
    voxCoord.w = 0.0f;
    // OpenCL Grid offset
    voxCoord += (float4)0.5f;

    // interpolate value in input image
    const float4 resampledVal = read_imagef(volume, samp, voxCoord);

    // write value to output buffer
    resampledVol[sampleID1 +
                 sampleID2 * get_global_size(0) +
                 sampleID3 * get_global_size(0) * get_global_size(1)] = resampledVal.x;
}

__kernel void sample( __constant float2* range1,
                      __constant float2* range2,
                      __constant float2* range3,
                      __constant float* samplePts,
                      __read_only image3d_t volume,
                      __global float* resampledVol)
{
    // Output sample IDs
    const uint sampleID = get_global_id(0);

    // dimension of input image
    const int4 volDim = get_image_dim(volume);

    // scales from unit (defined by range) to voxel number
    const float scaleX = ((float)volDim.x - 1.0f) / ((*range1).s1 - (*range1).s0);
    const float scaleY = ((float)volDim.y - 1.0f) / ((*range2).s1 - (*range2).s0);
    const float scaleZ = ((float)volDim.z - 1.0f) / ((*range3).s1 - (*range3).s0);
    // calculate voxel coordinate of input image for current sample IDs
    float4 voxCoord;
    voxCoord.x = scaleX * (samplePts[3 * sampleID] - (*range1).s0);
    voxCoord.y = scaleY * (samplePts[3 * sampleID+1] - (*range2).s0);
    voxCoord.z = scaleZ * (samplePts[3 * sampleID+2] - (*range3).s0);
    voxCoord.w = 0.0f;
    // OpenCL Grid offset
    voxCoord += (float4)0.5f;

    // interpolate value in input image
    const float4 resampledVal = read_imagef(volume, samp, voxCoord);

    // write value to output buffer
    resampledVol[sampleID] = resampledVal.x;
}
