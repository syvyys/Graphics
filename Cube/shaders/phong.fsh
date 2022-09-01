#version 120

uniform vec3 light_position;

varying vec3 fragment_position;
varying vec3 fragment_normal;
varying vec3 fragment_color;

void main()
{
    vec3 light_color = vec3(1.0f, 1.0f, 1.0f);
    vec3 view_position = vec3(0.0f, 0.0f, 0.0f);
    vec3 normal = normalize(fragment_normal);
    vec3 light_direction = normalize(light_position - fragment_position);

    // ambient light
    float ambient_power = 0.2f;
    vec3 ambient = ambient_power * light_color;

    // diffuse light
    float diffuse_power = max(dot(normal, light_direction), 0.0f);
    vec3 diffuse = diffuse_power * light_color;

    // specular light
    float specular_power = 1.0f;
    float specular_exponent = 256.0f;
    vec3 view_direction = normalize(view_position - fragment_position);
    vec3 reflection_direction = reflect(-light_direction, normal);
    float specular_angular = pow(max(dot(view_direction, reflection_direction), 0.0f), specular_exponent);
    vec3 specular = specular_power * specular_angular * light_color;

    vec3 result_color = (ambient + diffuse + specular) * fragment_color;

    gl_FragColor = vec4(result_color, 1.0f);
}
