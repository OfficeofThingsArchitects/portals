#version 150

uniform sampler2DRect tex0;
uniform vec2 resolution;
uniform float dotSize;
uniform vec3 palette[10];

in vec2 vTexCoord;
out vec4 fragColor;

const float PI = 3.141592653;

// All components are in the range [0…1], including hue.
vec3 rgb2hsv(vec3 c)
{
    vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
    vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));
    vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));

    float d = q.x - min(q.w, q.y);
    float e = 1.0e-10;
    return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
}

// All components are in the range [0…1], including hue.
vec3 hsv2rgb(vec3 c)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}


vec3 closestColor(vec3 color) {
    vec3 targetHSV = rgb2hsv(color);

    vec3 closest = palette[0];
    float minDist = 999.0;

    //float dmin = distance(idealColor, color0);

    for (int i = 0; i < 10; i++) {
        vec3 col = palette[i];
        vec3 colHSV = rgb2hsv(col);
        
        float dr = abs(color.r - col.r);
        float dg = abs(color.g - col.g);
        float db = abs(color.b - col.b);
        
        float dist = sqrt(dr * dr + dg * dg + db * db);
        
        if (dist < minDist) {
            closest = col;
            minDist = dist;
        }
    }
    return closest;
}

float greyscale(vec3 color, float strength) {
    float g = dot(color, vec3(0.299, 0.587, 0.114));
    return g * strength;
}

float hash(float n) { return fract(sin(n) * 1e4); }
float hash(vec2 p) { return fract(1e4 * sin(17.0 * p.x + p.y * 0.1) * (0.1 + abs(sin(p.y * 13.0 + p.x)))); }

vec2 hash22(vec2 p) {
    vec3 p3 = fract(vec3(p.xyx) * vec3(.1031, .1030, .0973));
    p3 += dot(p3, p3.yzx+33.33);
    return fract((p3.xx+p3.yz)*p3.zy);
}

vec2 hash21(float p)
{
    vec3 p3 = fract(vec3(p,p,p) * vec3(.1031, .1030, .0973));
    p3 += dot(p3, p3.yzx + 33.33);
    return fract((p3.xx+p3.yz)*p3.zy);
}

float blueNoise(vec2 p) {
return  (hash(p+vec2(-1,-1)) + hash(p+vec2(0,-1)) + hash(p+vec2(1,-1))
        + hash(p+vec2(-1, 0)) - 8.* hash( p ) + hash(p+vec2(1, 0))
        + hash(p+vec2(-1, 1)) + hash(p+vec2(0, 1)) + hash(p+vec2(1, 1))) * .5/9. * 2.1 +.5;
}

float noise(vec2 x) {
    vec2 i = floor(x);
    vec2 f = fract(x);

    float a = hash(i);
    float b = hash(i + vec2(1.0, 0.0));
    float c = hash(i + vec2(0.0, 1.0));
    float d = hash(i + vec2(1.0, 1.0));

    vec2 u = f * f * (3.0 - 2.0 * f);
    return mix(a, b, u.x) + (c - a) * u.y * (1.0 - u.x) + (d - b) * u.x * u.y;
}

// R dither mask
float intensity(vec2 pixel) {
    const float a1 = 0.75487766624669276;
    const float a2 = 0.569840290998;
    // const float a1 = 0.45;
    // const float a2 = 0.3;
    return fract(a1 * pixel.x + a2 * pixel.y);
}

float dither(vec2 coords, float color, float grey) {
    // Calculated noised gray value
    float noised = (2.0/grey) * (intensity(coords)) + color - (1.0/grey);
    // Clamp to the number of gray levels we want
    float levels = clamp(floor(grey * noised) / (grey-1.0), 0.0, 1.0);
    return levels;
}

//saturation function
vec3 sat(vec3 rgb, float intensity) {
    vec3 L = vec3(0.2125, 0.7154, 0.0721);
    vec3 grayscale = (vec3(dot(rgb, L)));
    return mix(grayscale, rgb, intensity);
}

//map range
float map(float value, float inMin, float inMax, float outMin, float outMax) {
    return outMin + (outMax - outMin) * (value - inMin) / (inMax - inMin);
}

void main() {

  float aspectRatio = resolution.x / resolution.y;
  float scale = dotSize;
  vec2 newRes = resolution / scale;
  //newRes.x *= aspectRatio;

  vec2 uv = floor((vTexCoord / resolution) * newRes) / newRes;
  
  //uv = floor( uv * newRes ) / newRes; //the actual magic.

  // get the image as a vec4 using texture2D and plug in our distored uv's
  vec4 inColor = texture(tex0, uv * resolution);

  float gray = 2.0;

  vec2 xyPos = floor(vTexCoord / scale);
  vec3 dith = vec3(dither(xyPos, inColor.r, gray), dither(xyPos, inColor.g, gray), dither(xyPos, inColor.b, gray));

  //vec4 color = texture(tex0, vTexCoord);
  // Apply a simple grayscale filter
  //float gray = dot(color.rgb, vec3(0.299, 0.587, 0.114));
  //fragColor = vec4(inColor);

  fragColor = vec4(closestColor(dith.rgb), 1.0);
}