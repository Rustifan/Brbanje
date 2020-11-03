

#type vertex
#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec4 color;
layout (location = 2) in vec2 textureCoords;
layout (location = 3) in float textureSlot;
layout (location = 4) in float tilingFactor;

out vec2 v_TexCoords;
out vec4 v_Color;
flat out float v_TexSlot;
out float v_TilingFactor;

uniform mat4 u_ViewProjection;

				
				

void main()
				
{
			
	
	
	v_TexCoords = textureCoords;
	v_Color = color;
	v_TexSlot = textureSlot;
	v_TilingFactor = tilingFactor;

	gl_Position = u_ViewProjection * vec4(position, 1.0);

}
	
	

#type fragment
#version 330 core

		
uniform sampler2D sampler[32];


in vec4 v_Color;		
in vec2 v_TexCoords;
flat in float v_TexSlot;
in float v_TilingFactor;

layout (location = 0) out vec4 Color;



void main()
{
	
	Color = vec4(0,0.5,0,1);

	for(int i = 0; i < 32; i++)
	{
		if(i == int(v_TexSlot))
		{
			Color = texture(sampler[i], v_TexCoords * v_TilingFactor) * v_Color;
		}
	}

}
