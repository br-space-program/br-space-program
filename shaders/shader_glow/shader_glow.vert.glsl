#version 330 core

// Vertex shader - this code is executed for every vertex of the shape

// Inputs coming from VBOs
layout (location = 0) in vec3 vertex_position; // vertex position in local space (x,y,z)
layout (location = 1) in vec3 vertex_normal;   // vertex normal in local space   (nx,ny,nz)
layout (location = 2) in vec3 vertex_color;    // vertex color      (r,g,b)
layout (location = 3) in vec2 vertex_uv;       // vertex uv-texture (u,v)

// Output variables sent to the fragment shader
out struct fragment_data
{
    vec3 position; // vertex position in world space
    vec3 normal;   // normal position in world space
    vec3 color;    // vertex color
    vec2 uv;       // vertex uv
    float glow_transparency; // For the glow effect
} fragment;

// Uniform variables expected to receive from the C++ program
uniform mat4 model; // Model affine transform matrix associated to the current shape
uniform mat4 view;  // View matrix (rigid transform) of the camera
uniform mat4 projection; // Projection (perspective or orthogonal) matrix of the camera




void main()
{
	// The position of the vertex in the world space
	vec4 position = model * vec4(vertex_position, 1.0);

	// The normal of the vertex in the world space
	mat4 modelNormal = transpose(inverse(model));
	vec4 normal = modelNormal * vec4(vertex_normal, 0.0);

    vec4 offset = normal * 0.001f;
	// The projected position of the vertex in the normalized device coordinates:
	vec4 position_projected = projection * view * (position);

	// The glow should be more prominent the closer to the object you are and get fainter and fainter the further away you are.
	// Instead of using proximity to the object, we can use the angle between the viewing vector and the normal of the triangle that is being rendered.
	// This way the glow becomes fainter when the surface of the object is less aimed at the viewer.

	// This effect can easily be calculated by using the dot-product of the normalized viewing vector and triangle normal
	// (note that Face Culling is enabled in this situation, which means that triangles that are pointing away from the camera are not rendered).
		// Compute the position of the center of the camera
	mat3 O = transpose(mat3(view));                   // get the orientation matrix
	vec3 last_col = vec3(view*vec4(0.0, 0.0, 0.0, 1.0)); // get the last column
	vec3 camera_position = -O*last_col;

	vec3 camera_to_vertex = camera_position - position.xyz;

    float glow_transparency = dot(normalize(normal.xyz), normalize(camera_to_vertex));

	// Fill the parameters sent to the fragment shader
	fragment.position = position.xyz;
	fragment.normal   = normal.xyz;
	fragment.color = vertex_color;
	fragment.uv = vertex_uv;
    fragment.glow_transparency = glow_transparency;


	// gl_Position is a built-in variable which is the expected output of the vertex shader
	gl_Position = position_projected; // gl_Position is the projected vertex position (in normalized device coordinates)
}
