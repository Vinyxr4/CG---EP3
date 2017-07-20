# version 120

attribute vec3 position;
attribute vec3 vNormal;
attribute vec3 texCoord;

varying vec3 N, E, L;
varying vec4 tex;

uniform mat4 modelToWorld;
uniform mat4 worldToCamera;
uniform mat4 cameraToView;

uniform vec4 lightPosition;

void main() {
    mat4 chain = cameraToView * worldToCamera * modelToWorld;

    tex = vec4 (texCoord, 1.0);

    vec3 pos = (vec4 (chain * vec4(position, 1.0))).xyz;

    E = normalize( pos );
    N = normalize(vec4 (chain * vec4(vNormal, 1.0))).xyz;
    L = normalize (lightPosition.xyz);

    gl_Position = chain * vec4(position, 1.0);
}
