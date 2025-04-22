//example of some shaders compiled
flat basic.vs flat.fs
texture basic.vs texture.fs
skybox basic.vs skybox.fs
depth quad.vs depth.fs
multi basic.vs multi.fs
compute test.cs

\test.cs
#version 430 core

layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
void main() 
{
	vec4 i = vec4(0.0);
}

\basic.vs

#version 330 core

in vec3 a_vertex;
in vec3 a_normal;
in vec2 a_coord;
in vec4 a_color;

uniform vec3 u_camera_pos;

uniform mat4 u_model;
uniform mat4 u_viewprojection;

//this will store the color for the pixel shader
out vec3 v_position;
out vec3 v_world_position;
out vec3 v_normal;
out vec2 v_uv;
out vec4 v_color;

uniform float u_time;

void main()
{	
	//calcule the normal in camera space (the NormalMatrix is like ViewMatrix but without traslation)
	v_normal = (u_model * vec4( a_normal, 0.0) ).xyz;
	
	//calcule the vertex in object space
	v_position = a_vertex;
	v_world_position = (u_model * vec4( v_position, 1.0) ).xyz;
	
	//store the color in the varying var to use it from the pixel shader
	v_color = a_color;

	//store the texture coordinates
	v_uv = a_coord;

	//calcule the position of the vertex using the matrices
	gl_Position = u_viewprojection * vec4( v_world_position, 1.0 );
}

\quad.vs

#version 330 core

in vec3 a_vertex;
in vec2 a_coord;
out vec2 v_uv;

void main()
{	
	v_uv = a_coord;
	gl_Position = vec4( a_vertex, 1.0 );
}


\flat.fs

#version 330 core

uniform vec4 u_color;

out vec4 FragColor;

void main()
{
	FragColor = vec4(0.0, 0.0, 0.0, 1.0);
}


\plain.fs

#version 330 core

out vec4 FragColor;

void main()
{
	FragColor = vec4(0.0, 0.0, 0.0, 1.0);
}

\texture.fs

#version 330 core

in vec3 v_position;
in vec3 v_world_position;
in vec3 v_normal;
in vec2 v_uv;
in vec4 v_color;

uniform vec4 u_color;
uniform sampler2D u_texture;
uniform float u_time;
uniform float u_alpha_cutoff;

uniform vec3 u_light_pos[10];
uniform vec3 u_light_color[10];
uniform float u_light_intensity[10];
uniform int u_light_count;
uniform vec3 u_light_direction[10];
uniform int u_light_type[10];
uniform float u_alpha_max[10];
uniform float u_alpha_min[10];

uniform vec3 u_light_pos1;
uniform vec3 u_light_color1;
uniform float u_light_intensity1;
uniform vec3 u_light_direction1;
uniform int u_light_type1;
uniform float u_alpha_max1;
uniform float u_alpha_min1;
uniform float u_shiny;

uniform bool u_multipass1;
uniform bool u_ambient1;

//Assigment 3
uniform sampler2D u_shadowmap[10];
uniform mat4 u_light_viewprojection[10];
uniform float u_bias;

uniform vec3 u_camera_pos;

out vec4 FragColor;

mat3 cotangent_frame(vec3 N, vec3 P, vec2 uv) {
	vec3 dp1 = dFdx(P);
	vec3 dp2 = dFdy(P);
	vec2 duv1 = dFdx(uv);
	vec2 duv2 = dFdy(uv);
	vec3 dp2perp = cross(dp2, N);
	vec3 dp1perp = cross(N, dp1);
	vec3 T = dp2perp * duv1.x + dp1perp * duv2.x;
	vec3 B = dp2perp * duv1.y + dp1perp * duv2.y;

	float invmax = inversesqrt(max(dot(T, T), dot(B, B)));
	return mat3(T * invmax, B * invmax, N);
}

vec3 perturbNormal(vec3 N,vec3 WP, vec2 uv, vec3 normal_pixel){
	normal_pixel = normal_pixel * 255./127. - 128./127.;
	mat3 TBN = cotangent_frame(N, WP, uv);
	return normalize(TBN * normal_pixel);
}





