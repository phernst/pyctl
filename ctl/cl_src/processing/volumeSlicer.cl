// interpolating sampler with `0` as boundary color
__constant sampler_t samp = CLK_NORMALIZED_COORDS_FALSE | CLK_ADDRESS_CLAMP | CLK_FILTER_LINEAR;

// the kernel
__kernel void slicer( __constant float3* volCorner_vox,
                      __constant uint2* sliceDim,
                      __constant float16* homography,
                      __global float* slice,
                      __read_only image3d_t volume )
{
    const float4 volDim = convert_float4(get_image_dim(volume));

    const float2 sliceCorner_pix = 0.5f * convert_float2(*sliceDim - (uint2)(1));

    uint x = get_global_id(0);
    uint y = get_global_id(1);

    uint idx = y * (*sliceDim).x + x;

    const float4 vec = (float4)((float)x - sliceCorner_pix.x,
                                (float)y - sliceCorner_pix.y,
                                0.0f,
                                1.0f);

    float4 voxToRead = (float4)(dot((*homography).s0123, vec),
                                dot((*homography).s4567, vec),
                                dot((*homography).s89ab, vec),
                                0.0f) - (float4)(*volCorner_vox, 0.0f) + (float4)(0.5f);

    slice[idx] = read_imagef(volume, samp, voxToRead).x;
}
