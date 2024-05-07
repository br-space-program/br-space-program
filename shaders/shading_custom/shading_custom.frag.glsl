#version 330 core


// Inputs coming from the vertex shader
in struct fragment_data
{
    vec3 position; // position in the world space
    vec3 normal;   // normal in the world space
    vec3 color;    // current color on the fragment
    vec2 uv;       // current uv-texture on the fragment

} fragment;

// Output of the fragment shader - output color
layout(location=0) out vec4 FragColor;

// View matrix
uniform mat4 view;

struct phong_parameters {
    float ambient;
    float diffuse;
    float specular;
    float specular_exponent;
};

struct material_structure
{
	vec3 color;  // Uniform color of the object
    phong_parameters phong; // Phong parameters
};
uniform material_structure material;

// Ambiant uniform controled from the GUI
uniform float ambiant;
uniform float diffus;
uniform float speculaire;
uniform float exp_spec;

// uniform vec3 light_color;
// uniform vec3 light_position;


const int nbr_lights = 3;
vec3 light_colors[nbr_lights] = vec3[](vec3(1, 1, 1), vec3(0.4, 0.6, 0.8), vec3(1, 0.3, 0.2));
vec3 light_positions[nbr_lights] = vec3[](vec3(-2, 2, 0.3), vec3(1, -2, 0.2), vec3(-2, 1, 1));

vec3 get_camera_pos() {
    mat3 O = transpose(mat3(view)); // get the orientation matrix
    vec3 last_col = vec3(view * vec4(0.0, 0.0, 0.0, 1.0)); // get the last column
   return -O * last_col;
}


void main()
{
    vec3 camera_pos = get_camera_pos();
    vec3 light_position;
    vec3 light_color;

    vec3 final_color = vec3(0.0, 0.0, 0.0);

    // == Diffusion ==
    for (int i = 0; i < nbr_lights; i++) {
        light_position = light_positions[i];
        light_color = light_colors[i];

        vec3 to_light = normalize(light_position - fragment.position);

        float diffus_color = (material.phong.ambient +
                            material.phong.diffuse
                            * max(dot(normalize(fragment.normal), to_light), 0));

        // == Reflexion ==
        vec3 object_pov = normalize(camera_pos - fragment.position);
        vec3 reflexion = normalize(reflect(-to_light, fragment.normal));

        float spec_color = material.phong.specular
                            * pow(max(dot(object_pov, reflexion), 0), material.phong.specular_exponent);

        // Diffusion + Reflexion
        vec3 color_neutral = diffus_color * material.color * light_color
                                + spec_color * light_color;


        final_color += color_neutral;

    }

	FragColor = vec4(final_color, 1.0);

}