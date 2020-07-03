#version 460 core

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

uniform mat4 u_Model;
uniform mat4 u_ViewProjection;

in vec2 v_Pos[];
in vec2 v_Size[];
in float v_Rot[];

in vec4 v_Color[];
out vec4 vf_Color;

void CreateVertex(vec2 pos, vec2 corner, float rotation)
{
	vec2 cornerRot;

	cornerRot.x = corner.x * cos(rotation) - corner.y * sin(rotation);
	cornerRot.y = corner.x * sin(rotation) + corner.y * cos(rotation);

	gl_Position = u_ViewProjection * vec4(cornerRot + pos, 0.0, 1.0);
	EmitVertex();
}
void main()
{
	// Forward to fragment shader
	vf_Color = v_Color[0];
	//

	vec2 size = v_Size[0];
	vec2 pos = v_Pos[0];

	CreateVertex(pos, size * vec2(-0.5,  0.5), v_Rot[0]);
	CreateVertex(pos, size * vec2(-0.5, -0.5), v_Rot[0]);
	CreateVertex(pos, size * vec2( 0.5,  0.5), v_Rot[0]);
	CreateVertex(pos, size * vec2( 0.5, -0.5), v_Rot[0]);

	EndPrimitive();
}