void main()
{

	if(u_multipass1){
		vec2 uv = v_uv;
		vec4 color = u_color;
		color *= texture( u_texture, v_uv );

		vec3 light_component = vec3(0.0);


		//normals
		vec3 texture_normal = texture(u_texture, v_uv).xyz;

		texture_normal = (texture_normal *2.0) -1.0;

		texture_normal = normalize(texture_normal);

		vec3 normal = perturbNormal(v_normal, v_world_position, v_uv, texture_normal);
		//ambient
			if(u_light_type1 == 1){ //point
				vec3 L = normalize(u_light_pos1 - v_world_position);
				vec3 V = normalize(u_camera_pos - v_world_position);
				vec3 R = reflect(-L, normalize(normal));

				float dist = distance(u_light_pos1, v_world_position);
				float attenuation = 1.0 / pow(dist, 2.0); //Li
				float specular = pow(max(dot(R, V), 0.0), u_shiny); //20 shinines

				float diffuse = dot(L,normal);

				light_component += u_light_intensity1 * (u_light_color1 * diffuse + u_light_color1 * specular) * attenuation; 
			}
		
			if(u_light_type1 == 2){ //spot
				vec3 L = normalize(u_light_pos1 - v_world_position);
				float DandL = dot(L,normalize(u_light_direction1));
				float cos_max = cos(u_alpha_max1);
				float cos_min = cos(u_alpha_min1);
				if(DandL >= cos_max){
					float dist = distance(u_light_pos1, v_world_position);
					float intensity = u_light_intensity1 / (dist * dist);

					float spot_effect = (DandL - cos_max) / (cos_min - cos_max);
					spot_effect = clamp(spot_effect, 0.0, 1.0);

					float Lspot = intensity * spot_effect;

					vec3 V = normalize(u_camera_pos - v_world_position);
					vec3 R = reflect(-L, normalize(normal));
					float specular = pow(max(dot(R, V), 0.0), u_shiny);
					float diffuse = dot(L,normal);

					light_component += Lspot * (u_light_color1 *(diffuse + specular)); 
				}
				else{
					light_component += vec3(0.0,0.0,0.0);
				}
			}
		
			if(u_light_type1 == 3){	
				vec3 L = normalize(-u_light_direction1);
				vec3 V = normalize(u_camera_pos - v_world_position);
				vec3 R = reflect(-L, normalize(normal));
				float diff = max(dot(L, normalize(v_normal)), 0.0);
				float specular = pow(max(dot(R, V), 0.0),u_shiny); //shinines
				light_component += u_light_intensity1 * (u_light_color1*diff + u_light_color1 * specular); 
			}
		
		if(u_ambient1){
			light_component += vec3(0.01,0.01,0.01); //ambient light
		}
		if(color.a < u_alpha_cutoff)
			discard;

		FragColor = color * vec4(light_component,1.0);
	}else{

		
		vec2 uv = v_uv;
		vec4 color = u_color;
		color *= texture( u_texture, v_uv );

		vec3 light_component = vec3(0.0);


		//normals
		vec3 texture_normal = texture(u_texture, v_uv).xyz;

		texture_normal = (texture_normal *2.0) -1.0;

		texture_normal = normalize(texture_normal);

		vec3 normal = perturbNormal(v_normal, v_world_position, v_uv, texture_normal);
		//ambient
		for(int i = 0; i<u_light_count;i++){ //I dont unerstand why +4 make it work, it is needed to render all lights for some reason
			
			if(u_light_type[i] == 1){ //point
				vec3 L = normalize(u_light_pos[i] - v_world_position);
				vec3 V = normalize(u_camera_pos - v_world_position);
				vec3 R = reflect(-L, normalize(normal));

				float dist = distance(u_light_pos[i], v_world_position);
				float attenuation = 1.0 / pow(dist, 2.0); //Li
				float specular = pow(max(dot(R, V), 0.0), u_shiny); //20 shinines

				float diffuse = dot(L,normal);

				light_component += u_light_intensity[i] * (u_light_color[i] * diffuse + u_light_color[i] * specular) * attenuation; 
			}
			if(u_light_type[i] != 1){
				//projected position
				vec4 proj_pos = u_light_viewprojection[i] * vec4(v_world_position, 1.0);
				proj_pos.z = proj_pos.z - u_bias;
				proj_pos = proj_pos /proj_pos.w; //clip space

				vec2 shadow_uv = proj_pos.xy * 0.5 + 0.5;

				float depth = proj_pos.z * 0.5 + 0.5;;

					float depth_shadowmap = texture(u_shadowmap[i],shadow_uv).r;


					if (depth > depth_shadowmap){ //has to be > we want to know if is being hit by light
						if(u_light_type[i] == 2){ //spot
							vec3 L = normalize(u_light_pos[i] - v_world_position);
							float DandL = dot(L,normalize(u_light_direction[i]));
							float cos_max = cos(u_alpha_max[i]);
							float cos_min = cos(u_alpha_min[i]);
							if(DandL >= cos_max){
								float dist = distance(u_light_pos[i], v_world_position);
								float intensity = u_light_intensity[i] / (dist * dist);

								float spot_effect = (DandL - cos_max) / (cos_min - cos_max);
								spot_effect = clamp(spot_effect, 0.0, 1.0);

								float Lspot = intensity * spot_effect;

								vec3 V = normalize(u_camera_pos - v_world_position);								vec3 R = reflect(-L, normalize(normal));
								float specular = pow(max(dot(R, V), 0.0), u_shiny);
								float diffuse = dot(L,normal);

								light_component += Lspot * (u_light_color[i] *(diffuse + specular));
							}
							else{
								light_component += vec3(0.0,0.0,0.0);
							}
						}
		
							if(u_light_type[i] == 3){	
								vec3 L = normalize(-u_light_direction[i]);
								vec3 V = normalize(u_camera_pos - v_world_position);
								vec3 R = reflect(-L, normalize(normal));
								float diff = max(dot(L, normalize(v_normal)), 0.0);
								float specular = pow(max(dot(R, V), 0.0),u_shiny); //shinines
								light_component += u_light_intensity[i] * (u_light_color[i]*diff + u_light_color[i] * specular); 
							}
				
				}
			}


		}
		light_component += vec3(0.01,0.01,0.01); //ambient light
		if(color.a < u_alpha_cutoff)
			discard;

		FragColor = (color * vec4(light_component,1.0));
	}
	
}



