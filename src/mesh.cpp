#include "mesh.h"

Mesh::Mesh(vector<Vertex> _vertices, vector<unsigned int> _indices, vector<Texture> _textures, bool _isFace, bool _isHair) {
	vertices = _vertices;
	indices = _indices;
	textures = _textures;
    isFace = _isFace;
    isHair = _isHair;

	setupMesh();
}

void Mesh::setupMesh() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
        &indices[0], GL_STATIC_DRAW);

    // 顶点位置
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // 顶点法线
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    // 顶点纹理坐标
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texcoords));
    
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));

    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));

    glBindVertexArray(0);
}

void Mesh::draw(nanogui::GLShader shader) {
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr = 1;
    unsigned int emissionNr = 1;
    for (unsigned int i = 0; i < textures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        string number;
        string name = textures[i].type;
        if (name == "texture_diffuse") {
            number = std::to_string(diffuseNr);
            diffuseNr++;
        }
        else if (name == "texture_specular") {
            number = std::to_string(specularNr);
            specularNr++;
        }
        else if (name == "texture_normal") {
            number = std::to_string(normalNr);
            normalNr++;
        }
        else if (name == "ilmTexture") {
            number = std::to_string(emissionNr);
            emissionNr++;
        }
        glUniform1i(glGetUniformLocation(shader.getShaderID(), (name + number).c_str()), i);
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }
    glActiveTexture(GL_TEXTURE0);

    //if (shader.uniform("bump", true)) shader.setUniform("bump", 1);
    //else shader.setUniform("bump", 0);
    //if (specularNr > 1) shader.setUniform("sepmap", 1);
    //else shader.setUniform("sepmap", 0);
        // 绘制网格
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

}

void Mesh::drawHairShadow(nanogui::GLShader shader) {
    //unsigned int diffuseNr = 1;
    //unsigned int specularNr = 1;
    //unsigned int normalNr = 1;
    //unsigned int emissionNr = 1;
    //for (unsigned int i = 0; i < textures.size(); i++) {
    //    glActiveTexture(GL_TEXTURE0 + i);
    //    string number;
    //    string name = textures[i].type;
    //    if (name == "texture_diffuse") {
    //        number = std::to_string(diffuseNr);
    //        diffuseNr++;
    //    }
    //    else if (name == "texture_specular") {
    //        number = std::to_string(specularNr);
    //        specularNr++;
    //    }
    //    else if (name == "texture_normal") {
    //        number = std::to_string(normalNr);
    //        normalNr++;
    //    }
    //    else if (name == "texture_emission") {
    //        number = std::to_string(emissionNr);
    //        emissionNr++;
    //    }
    //    glUniform1i(glGetUniformLocation(shader.getShaderID(), (name + number).c_str()), i);
    //    glBindTexture(GL_TEXTURE_2D, textures[i].id);
    //}
    //glActiveTexture(GL_TEXTURE0);
    if (isHair) {
        //shader.setUniform("color", vec3(1.0));
        // 绘制网格
        glBindVertexArray(VAO);
        //glStencilFunc(GL_EQUAL, 1, 0xFF);
        //glStencilMask(0xFF);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
    //else {
    //    shader.setUniform("color", vec3(0.0));
    //}
    //// 绘制网格
    //glBindVertexArray(VAO);
    ////glStencilFunc(GL_EQUAL, 1, 0xFF);
    ////glStencilMask(0xFF);
    //glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    //glBindVertexArray(0);
}

void Mesh::drawFaceStencil(nanogui::GLShader& shader) {
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr = 1;
    unsigned int emissionNr = 1;
    for (unsigned int i = 0; i < textures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        string number;
        string name = textures[i].type;
        if (name == "texture_diffuse") {
            number = std::to_string(diffuseNr);
            diffuseNr++;
        }
        else if (name == "texture_specular") {
            number = std::to_string(specularNr);
            specularNr++;
        }
        else if (name == "texture_normal") {
            number = std::to_string(normalNr);
            normalNr++;
        }
        else if (name == "ilmTexture") {
            number = std::to_string(emissionNr);
            emissionNr++;
        }
        glUniform1i(glGetUniformLocation(shader.getShaderID(), (name + number).c_str()), i);
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }
    glActiveTexture(GL_TEXTURE0);

    if (isFace) {
        glStencilFunc(GL_ALWAYS, 128, 0xFF);
        glStencilMask(0xFF);
        shader.setUniform("_IsFace", 1.0);
    }
    else {
        //glStencilMask(0x00);
        glStencilFunc(GL_ALWAYS, 0, 0xFF);
        glStencilMask(0xFF);
        shader.setUniform("_IsFace", 0.0);
    }
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Mesh::drawHairShadowStencil(nanogui::GLShader& shader) {
    glStencilFunc(GL_EQUAL, 128, 0xFF);
    //glStencilMask(0x00);
    glStencilMask(0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glDepthMask(GL_FALSE);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
    glDepthMask(GL_TRUE);
}

void Mesh::drwaFaceAfterSampleHairShadowStencil(nanogui::GLShader& shader) {
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr = 1;
    unsigned int emissionNr = 1;
    for (unsigned int i = 0; i < textures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        string number;
        string name = textures[i].type;
        if (name == "texture_diffuse") {
            number = std::to_string(diffuseNr);
            diffuseNr++;
        }
        else if (name == "texture_specular") {
            number = std::to_string(specularNr);
            specularNr++;
        }
        else if (name == "texture_normal") {
            number = std::to_string(normalNr);
            normalNr++;
        }
        else if (name == "ilmTexture") {
            number = std::to_string(emissionNr);
            emissionNr++;
        }
        glUniform1i(glGetUniformLocation(shader.getShaderID(), (name + number).c_str()), i);
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }
    glActiveTexture(GL_TEXTURE0);

    glStencilFunc(GL_EQUAL, 129, 0xFF);
    glStencilMask(0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
    //glDisable(GL_STENCIL_TEST);
    glDepthFunc(GL_LEQUAL);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS);
}