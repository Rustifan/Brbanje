

#type vertex
#version 330 core
				
layout (location = 0) in vec3 position;
				
uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;
				
				

void main()
				
	{
				
		gl_Position = u_ViewProjection * u_Transform * vec4(position, 1.0);
				 
				

	} 

#type fragment
#version 330 core

uniform vec3 u_Color;
out vec4 Color;

void main()
{
	Color = vec4(u_Color,1.0);

}

		

