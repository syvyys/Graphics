#version 120

uniform sampler2D map;
uniform sampler2D texture;

varying highp vec3 v_lightPos;
varying highp vec3 v_viewPos;
varying highp vec3 v_fragPos;
varying highp vec2 v_textCoord;
varying highp mat3 TBN;

uniform vec3 objectColor = vec3(0.2f, 0.3f, 0.8f);

void main()
{
    vec3 normal = texture2D(map, v_textCoord).rgb;      // достаем нормали
    normal = normalize(TBN * (normal * 2.0f - 1.0f));   // преобразуем к [-1, 1] +
                                                        // переводим в tangent space

    // ambient light
    float ambient = 0.2f;

    // diffuse light
    vec3 lightDirection = normalize(v_lightPos - v_fragPos);
    float diffuse = max(dot(normal, lightDirection), 0.0f);

    // specular light
    float strengh = 0.9f;
    vec3 viewDirection = normalize(v_viewPos - v_fragPos);
    vec3 reflectDirection = reflect(-lightDirection, normal);
    float specular = strengh * pow(max(dot(viewDirection, reflectDirection), 0.0f), 256);

    gl_FragColor = (specular + ambient + diffuse) * texture2D(texture, v_textCoord);
}