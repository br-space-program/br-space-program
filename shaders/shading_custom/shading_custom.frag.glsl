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

struct material_structure
{
	vec3 color;  // Uniform color of the object
};
uniform material_structure material;

// Ambiant uniform controled from the GUI
uniform float ambiant;
uniform float diffus;
uniform float speculaire;
uniform float exp_spec;

// uniform vec3 light_color;
// uniform vec3 light_position;

uniform float fog_dist_max;
uniform vec3 fog_color;

uniform float mitigation_dist_max;

const int nbr_lights = 3;
vec3 light_colors[nbr_lights] = vec3[](vec3(0.3, 1, 0.1), vec3(0.4, 0.6, 0.8), vec3(1, 0.3, 0.2));
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

        float diffus_color = (ambiant +
                            diffus
                            * max(dot(normalize(fragment.normal), to_light), 0));

        // == Reflexion ==
        vec3 object_pov = normalize(camera_pos - fragment.position);
        vec3 reflexion = normalize(reflect(-to_light, fragment.normal));

        float spec_color = speculaire
                            * pow(max(dot(object_pov, reflexion), 0), exp_spec);

        // Diffusion + Reflexion
        vec3 color_neutral = diffus_color * material.color * light_color
                                + spec_color * light_color;

        // == Mitigation ==
        float d_l = length(light_position - fragment.position);
        float alpha_l = min(d_l/mitigation_dist_max, 1);

        vec3 color_mitigated = (1 - alpha_l) * color_neutral;

        final_color += color_mitigated;

    }

    // == Fog ==
    float d = length(camera_pos - fragment.position);
    float alpha_f = min(d/fog_dist_max, 1);
    vec3 color_with_fog = (1 - alpha_f) * final_color
                                + alpha_f * fog_color;



	FragColor = vec4(color_with_fog, 1.0);

}