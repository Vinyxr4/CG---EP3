# version 120

attribute vec3 position;
attribute vec3 vNormal;
attribute vec3 texCoord;
attribute vec3 vTangent;

uniform mat4 modelToWorld;
uniform mat4 worldToCamera;
uniform mat4 cameraToView;

uniform vec4 lightPosition;

uniform sampler2D texture;

varying vec3 N, E, L, tangent;
varying vec4 tex;

void main() {
    mat4 chain = cameraToView * worldToCamera * modelToWorld;

    tex = vec4 (texCoord, 1.0);

    vec4 t = texture2D(texture, tex.st);

    N = normalize(vec4 (chain * vec4(vNormal, 1.0))).xyz;

    vec3 pos = vec4 (chain * vec4(position, 1.0)).xyz;

    tangent = normalize(vec4 (chain * vec4(vTangent, 1.0))).xyz;

    mat3 tbn = mat3 (tangent, cross(tangent, N), N);
    tbn = transpose(tbn);

    L = normalize(tbn*lightPosition.xyz);
    E = normalize(tbn*pos);

    gl_Position = chain * vec4(position, 1.0);
}
