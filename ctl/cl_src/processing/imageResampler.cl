// interpolating sampler with `0` as boundary color
__constant sampler_t samp = CLK_NORMALIZED_COORDS_FALSE | CLK_ADDRESS_CLAMP | CLK_FILTER_LINEAR;

// the kernel
__kernel void resample( __constant float2* range1,
                        __constant float2* range2,
                        __constant float* samplePts1,
                        __constant float* samplePts2,
                        __read_only image2d_t image,
                        __global float* resampledImg)
{
    // Output sample IDs
    const uint sampleID1 = get_global_id(0);
    const uint sampleID2 = get_global_id(1);
    // dimension of input image
    const int2 imgDim = get_image_dim(image);

    // scales from unit (defined by range) to pixel number
    const float scaleX = ((float)imgDim.x - 1.0f) / ((*range1).s1 - (*range1).s0);
    const float scaleY = ((float)imgDim.y - 1.0f) / ((*range2).s1 - (*range2).s0);
    // calculate voxel coordinate of input image for current sample IDs
    float2 pixCoord;
    pixCoord.x = scaleX * (samplePts1[sampleID1] - (*range1).s0);
    pixCoord.y = scaleY * (samplePts2[sampleID2] - (*range2).s0);
    // OpenCL Grid offset
    pixCoord += (float2)0.5f;

    // interpolate value in input image
    const float4 resampledVal = read_imagef(image, samp, pixCoord);

    // write value to output buffer
    resampledImg[sampleID1 +
                 sampleID2 * get_global_size(0)] = resampledVal.x;
}

__kernel void sample( __constant float2* range1,
                      __constant float2* range2,
                      __constant float* samplePts,
                      __read_only image2d_t image,
                      __global float* resampledImg)
{
    // Output sample IDs
    const uint sampleID = get_global_id(0);

    // dimension of input image
    const int2 imgDim = get_image_dim(image);

    // scales from unit (defined by range) to voxel number
    const float scaleX = ((float)imgDim.x - 1.0f) / ((*range1).s1 - (*range1).s0);
    const float scaleY = ((float)imgDim.y - 1.0f) / ((*range2).s1 - (*range2).s0);
    // calculate voxel coordinate of input image for current sample IDs
    float2 pixCoord;
    pixCoord.x = scaleX * (samplePts[2 * sampleID] - (*range1).s0);
    pixCoord.y = scaleY * (samplePts[2 * sampleID+1] - (*range2).s0);
    // OpenCL Grid offset
    pixCoord += (float2)0.5f;

    // interpolate value in input image
    const float4 resampledVal = read_imagef(image, samp, pixCoord);

    // write value to output buffer
    resampledImg[sampleID] = resampledVal.x;
}
