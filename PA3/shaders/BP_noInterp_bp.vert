# version 120

attribute vec3 position;
attribute vec3 vNormal;
attribute vec3 texCoord;
attribute vec3 vTangent;

uniform vec3 ambientProduct;
uniform vec3 diffuseProduct;
uniform vec3 specularProduct;

uniform float shininess, bumpness;

uniform vec4 lightPosition;

uniform sampler2D texture;

uniform mat4 modelToWorld;
uniform mat4 worldToCamera;
uniform mat4 cameraToView;

varying vec4 fColor;

void main() {
    mat4 chain = cameraToView * worldToCamera * modelToWorld;

    vec4 tex = vec4 (texCoord, 1.0);
    vec4 t = texture2D(texture, tex.st);

    vec3 pos = vec4 (chain * vec4(position, 1.0)).xyz;

    vec3 N = normalize(chain * vec4 (vNormal,1.0)).xyz;
    vec3 tangent = normalize(chain * vec4 (vTangent,1.0)).xyz;

    N = normalize(N+bumpness*normalize(tangent)*(t.r-0.5)+bumpness*cross(tangent,N)*(t.g-0.5));

    mat3 tbn = mat3 (tangent, cross(tangent, N), N);
    tbn = transpose(tbn);

    vec3 L = normalize(tbn*lightPosition.xyz);
    vec3 E = normalize(tbn*pos);
    vec3 H = normalize(L+E);

    vec4 ambient = vec4(ambientProduct, 1.0);
    vec4 diffuse = vec4(diffuseProduct, 1.0) * max( dot(L, N), 0.0 );
    vec4 specular = vec4(specularProduct, 1.0) * max(0.0, pow(dot(N, H), shininess));

    fColor = ambient + diffuse + specular;
    fColor.a = 1.0;

    gl_Position = chain * vec4(position, 1.0);
}
