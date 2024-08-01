#include "Material.h"

Material::Material() {
	specularIntensity = 0.0f;
	shineness = 0.0f;

}
Material::Material(GLfloat sIntensity, GLfloat shine) {
	specularIntensity = sIntensity;
	shineness = shine;
}

void Material::UseMaterial(GLuint specualIntensityLocalation, GLuint shinenessLoation) {
	glUniform1f(specualIntensityLocalation, specularIntensity);
	glUniform1f(shinenessLoation, shineness);
}
Material::~Material() {

}
