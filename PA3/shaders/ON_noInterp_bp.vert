# version 120

attribute vec3 position;
attribute vec3 vNormal;
attribute vec3 texCoord;
attribute vec3 vTangent;

uniform vec3 diffuseProduct;
uniform float albedo;
uniform float roughness;
uniform float bumpness;

uniform sampler2D texture;

uniform vec4 lightPosition;

uniform mat4 modelToWorld;
uniform mat4 worldToCamera;
uniform mat4 cameraToView;

varying vec4 fColor;

void main() {
    mat4 chain = cameraToView * worldToCamera * modelToWorld;

    vec4 tex = vec4 (texCoord, 1.0);
    vec4 t = texture2D(texture, tex.st);

    vec3 pos = (chain * vec4(position, 1.0)).xyz;

    vec3 N = normalize(chain * vec4 (vNormal,1.0)).xyz;
    vec3 tangent = normalize(chain * vec4 (vTangent,1.0)).xyz;

    N = normalize(N+bumpness*normalize(tangent)*(t.r-0.5)+bumpness*cross(tangent,N)*(t.g-0.5));

    mat3 tbn = mat3 (tangent, cross(tangent, N), N);
    tbn = transpose(tbn);

    vec3 L = normalize(tbn*lightPosition.xyz);
    vec3 E = normalize(tbn*pos);

    float sig2 = pow(roughness, 2);
    float A = 1.0 - 0.5 * (sig2/(sig2+0.57));
    float B = 0.45 * (sig2/(sig2+0.09));

    float alpha = max(acos(dot(N, L)), acos(dot(N,E)));
    float beta = min(acos(dot(N, L)), acos(dot(N,E)));
    float gamma = dot(normalize(E - N * dot(E, N)), normalize(L - N * dot(L, N)));

    fColor = vec4(diffuseProduct, 1.0) * (A + B*max(0.0, gamma)* sin(alpha)*tan(beta)) * albedo * dot(N, L) / 3.1415;
    fColor.a = 1.0;

    gl_Position = chain * vec4(position, 1.0);
}
