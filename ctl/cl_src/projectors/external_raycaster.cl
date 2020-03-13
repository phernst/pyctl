#pragma OPENCL EXTENSION cl_khr_fp64: enable

// normalized direction vector to detector pixel [x,y]
float3 calculateDirection(double x, double y, double16 QR);
// parameters of the ray (specified by `source`and `direction`) for the entry and exit point
float2 calculateIntersections(float3 source, float3 direction, float3 volSize, float3 volCorner);

// interpolating sampler with `0` as boundary color
__constant sampler_t samp = CLK_NORMALIZED_COORDS_FALSE | CLK_ADDRESS_CLAMP | CLK_FILTER_LINEAR;

// the kernel
__kernel void ray_caster( uint detectorColumns,
                          float increment,
                          __constant float3* source,
                          __constant float3* volCorner,
                          __constant float3* voxelSize,
                          __constant double16* QR,
                          __global float* projection,
                          __read_only image3d_t volume )
{
    const uint x = get_global_id(0);
    const uint y = get_global_id(1);
    const float4 volDim = convert_float4(get_image_dim(volume));

    float3 direction = increment * calculateDirection((double)x,(double)y,*QR);
    const float2 rayBounds = calculateIntersections(*source,direction,volDim.xyz * *voxelSize,*volCorner);

    const uint steps = (rayBounds.y - rayBounds.x) <= 0.0f
                       ? 0
                       : (uint)( (rayBounds.y - rayBounds.x)/increment ) + 1;
    float3 position = (*source + rayBounds.x*direction - *volCorner) / *voxelSize;
    float projVal = 0.0f;
    float4 interpVoxelValue;
    direction /= *voxelSize;

    for(uint i = 0; i < steps; ++i)
    {
        interpVoxelValue = read_imagef(volume, samp, (float4)(position,0.0f));
        projVal += interpVoxelValue.x;
        position += direction;
    }

    projection[x + y*detectorColumns] = increment * projVal;
}


float2 calculateIntersections(float3 source, float3 direction, float3 volSize, float3 volCorner)
{
    const float3 corner1 = volCorner;
    const float3 corner2 = volCorner + volSize;

    // intersection of the ray with all six planes/faces of the volume
    const float3 lambda1 = (corner1 - source) / direction;
    const float3 lambda2 = (corner2 - source) / direction;

    // find the two intersections within the volume boundaries
    float minL = FLT_MAX;
    float maxL = 0.0f;
    float2 hit;

    hit = source.yz + lambda1.x*direction.yz;
    if(all(isgreaterequal(hit,corner1.yz)) && all(islessequal(hit,corner2.yz)))
    {
        minL = lambda1.x < minL ? lambda1.x : minL;
        maxL = lambda1.x > maxL ? lambda1.x : maxL;
    }

    hit = source.xz + lambda1.y*direction.xz;
    if(all(isgreaterequal(hit,corner1.xz)) && all(islessequal(hit,corner2.xz)))
    {
        minL = lambda1.y < minL ? lambda1.y : minL;
        maxL = lambda1.y > maxL ? lambda1.y : maxL;
    }

    hit = source.xy + lambda1.z*direction.xy;
    if(all(isgreaterequal(hit,corner1.xy)) && all(islessequal(hit,corner2.xy)))
    {
        minL = lambda1.z < minL ? lambda1.z : minL;
        maxL = lambda1.z > maxL ? lambda1.z : maxL;
    }

    hit = source.yz + lambda2.x*direction.yz;
    if(all(isgreaterequal(hit,corner1.yz)) && all(islessequal(hit,corner2.yz)))
    {
        minL = lambda2.x < minL ? lambda2.x : minL;
        maxL = lambda2.x > maxL ? lambda2.x : maxL;
    }

    hit = source.xz + lambda2.y*direction.xz;
    if(all(isgreaterequal(hit,corner1.xz)) && all(islessequal(hit,corner2.xz)))
    {
        minL = lambda2.y < minL ? lambda2.y : minL;
        maxL = lambda2.y > maxL ? lambda2.y : maxL;
    }

    hit = source.xy + lambda2.z*direction.xy;
    if(all(isgreaterequal(hit,corner1.xy)) && all(islessequal(hit,corner2.xy)))
    {
        minL = lambda2.z < minL ? lambda2.z : minL;
        maxL = lambda2.z > maxL ? lambda2.z : maxL;
    }

    // no intersection case
    minL = maxL==0.0f ? 0.0f : minL;

    // enforce positivity (ray needs to start from the source)
    return (float2)( fmax(minL,0.0f), fmax(maxL,0.0f) );
}

float3 calculateDirection(double x, double y, double16 QR)
{
    // Q^t*[x,y,1]
    const double3 Qtx = (double3)( QR.s0*x + QR.s3*y + QR.s6,
                                   QR.s1*x + QR.s4*y + QR.s7,
                                   QR.s2*x + QR.s5*y + QR.s8 );
    // R^-1*Qtx
    const double dz = Qtx.z / QR.se;
    const double dy = (Qtx.y - dz*QR.sd) / QR.sc;
    const double dx = (Qtx.x - dy*QR.sa - dz*QR.sb) / QR.s9;

    return normalize((float3)((float)dx,(float)dy,(float)dz));
}
