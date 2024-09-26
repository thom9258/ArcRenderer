#version 330 core

struct Material {
	vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

out vec4 out_color;

in vec3 f_normal;
in vec3 f_pos;

uniform vec3 view_pos;
uniform Material material;
uniform Light light;

void main()
{
	// ambient
	vec3 ambient = light.ambient * material.ambient;
	
	// diffuse
	vec3 norm = normalize(f_normal);
	vec3 light_dir = normalize(light.position - f_pos);
	float diff = max(dot(norm, light_dir), 0.0);
	vec3 diffuse = light.diffuse * (diff * material.diffuse);
	
	// specular
	vec3 view_dir = normalize(view_pos - f_pos);
	vec3 reflect_dir = reflect(-light_dir, norm);
	float spec = pow(max(dot(view_dir, reflect_dir), 0.0), 32);
	vec3 specular = light.specular * (material.specular * spec);
	
	vec3 phong = (ambient + diffuse + specular);
    out_color = vec4(phong, 1.0);
} 
