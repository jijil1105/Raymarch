#include"BasicType.hlsli"

Texture2D g_texture : register(t0);
SamplerState g_sampler : register(s0);

#define PI 3.14159265
#define mod(a, b) ((a) - (b) * floor((a) / (b)))
#define lofi(x,d) (floor((x)/(d)))
#define saturate(a) (clamp(a,0,1.))
#define linearstep(a,b,t) (saturate(((t)-(a))/((b)-(a))))
#define rand(z) frac(sin(z)*1352.2342)
#define bpm 120.
#define beatlength 60./bpm

float grid = 6.;

float3 rand3f(float3 co)
{
    float z = dot(co, float3(1.1521, 1.4322, 1.4545));
    return rand(float3(z,z,z));
}

float fractsin(float v)
{
    return frac(sin(v * 121.445) * 34.59);
}

float easeceil(float t, float fac)
{
    return floor(t) + .5 + .5 * cos(PI * exp(fac * frac(t)));
}

float2x2 rot2d(float t)
{
    return float2x2(cos(t), -sin(t), sin(t), cos(t));
}

float3 ifs(float3 p, float3 rot, float3 shift)
{
    float3 pt = abs(p);
    float3 t = shift;
    for (int i = 0; i < 6; i++)
    {
        pt = abs(pt) - abs(t * pow(1.8, -float(i)));
        t.yz = mul(rot2d(rot.x), t.yz);
        t.zx = mul(rot2d(rot.y), t.zx);
        t.xy = mul(rot2d(rot.z), t.xy);
        pt.xy = pt.x < pt.y ? pt.yx : pt.xy;
        pt.yz = pt.y < pt.z ? pt.zy : pt.yz;
    }
    return pt;
}

float sdbox(float3 p, float3 s)
{
    float3 d = abs(p) - s;
    return length(max(d, 0.));
}

float4 map(float3 p, float time)
{
    float3 pt = p;
    float3 cell = floor(pt / grid);
    float phase = time;
    phase = easeceil(phase + length(rand3f(cell)), -10);
    pt = mod(pt, grid) - grid / 2;
    pt.yz = mul(rot2d(.5 * PI * phase + 25 * PI), pt.yz);
    float clampBox = sdbox(pt, float3(.7, .7, 3.));
    pt = ifs(pt, float3(2.0, phase, 2.0), float3(.5, .6, .7));
    pt = mod(pt - .5, 1.) - .5;
    float dist = sdbox(pt, float3(.03,.03,.03));
    dist = max(dist, clampBox);
    return float4(
  dist,
  sin(PI * frac(phase)),
  0.0,
  0.0);
}

float3 normalFunc(float3 p, float2 d, float time)
{
    return normalize(float3(
    map(p + d.yxx, time).x - map(p - d.yxx, time).x,
    map(p + d.xyx, time).x - map(p - d.xyx, time).x,
    map(p + d.xxy, time).x - map(p - d.xxy, time).x
  ));
}

float aoFunc(float3 p, float3 n, float time)
{
    float accum = 0;
    for (int i = 0; i < 64; i++)
    {
        float3 d = (.02 + .02 * float(i)) * rand3f(p);
        d = dot(d, n) < .0 ? -d : d;
        accum += linearstep(0.02,0.0,map(p+d, time).x) / 64.0;
    }
    return 1.0 - sqrt(saturate(4.0*accum));

}

float4 main(BasicType input) : SV_TARGET
{
    float2 resolution = input.resolution;
    float time = input.elapsedTime * beatlength;
    float2 p = (input.uv * 2.0f - 1);
    float4 o = float4(0,0,0,1);
    float3 ro = float3(0, 0, -time);
    float3 rd = float3(p, -1);
    rd.yz = mul(rot2d(floor(time / 2) + 0.03 * sin(time)), rd.yz);
    rd.zx = mul(rot2d(0.76 * time + 0.03 * cos(time)), rd.zx);
    rd.xy = mul(rot2d(0.48 * time), rd.xy);
    rd = normalize(rd);
  
    float4 dist;
    float rl = 0.01;
    float3 rp = ro + rl * rd;
    float count = 0;
    float countMax = 99;
    for (count = countMax; count > 0; count--)
    {
        dist = map(rp, time);
        rl += dist.x * 0.5;
        rp = ro + rl * rd;
        if (dist.x < 1E-2)
            break;
    }
  
    if (dist.x < 1E-2)
    {
        float shade = count / countMax;
        float3 n = normalFunc(rp, float2(0.0, 2E-3), time);
        float3 n2 = normalFunc(rp, float2(0.0, 1E-2), time);
        float edge = length(n - n2);
        //float ao = aoFunc(rp, n, time);
        //float3 ao3d = ao;
        float3 tint = .5 + .5 * cos(float3(3, 2, 1) + length(rp) * .5);
        // lighting
        float ld = dot(reflect(rd, n), float3(0, 1, 0)) * 0.5 + 0.5;
        float3 light = float3(.702, 1., .502) * sqrt(ld) * .3;
        ld = dot(reflect(rd, n), float3(0, 0, 1)) * 0.5 + 0.5;
        light += float3(1., 0.714, 0.145) * sqrt(ld) * .5;
        //o.xyz = shade * ao3d * (tint + light);
        o.xyz = shade * (tint + light);
        o.xyz += edge * dist.y * float3(15., 1., 1.5);
    }
    
    float3 f3d74545 = 2.9545;
    o.xyz = pow(o.xyz, f3d74545);
    o.xyz = float3(
    smoothstep(.1, .4, o.x),
    linearstep(.1, .5, o.y),
    smoothstep(-.3, 1.1, o.z));
    
    return o;
}