\textureDirectionLight.fs

#version 330 core

in vec3 v_position;
in vec3 v_world_position;
in vec3 v_normal;
in vec2 v_uv;
in vec4 v_color;

uniform vec4 u_color;
uniform sampler2D u_texture;
uniform float u_time;
uniform float u_alpha_cutoff;

uniform vec3 u_light_pos[10];
uniform vec3 u_light_color[10];
uniform float u_light_intensity[10];
uniform int u_light_count;
uniform vec3 u_light_direction[10];

uniform vec3 u_camera_pos;

out vec4 FragColor;

//directional lights
void main()
{
	vec2 uv = v_uv;
	vec4 color = u_color;
	color *= texture( u_texture, v_uv );

	vec3 light_component = vec3(0.0);
	//ambient
	for(int i = 0; i<u_light_count+1;i++){
		vec3 L = normalize(-u_light_direction[i]);
		vec3 V = normalize(u_camera_pos - v_world_position);
		vec3 R = reflect(-L, normalize(v_normal));

		float diff = max(dot(L, normalize(v_normal)), 0.0);
		float specular = pow(max(dot(R, V), 0.0), 22.0); //shinines

		light_component += u_light_intensity[i] * u_light_color[i]*diff + u_light_color[i] * specular; 
	}
	light_component += u_light_color[0]*vec3(0.01,0.01,0.01); //ambient light
	if(color.a < u_alpha_cutoff)
		discard;

	FragColor = color * vec4(light_component,1.0);
}
\skybox.fs

#version 330 core

in vec3 v_position;
in vec3 v_world_position;

uniform samplerCube u_texture;
uniform vec3 u_camera_position;
out vec4 FragColor;

void main()
{
	vec3 E = v_world_position - u_camera_position;
	vec4 color = texture( u_texture, E );
	FragColor = color;
}


\multi.fs

#version 330 core

in vec3 v_position;
in vec3 v_world_position;
in vec3 v_normal;
in vec2 v_uv;

uniform vec4 u_color;
uniform sampler2D u_texture;
uniform float u_time;
uniform float u_alpha_cutoff;

layout(location = 0) out vec4 FragColor;
layout(location = 1) out vec4 NormalColor;

void main()
{
	vec2 uv = v_uv;
	vec4 color = u_color;
	color *= texture( u_texture, uv );

	if(color.a < u_alpha_cutoff)
		discard;

	vec3 N = normalize(v_normal);

	FragColor = color;
	NormalColor = vec4(N,1.0);
}


\depth.fs

#version 330 core

uniform vec2 u_camera_nearfar;
uniform sampler2D u_texture; //depth map
in vec2 v_uv;
out vec4 FragColor;

void main()
{
	float n = u_camera_nearfar.x;
	float f = u_camera_nearfar.y;
	float z = texture2D(u_texture,v_uv).x;
	if( n == 0.0 && f == 1.0 )
		FragColor = vec4(z);
	else
		FragColor = vec4( n * (z + 1.0) / (f + n - z * (f - n)) );
}


\instanced.vs

#version 330 core

in vec3 a_vertex;
in vec3 a_normal;
in vec2 a_coord;

in mat4 u_model;



uniform mat4 u_viewprojection;

//this will store the color for the pixel shader
out vec3 v_position;
out vec3 v_world_position;
out vec3 v_normal;
out vec2 v_uv;

void main()
{	
	//calcule the normal in camera space (the NormalMatrix is like ViewMatrix but without traslation)
	v_normal = (u_model * vec4( a_normal, 0.0) ).xyz;
	
	//calcule the vertex in object space
	v_position = a_vertex;
	v_world_position = (u_model * vec4( a_vertex, 1.0) ).xyz;
	
	//store the texture coordinates
	v_uv = a_coord;

	//calcule the position of the vertex using the matrices
	gl_Position = u_viewprojection * vec4( v_world_position, 1.0 );
}