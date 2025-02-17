precision mediump float;

uniform float u_time;
uniform vec2 u_resolution;

float random(vec2 st) {
    return fract(sin(dot(st.xy, vec2(12.9898,78.233))) * 43758.5453123);
}

float noise(vec2 st) {
    vec2 i = floor(st);
    vec2 f = fract(st);
    float a = random(i);
    float b = random(i + vec2(1.0, 0.0));
    float c = random(i + vec2(0.0, 1.0));
    float d = random(i + vec2(1.0, 1.0));
    vec2 u = f * f * (3.0 - 2.0 * f);
    return mix(a, b, u.x) + (c - a)* u.y * (1.0 - u.x) + (d - b)* u.x * u.y;
}

void main() {
    vec2 st = gl_FragCoord.xy / u_resolution.xy;
    float t = u_time * 0.5;
    float n = noise(st * 10.0 - vec2(0.0, t*5.0));
    float intensity = smoothstep(0.3, 0.5, n);
    vec3 color = mix(vec3(1.0, 0.2, 0.0), vec3(1.0, 1.0, 0.0), intensity);
    
    gl_FragColor = vec4(color, 0.7);
}