#version 330

// Input
// TODO(student): Get vertex attributes from each location
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texcoord;
layout(location = 3) in vec3 v_color;



// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform vec3 CarPosition;



// Output
// TODO(student): Output values to fragment shader
out vec3 frag_color;
out vec3 frag_normal;
out vec2 frag_texcoord;


void main()
{
    // TODO(student): Send output to fragment shader
	frag_color = v_color;
	frag_normal = v_normal;
	frag_texcoord = v_texcoord;
	

    // TODO(student): Compute gl_Position
	float scaleFactor = 0.018;

	vec3 worldPosition = (Model * vec4(v_position, 1.0)).xyz;
	float new_y = worldPosition.y - pow(length(CarPosition - worldPosition), 2) * scaleFactor;
	vec4 newPosition = vec4(worldPosition.x, new_y, worldPosition.z, 1.0);
	
	gl_Position = Projection * View * newPosition;
	
}
