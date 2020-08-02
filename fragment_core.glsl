#version 450

struct Material
{
    vec3 ambient;
    vec3 diffuse; // diffuse color
    vec3 specular;
    sampler2D diffuseTex;
    sampler2D specularTex;
};

in vec3 vs_position;
in vec3 vs_color;
in vec2 vs_texcoord;
in vec3 vs_normal;

out vec4 fs_color;

uniform Material material;
uniform vec3 lightPos0;
uniform vec3 cameraPos;

// Functions
vec3 calculateAmbient(Material material)
{
    return material.ambient;
}

vec3 calculateDiffuse(Material material, vec3 vs_position, vec3 vs_normal, vec3 lightPos0)
{
    // Diffuse light
    vec3 posToLightDirVec = normalize(lightPos0 - vs_position); // from vertex to the light
    float diffuse = clamp(dot(posToLightDirVec, vs_normal), 0, 1);
    vec3 diffuseFinal = material.diffuse * diffuse; // like a percentage

    return diffuseFinal;
}

vec3 calculateSpecular(Material material, vec3 vs_position, vec3 vs_normal, vec3 lightPos0, vec3 cameraPos)
{
    // Specular light (creates a shiny dot that shows the light being reflected into the camera)
    vec3 lightToPosDirVec = normalize(lightPos0 - vs_position);
    vec3 reflectDirVec = normalize(reflect(lightToPosDirVec, normalize(vs_normal)));
    vec3 posToViewDirVec = normalize(vs_position - cameraPos);
    float specularConstant = pow(max(dot(posToViewDirVec, reflectDirVec), 0), 30); // makes sure value goes from 0 -> inf, power of 30 makes the light very shiny
    vec3 specularFinal = material.specular * specularConstant * texture(material.specularTex, vs_texcoord).rgb;

    return specularFinal;
}

void main()
{
    //fs_color = vec4(vs_color, 1.f); -> this is color data
    // fs_color = texture(texture0, vs_texcoord) + texture(texture1, vs_texcoord); // looks at bound texture unit 

    // Ambient light
    vec3 ambientLight = calculateAmbient(material); // hits object from all around

    vec3 diffuseFinal = calculateDiffuse(material, vs_position, vs_normal, lightPos0);

    // Specular light (creates a shiny dot that shows the light being reflected into the camera)
    vec3 specularFinal = calculateSpecular(material, vs_position, vs_normal, lightPos0, cameraPos);

    // Attenuation

    // Final light
    // fs_color = texture(texture0, vs_texcoord);
    fs_color = texture(material.diffuseTex, vs_texcoord)
     * (vec4(ambientLight, 1.f) + vec4(diffuseFinal, 1.f) + vec4(specularFinal, 1.f));
}