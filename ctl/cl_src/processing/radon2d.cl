// interpolating sampler with `0` as boundary color
__constant sampler_t samp = CLK_NORMALIZED_COORDS_FALSE | CLK_ADDRESS_CLAMP | CLK_FILTER_LINEAR;

float8 xAxisToLineMapping(float s, float theta);

// the kernel
__kernel void radon2d( float lineReso,
                       __constant float* sVec,
                       __constant float* thetaVec,
                       __constant float2* origin,
                       __global float* result,
                       __read_only image2d_t image )
{
    const int2 imgDim = get_image_dim(image);
    const uint s = get_global_id(0);
    const uint theta = get_global_id(1);
    const uint nbTheta = get_global_size(1);

    const uint nbSamples = (uint)ceil(length(convert_float2(imgDim)) / lineReso);
    const float2 imgOrigin_pix = (*origin) + (float2)0.5f;
    const float lineOrigin_ax = 0.5f * (float)(nbSamples - 1);

    float8 H = xAxisToLineMapping(sVec[s], thetaVec[theta]);

    float sum = 0.0f;
    for(uint x = 0; x < nbSamples; ++x)
    {
        float3 pt = (float3)( (x - lineOrigin_ax) * lineReso, 0.0f, 1.0f);

        float2 coord = (float2)(dot(H.s012, pt), dot(H.s456, pt)) + imgOrigin_pix;

        sum += read_imagef(image, samp, coord).x;
    }

    result[s * nbTheta + theta] = sum * lineReso;
}

__kernel void radon2dsubset( float lineReso,
                             __constant float2* samplePoints,
                             __constant float2* origin,
                             __global float* result,
                             __read_only image2d_t image )
{
    const int2 imgDim = get_image_dim(image);
    const uint smplID = get_global_id(0);

    const uint nbSamples = (uint)ceil(length(convert_float2(imgDim)) / lineReso);
    const float2 imgOrigin_pix = (*origin) + (float2)0.5f;
    const float lineOrigin_ax = 0.5f * (float)(nbSamples - 1);

    float8 H = xAxisToLineMapping(samplePoints[smplID].s1, samplePoints[smplID].s0);

    float sum = 0.0f;
    for(uint x = 0; x < nbSamples; ++x)
    {
        float3 pt = (float3)( (x - lineOrigin_ax) * lineReso, 0.0f, 1.0f);

        float2 coord = (float2)(dot(H.s012, pt), dot(H.s456, pt)) + imgOrigin_pix;

        sum += read_imagef(image, samp, coord).x;
    }

    result[smplID] = sum * lineReso;
}


float8 xAxisToLineMapping(float s, float theta)
{
    const float co = cos(theta);
    const float si = sin(theta);

    return (float8)( si, co, s * co, 0.0f,
                    -co, si, s * si, 0.0f );
}
