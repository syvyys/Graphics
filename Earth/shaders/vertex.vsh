#version 120

attribute highp vec4 posAttr;
attribute highp vec3 norAttr;
attribute highp vec2 texAttr;
attribute highp vec3 tanAttr;
attribute highp vec3 bitanAttr;

uniform highp vec3 viewPos;
uniform highp vec3 lightPos;
uniform highp mat4 viewMatrix;
uniform highp mat4 modelMatrix;
uniform highp mat3 normalMatrix;
uniform highp mat4 projectionMatrix;

varying highp vec3 v_lightPos;
varying highp vec3 v_viewPos;
varying highp vec3 v_fragPos;
varying highp vec2 v_textCoord;
varying highp mat3 TBN;

void main()
{
    mat4 mv_matrix = modelMatrix * viewMatrix;

    v_viewPos = viewPos;
    v_lightPos = lightPos;
    v_textCoord = texAttr;
    v_fragPos = vec3(modelMatrix * posAttr);

    vec3 T = normalize(vec3(mv_matrix * vec4(tanAttr, 0.0)));
    vec3 B = normalize(vec3(mv_matrix * vec4(bitanAttr, 0.0)));
    vec3 N = normalize(vec3(mv_matrix * vec4(norAttr, 0.0)));
    TBN = mat3(T, B, N);

    gl_Position = projectionMatrix * mv_matrix * posAttr;
}
