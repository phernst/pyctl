#pragma OPENCL EXTENSION cl_khr_fp64: enable

// normalized direction vector (world coord. frame) to detector pixel [x,y]
float3 calculateDirection(double x, double y, double16 QR);
// parameters of the ray (specified by `source`and `direction`) for the entry and exit point
float2 calculateIntersections(float3 source, float3 direction, float3 volSize, float3 volCorner);
// helper for `calculateIntersections`: checks `lambda` as a candidate for a parameter of entry/exit
// point compared to given `minMax` values for a certain face of the volume. `corner1` and `corner2`
// are the corners of the 2d face and `hit` is the intersection of the ray with the face.
float2 checkFace(float2 hit, float2 corner1, float2 corner2, float lambda, float2 minMax);

// interpolating sampler with `0` as boundary color
__constant sampler_t samp = CLK_NORMALIZED_COORDS_FALSE | CLK_ADDRESS_CLAMP | CLK_FILTER_LINEAR;
// slightly earlier/later entry/exit point (1% of voxel size) allowed for numerical reasons
__constant float3 EPS = (float3)(0.01f, 0.01f, 0.01f);

// the kernel
__kernel void ray_caster( float increment_mm,
                          __constant uint2* raysPerPixel,
                          __constant float3* src_mm,
                          __constant float3* corner_mm,
                          __constant float3* voxelSize_mm,
                          __constant double16* QR,
                          __global float* projection,
                          __read_only image3d_t volume )
{
    // IDs and sizes
    const uint detectorColumns = get_global_size(0);
    const uint detectorRows = get_global_size(1);
    const uint x = get_global_id(0);
    const uint y = get_global_id(1);
    const uint module = get_global_id(2);
    // quantities normalized by the voxel size (units of "voxel numbers")
    const float4 volDim = convert_float4(get_image_dim(volume));
    const float3 source = *src_mm / *voxelSize_mm;
    const float3 volCorner = *corner_mm / *voxelSize_mm;
    const float3 cornerToSourceVector = source - volCorner;
    // quantities related to the projection image pixels
    const float2 intraPixelSpacing = (float2)1.0f / convert_float2(*raysPerPixel);
    const float2 pixelCornerPlusOffset = (float2)((float)x, (float)y) - (float2)0.5f + 0.5f*intraPixelSpacing;

    // helper variables
    float4 interpVoxelValue;
    float3 direction, position;
    float2 rayBounds, pixelCoord;

    // resulting projection value
    double projVal = 0.0;

    for(uint rayX = 0; rayX < raysPerPixel[0].x; ++rayX)
        for(uint rayY = 0; rayY < raysPerPixel[0].y; ++rayY)
        {
            pixelCoord = pixelCornerPlusOffset + (float2)((float)rayX, (float)rayY) * intraPixelSpacing;
            direction = increment_mm * calculateDirection((double)pixelCoord.x,
                                                          (double)pixelCoord.y,
                                                          QR[module]);
            direction /= *voxelSize_mm; // normalize to voxel numbers

            rayBounds = calculateIntersections(source, direction, volDim.xyz, volCorner);

            for(uint i = (uint)rayBounds.x, end = (uint)rayBounds.y+1; i <= end; ++i)
            {
                position = mad((float3)i, direction, cornerToSourceVector); // i*direction + cornerToSourceVector
                
                interpVoxelValue = read_imagef(volume, samp, (float4)(position, 0.0f));
                projVal += (double)interpVoxelValue.x;
            }
        }

    projection[x + y*detectorColumns + module*detectorColumns*detectorRows] =
        increment_mm * (float)projVal / (float)(raysPerPixel[0].x * raysPerPixel[0].y);
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

float2 calculateIntersections(float3 source, float3 direction, float3 volSize, float3 volCorner)
{
    // additional half voxel needs to be traced due to the higher extent caused by the interpolation
    float3 corner1 = volCorner - (float3)0.5f;
    float3 corner2 = volCorner + volSize + (float3)0.5f;

    // intersection of the ray with all six planes/faces of the volume
    const float3 lambda1 = (corner1 - source) / direction;
    const float3 lambda2 = (corner2 - source) / direction;
    
    // relax boundary conditions
    corner1 -= EPS;
    corner2 += EPS;

    // find the two intersections within the volume boundaries (entry/exit)
    float2 minMax = (float2)(FLT_MAX, 0.0f);
    float2 hit;

    // # lambda1: faces around corner1
    // yz-face
    hit = source.yz + lambda1.x*direction.yz;
    minMax = checkFace(hit, corner1.yz, corner2.yz, lambda1.x, minMax);

    // xz-face
    hit = source.xz + lambda1.y*direction.xz;
    minMax = checkFace(hit, corner1.xz, corner2.xz, lambda1.y, minMax);

    // xy-face
    hit = source.xy + lambda1.z*direction.xy;
    minMax = checkFace(hit, corner1.xy, corner2.xy, lambda1.z, minMax);

    // # lambda2: faces around corner2
    // yz-face
    hit = source.yz + lambda2.x*direction.yz;
    minMax = checkFace(hit, corner1.yz, corner2.yz, lambda2.x, minMax);

    // xz-face
    hit = source.xz + lambda2.y*direction.xz;
    minMax = checkFace(hit, corner1.xz, corner2.xz, lambda2.y, minMax);

    // xy-face
    hit = source.xy + lambda2.z*direction.xy;
    minMax = checkFace(hit, corner1.xy, corner2.xy, lambda2.z, minMax);

    // enforce positivity (ray needs to start from the source)
    return fmax(minMax, (float2)0.0f);
}

float2 checkFace(float2 hit, float2 corner1, float2 corner2, float lambda, float2 minMax)
{
    // basic condition: ray must hit the volume (must not pass by)
    const int intersects = all( (int4)(isgreaterequal(hit, corner1), 
                                          islessequal(hit, corner2)) );

    // check for intersection and if new `lambda` is less/greater then `minMax`
    const int2 conditions = (int2)intersects &&
                            (int2)(lambda < minMax.x, lambda > minMax.y);
    
    // select new `lambda` if a condition is true, otherwise return old `minMax`
    return select(minMax, (float2)lambda, conditions);
}
