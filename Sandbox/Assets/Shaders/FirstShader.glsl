

#type vertex
#version 330 core
				
layout (location = 0) in vec3 position;
layout (location = 1 ) in vec4 color;
				
uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

out vec4 fragColor;

void main()
{
				
	gl_Position = u_ViewProjection * u_Transform * vec4(position, 1.0);
				 
	fragColor = color;

}


#type fragment
#version 330 core
in vec4 fragColor;
out vec4 Color;

void main()
{
	
	Color = fragColor;

}
			