#version 120

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;

uniform float factor;

attribute vec4 vertex_position;
attribute vec3 vertex_normal;
attribute vec3 vertex_color;

varying vec3 fragment_position;
varying vec3 fragment_normal;
varying vec3 fragment_color;

void main()
{
    mat4 mv_matrix = view_matrix * model_matrix;

    vec3 cube_position_3d = vertex_position.xyz;
    vec3 sphere_position_3d = normalize(cube_position_3d);
    vec3 morphed_position_3d = factor * cube_position_3d + (1.0f - factor) * sphere_position_3d;
    vec3 morphed_normal_3d = factor * vertex_normal + (1.0f - factor) * sphere_position_3d;

    vec4 mv_morphed_position = mv_matrix * vec4(morphed_position_3d, 1.0f);
    gl_Position = projection_matrix * mv_morphed_position;

    fragment_position = vec3(mv_morphed_position);
    fragment_normal = normalize(vec3(mv_matrix * vec4(morphed_normal_3d, 0.0f)));
    fragment_color = vertex_color;
}
