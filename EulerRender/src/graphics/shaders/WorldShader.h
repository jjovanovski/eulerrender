#pragma once

const char* WORLD_SHADER_VERTEX = "#version 330 core \n"
""
"layout (location = 0) in vec3 in_pos;"
"layout (location = 1) in vec3 in_normal;"
"layout (location = 2) in vec2 in_uv;"
"layout (location = 3) in vec3 in_tangent;"
""
"uniform mat4 proj, view, model;"
""
"out vec3 fragmentPos;"
"out mat3 TBN;"
"out vec2 fragmentUV;"
""
"void main() {"
"	vec4 posInWorld = model * vec4(in_pos, 1.0);"
"	gl_Position = proj * view * posInWorld;"
""
"	mat3 normalMat = mat3(transpose(inverse(model)));"
""
"	vec3 normal = normalize(normalMat * in_normal);"
"	vec3 tangent = normalize(normalMat * in_tangent);"
"	vec3 bitangent = normalize(cross(normal, tangent));"
""
"	tangent = tangent * -1;"
""
"	fragmentPos = vec3(posInWorld);"
"	TBN = mat3(tangent, bitangent, normal);"
"	fragmentUV = in_uv;"
"}"
"";

const char * WORLD_SHADER_FRAGMENT = "#version 330 core \n"
""
"in vec3 fragmentPos;"
"in mat3 TBN;"
"in vec2 fragmentUV;"
""
"struct Material {"
"	vec3 diffuse;"
"	vec3 specular;"
"	float shininess;"
"};"
"uniform Material material;"
""
"struct DirectionalLight {"
"	vec3 direction;"
"	vec3 color;"
"	float intensity;"
"};"
"uniform DirectionalLight directionalLight;"
""
"uniform vec3 cameraPos;"
""
"uniform sampler2D diffuseTex;"
"uniform sampler2D normalTex;"
"uniform int useTexture;"
"uniform int useNormalMap;"
""
"out vec4 fragColor;"
""
"vec3 calcDirectionalLight(DirectionalLight light, vec3 normal, vec3 cameraDir);"
""
"void main() {"
"	vec3 normal;"
"	if (useNormalMap > 0)"
"		normal = normalize(TBN * (vec3(texture(normalTex, fragmentUV)) * 2.0 - 1.0));"
"	else"
"		normal = TBN[2];"
""
"	vec3 cameraDir = normalize(cameraPos - fragmentPos);"
""
"	vec4 textureColor = vec4(1.0);"
"	if (useTexture > 0)"
"		textureColor = texture(diffuseTex, fragmentUV);"
""
"	vec3 totalLight = calcDirectionalLight(directionalLight, normal, cameraDir);"
""
"	fragColor = vec4(totalLight, 1.0) * textureColor;"
"}"
""
"vec3 calcDirectionalLight(DirectionalLight light, vec3 normal, vec3 cameraDir) {"
"	vec3 ambient = light.color * (material.diffuse * 0.1);"
""
"	float diffuseIntensity = max(dot(-light.direction, normal), 0);"
"	vec3 diffuse = (light.color * diffuseIntensity * light.intensity) * material.diffuse;"
""
"	vec3 reflectDirection = normalize(reflect(light.direction, normal));"
"	float specularIntensity = pow(max(dot(reflectDirection, cameraDir), 0.0), material.shininess);"
"	vec3 specular = (light.color * specularIntensity * light.intensity) * material.specular;"
""
"	return ambient + diffuse + specular;"
"}";