#shader vertex
#version 330 core

layout(location = 0)in vec4 a_Position;
out vec4 v_Position;

uniform mat4 u_MVP;

void main()
{
	gl_Position = u_MVP * a_Position;
	v_Position =  u_MVP *a_Position;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 colour;

in vec4 v_Position;

float spheres(vec3 z)
{
	z = mod((z),1.0)-vec3(0.5);
	return length(z)-0.1;
}

float Scale = 2.0;
int Iterations = 10;
float Power = 8.0;
float Bailout = 1.15;
float Offset = 1.0;
float min_dist = 0.0000005;
float max_dist = 50.0;

vec4 ray_march(vec3 o, vec3 r, float sharpness) {
	float d = min_dist;
	float s = 0.0;
	float t= 0.0;
	float min_d = 1.0;
	for (; s < 50; s += 1.0) { 
		vec3 p = o + r*t;
		d = spheres(p);
		if ( d < min_dist ) {
			s += d/min_dist;
			break;
		} else if ( t > 50.0) {
			break;
		}
		t += d;
		min_d = min(min_d,sharpness*d/t);
	}
	return vec4(d,s,t,min_d);
};

void main()
{
	//vec2 uv = 2*v_Position.xy-1.0;
	vec2 uv = 0.5*v_Position.xy + 0.5;
	vec3 r = normalize(vec3(uv, 1.0));
		vec3 o = vec3(0.0,0.0,-2.0);
	vec4 e = vec4(min_dist, 0.0,0.0,0.0);
	vec3 n = normalize(vec3(spheres(r + e.xyy) - spheres(r - e.xyy),\
		spheres(r + e.yxy) - spheres(r - e.yxy),\
		spheres(r + e.yyx) - spheres(r - e.yyx)));

	vec3 light_dir = vec3(0.0);

    
    // ambient occlusion
//    vec2 rmvec = trace(o,r);
	vec4 rmvec = ray_march(o,r,4.0);
    float d = rmvec.x;
    float s = rmvec.y;
    float t = rmvec.z;
    float m = rmvec.w;
    float a = 1.0 / (1.0 + t + 0.01);
    vec3 col = vec3(0.0);
    vec3 orig_col = vec3(0.8,0.3,0.5)*255;
    if (d < min_dist) {
    vec3 reflected = r.xyz - 2.0*dot(r.xyz,n)*n;
    
	float k = 2.0;
	vec3 light_pt = vec3(-1.0,-1.0,10.0);
	light_pt.xyz += n*min_dist * 0.01;
	vec4 rm = ray_march(light_pt, light_dir,4.0);
	k = rm.w * min(rm.z,1.0);
	
	
	float specular = max(dot(reflected, light_dir),0.0);
	specular = pow(specular, 16);
	col += specular*k;
	
	k = min(k, 0.8* 0.5 * (dot(n,light_dir) - 1.0) + 1.0);
	k = max(k,0.2);
	col += orig_col*k;
	
    //vec3 col = 0.5 + 0.5*cos(u_Time*0.001+uv.xyx+vec3(0.1*t,0.2*t,0.4*t));
    col += (1.0 - a)*vec3(0.8);
    a = t / max_dist;
    col = (1.0 - a)*col*a*vec3(0.2,0.8,0.5);

    float fog = 1.0/(1.0+t*0.009);
    vec3 fc = vec3(fog,0.5*fog, 0.2*fog);
    } else {
    	col += vec3(0.8,0.3,0.5);
    	//col += (1.0 - m) * (1.0 - m) * vec3(-0.2,0.5,-0.2);
    }
    vec4 fragColour = vec4(col,1.0);
	colour = fragColour;
}