#include "canvas.h"

const string diffuseVertexShader =
"#version 330 core\n"
"layout(location = 0) in vec3 aPos;\n"
"layout(location = 1) in vec3 aNormal;\n"
"layout(location = 2) in vec2 aTexCoords;\n"
"out vec2 TexCoords;\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"void main(){\n"
"   TexCoords = aTexCoords;\n"
"   gl_Position =  projection * view * model * vec4(aPos, 1.0);\n"
"}\n";

const string diffuseFragShader =
"#version 330 core\n"
"out vec4 FragColor;\n"
"in vec2 TexCoords;\n"
"uniform sampler2D texture_diffuse1;\n"
"void main(){\n"
"   FragColor = texture(texture_diffuse1, TexCoords);\n"
"}\n";

const string specularVertexShader =
"#version 330 core\n"
"void main() {\n"
"}\n";

const string specularFragShader =
"#version 330 core\n"
"void main() {\n"
"}\n";

const string phongVertexShader =
"#version 330 core\n"
"layout(location = 0) in vec3 aPos;\n"
"layout(location = 1) in vec3 aNormal;\n"
"layout(location = 2) in vec2 aTexCoords;\n"
"out vec2 TexCoords;\n"
"out vec3 FragPos;\n"
"out vec3 Normal;\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"void main() {\n"
"   FragPos = vec3(model * vec4(aPos, 1.0));\n"
"   Normal = normalize(mat3(transpose(inverse(model))) * aNormal);\n"
"   TexCoords = aTexCoords;\n"
"   gl_Position = projection * view * vec4(FragPos, 1.0);\n"
"}\n";

const string phongFragShader =
"#version 330 core\n"
"out vec4 FragColor;\n"
"in vec2 TexCoords;\n"
"in vec3 Normal;\n"
"in vec3 FragPos;\n"
"uniform vec3 lightColor;\n"
"uniform vec3 lightPos;\n"
"uniform vec3 viewPos;\n"
"uniform sampler2D texture_diffuse1;\n"
"void main(){\n"
"   vec4 kd = texture(texture_diffuse1, TexCoords);\n"
"   float ambientStrength = 0.1;\n"
"   vec3 ambient = ambientStrength * lightColor;\n"
"   vec3 lightDir = normalize(lightPos - FragPos);\n"
"   float diff = max(dot(Normal, lightDir), 0.0);\n"
"   vec3 diffuse = diff * lightColor;\n"
"   float specularStrength = 0.5;\n"
"   vec3 viewDir = normalize(viewPos - FragPos);\n"
"   vec3 reflectDir = reflect(-lightDir, Normal);\n"
"   float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);\n"
"   vec3 specular = specularStrength * spec * lightColor;\n"
"   FragColor = vec4(ambient + diffuse + specular, 1.0) * kd;\n"
"}\n";

const string blinnPhongVertexShader =
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aNormal;\n"
"layout(location = 2) in vec2 aTexCoords;\n"
"out vec3 FragPos;\n"
"out vec3 Normal;\n"
"out vec2 TexCoords;\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"void main() {\n"
"   FragPos = vec3(model * vec4(aPos, 1.0));\n"
"   Normal = normalize(mat3(transpose(inverse(model))) * aNormal);\n"
"   TexCoords = aTexCoords;\n"
"   gl_Position = projection * view * vec4(FragPos, 1.0);\n"
"}\n";

const string blinnPhongFragShader =
"#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 Normal;\n"
"in vec3 FragPos;\n"
"in vec2 TexCoords;\n"
"uniform vec3 ks;\n"
"uniform vec3 ka;\n"
"uniform vec3 intensity;\n"
"uniform vec3 ambLightIntensity;\n"
"uniform vec3 lightPos;\n"
"uniform vec3 viewPos;\n"
"uniform sampler2D texture_diffuse1;\n"
"void main() {\n"
"   vec3 kd = texture(texture_diffuse1, TexCoords).rgb;\n"
"   vec3 viewDir = viewPos - FragPos;\n"
"   vec3 lightDir = lightPos - FragPos;\n"
"   vec3 l = normalize(lightDir);\n"
"   vec3 v = normalize(viewDir);\n"
"   vec3 half = normalize(v + l);\n"
"   vec3 realIntensity = intensity / dot(lightDir, lightDir);\n"
"   vec3 ambient = ka * ambLightIntensity;\n"
"   vec3 diffuse = kd * realIntensity * max(dot(Normal, l), 0);\n"
"   vec3 specular = ks * realIntensity * pow(max(dot(Normal, half), 0), 150);\n"
"   vec3 result = ambient + diffuse + specular;\n"
"   FragColor = vec4(result, 1.0);\n"
"}\n";


MyGLCanvas::MyGLCanvas(Widget* parent, Camera* _camera) : 
    nanogui::GLCanvas(parent), camera(_camera), untitleModel(1), untitleLight(1) {
    using namespace nanogui;

    //modelList.emplace_back(new Model("..\\models\\sara\\sara.obj", "Model 1"));

    GLShader diffuseShder;
    diffuseShder.init("diffuse_shader", diffuseVertexShader, diffuseFragShader);
    diffuseShder.bind();
    shaderList.emplace_back(diffuseShder);

    GLShader phongShader;
    phongShader.init("phong_shader", phongVertexShader, phongFragShader);
    phongShader.bind();
    phongShader.setUniform("viewPos", camera->position);
    phongShader.setUniform("lightPos", vec3(2.5, 2.5, 2.5));
    phongShader.setUniform("lightColor", vec3(1.0, 1.0, 1.0));
    shaderList.emplace_back(phongShader);

    GLShader blinnPhongShder;
    blinnPhongShder.init("blinn_phong_shader", blinnPhongVertexShader, blinnPhongFragShader);
    blinnPhongShder.bind();
    blinnPhongShder.setUniform("lightPos", vec3(2.5));
    blinnPhongShder.setUniform("viewPos", camera->position);
    blinnPhongShder.setUniform("intensity", vec3(100.f));
    blinnPhongShder.setUniform("ka", vec3(0.005));
    blinnPhongShder.setUniform("ambLightIntensity", vec3(10.f));
    blinnPhongShder.setUniform("ks", vec3(0.f));
    shaderList.emplace_back(blinnPhongShder);

    //lightShader.init(
    //    "light_shader",

    //    "#version 330 core\n"
    //    "in vec3 position;\n"
    //    "uniform mat4 lightMVP;\n"
    //    "void main() {\n"
    //    "   gl_Position = lightMVP * vec4(position, 1.0);\n"
    //    "}\n",

    //    "#version 330 core\n"
    //    "out vec4 FragColor;\n"

    //    "void main() {\n"
    //    "   FragColor = vec4(1.0);\n"
    //    "}\n"
    //);

    //MatrixXu indices(3, 12); /* Draw a cube */
    //indices.col(0) << 0, 1, 3;
    //indices.col(1) << 3, 2, 1;
    //indices.col(2) << 3, 2, 6;
    //indices.col(3) << 6, 7, 3;
    //indices.col(4) << 7, 6, 5;
    //indices.col(5) << 5, 4, 7;
    //indices.col(6) << 4, 5, 1;
    //indices.col(7) << 1, 0, 4;
    //indices.col(8) << 4, 0, 3;
    //indices.col(9) << 3, 7, 4;
    //indices.col(10) << 5, 6, 2;
    //indices.col(11) << 2, 1, 5;

    //MatrixXf positions(3, 8);
    //positions.col(0) << 2, 3, 3;
    //positions.col(1) << 2, 3, 2;
    //positions.col(2) << 3, 3, 2;
    //positions.col(3) << 3, 3, 3;
    //positions.col(4) << 2, 2, 3;
    //positions.col(5) << 2, 2, 2;
    //positions.col(6) << 3, 2, 2;
    //positions.col(7) << 3, 2, 3;

    //MatrixXf colors(3, 12);
    //colors.col(0) << 1, 0, 0;
    //colors.col(1) << 0, 1, 0;
    //colors.col(2) << 1, 1, 0;
    //colors.col(3) << 0, 0, 1;
    //colors.col(4) << 1, 0, 1;
    //colors.col(5) << 0, 1, 1;
    //colors.col(6) << 1, 1, 1;
    //colors.col(7) << 0.5, 0.5, 0.5;
    //colors.col(8) << 1, 0, 0.5;
    //colors.col(9) << 1, 0.5, 0;
    //colors.col(10) << 0.5, 1, 0;
    //colors.col(11) << 0.5, 1, 0.5;
    

    //camera->cameraPos = vec3(0.0f, 0.0f, 3.0f);
    //camera->cameraFront = vec3(0.0f, 0.0f, -1.0f);
    //camera->cameraUp = vec3(0.0f, 1.0f, 0.0f);
    //camera->cameraTarget = vec3(0, 0, 0);
    //camera->translateSpeed = 0.01;
    //camera->scaleSpeed = 0.15;
    //camera->sensitivity = 0.2;

    //translate = vec3(0.f);
    //scale = vec3(1.f);
    //model = mat4(1.0f);
    //model = glm::translate(model, vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
    //mShader.bind();
 /*   mShader.uploadIndices(indices);
    mShader.uploadAttrib("position", positions);
    mShader.uploadAttrib("color", colors);*/

    //lightShader.bind();
    //lightShader.uploadIndices(indices);
    //lightShader.uploadAttrib("position", positions);
}

void MyGLCanvas::drawGL() {
    using namespace nanogui;

    //mShader.bind();
    //shaderList[0].bind();

    if (modelList.size() == 0) return;
    updateCamera();
    //std::cout << camera->position.x << " " << camera->position.y << " " << camera->position.z << std::endl;
    //std::cout << camera->target.x << " " << camera->target.y << " " << camera->target.z << std::endl;
    //std::cout << std::endl;
    view = lookAt(camera->position, camera->target, camera->up);
    projection = glm::perspective(camera->fov, camera->aspect, 0.1f, 100.0f);
    //view = lookAt(vec3(0.0f, 0.0f, 3.f), vec3(0.f, 0.f, 2.f), vec3(0, 1.f, 0));
    //projection = glm::perspective(radians(camera->fov), 1.0f, 0.1f, 100.0f);
    //projection = glm::perspective(radians(45.f), 1.0f, 0.1f, 100.0f);

    //model = glm::translate(model, translate);
    //model = glm::scale(model, scale);
    //mvp = projection * view * model;

    //mShader.setUniform("model", model);
    //mShader.setUniform("view", view);
    //mShader.setUniform("projection", projection);
    for (int i = 0; i < modelList.size(); i++) {
        model[0][0] = 1.0f;
        model[0][1] = 0.0f;
        model[0][2] = 0.0f;
        model[0][3] = 0.0f;
        model[1][0] = 0.0f;
        model[1][1] = 1.0f;
        model[1][2] = 0.0f;
        model[1][3] = 0.0f;
        model[2][0] = 0.0f;
        model[2][1] = 0.0f;
        model[2][2] = 1.0f;
        model[2][3] = 0.0f;
        model[3][0] = 0.0f;
        model[3][1] = 0.0f;
        model[3][2] = 0.0f;
        model[3][3] = 1.0f;
        model = glm::translate(model, modelList[i]->translate);
        model = glm::scale(model, modelList[i]->scale);
        shaderList[modelList[i]->shaderIndex].bind();
        shaderList[modelList[i]->shaderIndex].setUniform("model", model);
        shaderList[modelList[i]->shaderIndex].setUniform("view", view);
        shaderList[modelList[i]->shaderIndex].setUniform("projection", projection);
        glEnable(GL_DEPTH_TEST);
        //modelList[i]->draw(mShader);
        modelList[i]->draw(shaderList[modelList[i]->shaderIndex]);
        glDisable(GL_DEPTH_TEST);
    }
    //lightShader.bind();
    //lightShader.setUniform("lightMVP", projection * view);
    //glEnable(GL_DEPTH_TEST);
    /* Draw 12 triangles starting at index 0 */
    //mShader.drawIndexed(GL_TRIANGLES, 0, 12);
    //lightShader.drawIndexed(GL_TRIANGLES, 0, 12);
    //glDisable(GL_DEPTH_TEST);
}

string MyGLCanvas::addModel(const string& path, const string& name) {
    string id;
    if (name == "" || name == "untitle") {
        id = "untitle." + untitleModel;
        modelList.emplace_back(new Model(path, id));
        untitleModel++;
    }
    else {
        int num = 0;
        string realname = name.substr(0, name.find_last_of('.'));
        for (int i = 0; i < modelList.size(); i++) {
            if (modelList[i]->name.substr(0, modelList[i]->name.find_last_of('.')) == realname) {
                num++;
            }
        }
        if (num > 0) {
            id = realname + "." + std::to_string(num);
            modelList.emplace_back(new Model(path, id));
        }
        else {
            id = name;
            modelList.emplace_back(new Model(path, name));
        }
    }
    return id;
}

string MyGLCanvas::addLight(const string& name, int type, vec3 _position, vec3 _color, vec3 _intensity) {
    //if (type < 0 || type > lightShaderList.size()) type = 0;
    string id;
    if (name == "" || name == "untitle") {
        id = "untitle." + untitleLight;
        lightList.emplace_back(new Light(id, type, _position, _color, _intensity));
        untitleLight++;
    }
    else {
        int num = 0;
        string realname = name.substr(0, name.find_last_of('.'));
        for (int i = 0; i < lightList.size(); i++) {
            if (lightList[i]->name.substr(0, lightList[i]->name.find_last_of('.')) == realname) {
                num++;
            }
        }
        if (num > 0) {
            id = realname + "." + std::to_string(num);
            lightList.emplace_back(new Light(id, type, _position, _color, _intensity));
        }
        else {
            id = name;
            lightList.emplace_back(new Light(name, type, _position, _color, _intensity));
        }
    }
    return id;
}

Light* MyGLCanvas::findLight(const string& id) {
    for (int i = 0; i < lightList.size(); i++) {
        if (lightList[i]->name == id) return lightList[i];
    }
    return nullptr;
}

Model* MyGLCanvas::findModel(const string& id) {
    for (int i = 0; i < modelList.size(); i++) {
        if (modelList[i]->name == id) return modelList[i];
    }
    return nullptr;
}

Light* MyGLCanvas::firstLight() {
    return lightList[0];
}

Model* MyGLCanvas::firstModel() {
    return modelList[0];
}

bool MyGLCanvas::scrollEvent(const nanogui::Vector2i& p, const nanogui::Vector2f& rel) {
    // p是鼠标位置，rel是鼠标的滚动，(0,1)是前滚，(0,-1)是后滚
    //std::cout << "scroll" << std::endl;
    //std::cout << p.x() << " " << p.y() << std::endl;
    //std::cout << rel.x() << " " << rel.y() << std::endl;
    //if (rel.y() == 1) {
    //    camera->cameraPos += camera->scaleSpeed * camera->cameraFront;
    //    return true;
    //}
    //else if (rel.y() == -1) {
    //    camera->cameraPos -= camera->scaleSpeed * camera->cameraFront;
    //    return true;
    //}
    //if (rel.y() == 1) {
    //    camera->fov -= 2;
    //    if (camera->fov < 1.0f) {
    //        camera->fov = 1.0f;
    //    }
    //    else if (camera->fov > 60.0f) {
    //        camera->fov = 60.0f;
    //    }
    //    return true;
    //}
    //else if (rel.y() == -1) {
    //    camera->fov += 2;
    //    if (camera->fov < 1.0f) {
    //        camera->fov = 1.0f;
    //    }
    //    else if (camera->fov > 45.0f) {
    //        camera->fov = 45.0f;
    //    }
    //    return true;
    //}
    if (rel.y() == 1) {
        camera->dollyDelta += 2.f;
        return true;
    }
    else if (rel.y() == -1) {
        camera->dollyDelta -= 2.f;
        return true;
    }
    return false;
}

//bool MyGLCanvas::mouseDragEvent(const nanogui::Vector2i& p, const nanogui::Vector2i& rel, int button, int modifiers) {
//    // 左键是1，右键是2，p是鼠标位置，rel是相对于上一步朝哪个方向移动
//    //std::cout << button << std::endl;
//    //std::cout << p.x() << " " << p.y() << std::endl;
//    //std::cout << rel.x() << " " << rel.y() << std::endl;
//    if (button == 1) {
//        //if (camera->firstMouse) {
//        //    camera->lastX = p.x();
//        //    camera->lastY = p.y();
//        //    camera->firstMouse = false;
//        //}
//
//        //float xOffset = p.x() - camera->lastX;
//        //float yOffset = p.y() - camera->lastY;
//        //std::cout << p.x() << " " << p.y() << std::endl;
//        //std::cout << camera->lastX << " " << camera->lastY << std::endl;
//        //std::cout << std::endl;
//        //camera->lastX = p.x();
//        //camera->lastY = p.y(); 
//
//        //xOffset *= camera->sensitivity;
//        //yOffset *= camera->sensitivity;
//
//        //camera->yaw += xOffset;
//        //camera->pitch -= yOffset;
//        //if (camera->pitch > 89.0f) {
//        //    camera->pitch = 89.0f;
//        //}
//        //if (camera->pitch < -89.0f) {
//        //    camera->pitch = -89.0f;
//        //}
//
//        //vec3 front;
//        //front.x = cos(radians(camera->yaw)) * cos(radians(camera->pitch));
//        //front.y = sin(radians(camera->pitch));
//        //front.z = sin(radians(camera->yaw)) * cos(radians(camera->pitch));
//        //camera->cameraFront = front;
//        //return true;
//
//        vec3 from_target = camera->cameraPos - camera->cameraTarget;
//        float radius = 3.0f;
//        float theta = (float)atan2(from_target.x, from_target.z);  /* azimuth */
//        float phi = (float)acos(from_target.y / radius);           /* polar */
//        vec3 offset;
//
//        offset.x = radius * (float)sin(phi) * (float)sin(theta);
//        offset.y = radius * (float)cos(phi);
//        offset.z = radius * (float)sin(phi) * (float)cos(theta);
//        camera->cameraPos += offset;
//        return true;
//    }
//    else if (button == 2) {
//        if (rel.x() == 1) {
//            std::cout << "1" << std::endl;
//            camera->cameraPos -= normalize(cross(camera->cameraFront, camera->cameraUp)) * camera->translateSpeed;
//        }
//        else if (rel.x() == -1) {
//            std::cout << "2" << std::endl;
//            camera->cameraPos += normalize(cross(camera->cameraFront, camera->cameraUp)) * camera->translateSpeed;
//        }
//        else if (rel.y() == 1) {
//
//        } 
//    }
//    return false;
//}

// 可以用来实现fps镜头
//bool MyGLCanvas::mouseMotionEvent(const nanogui::Vector2i& p, const nanogui::Vector2i& rel, int button, int modifiers) {
//    if (camera->firstMouse) {
//        camera->lastX = p.x();
//        camera->lastY = p.y();
//        camera->firstMouse = false;
//    }
//
//    float xOffset = p.x() - camera->lastX;
//    float yOffset = p.y() - camera->lastY;
//    std::cout << p.x() << " " << p.y() << std::endl;
//    std::cout << camera->lastX << " " << camera->lastY << std::endl;
//    std::cout << std::endl;
//    camera->lastX = p.x();
//    camera->lastY = p.y();
//
//    xOffset *= camera->sensitivity;
//    yOffset *= camera->sensitivity;
//
//    camera->yaw += xOffset;
//    camera->pitch += yOffset;
//    if (camera->pitch > 89.0f) {
//        camera->pitch = 89.0f;
//    }
//    if (camera->pitch < -89.0f) {
//        camera->pitch = -89.0f;
//    }
//
//    vec3 front;
//    front.x = cos(radians(camera->yaw)) * cos(radians(camera->pitch));
//    front.y = sin(radians(camera->pitch));
//    front.z = sin(radians(camera->yaw)) * cos(radians(camera->pitch));
//    camera->cameraFront = front;
//    return true;
//}

bool MyGLCanvas::mouseButtonEvent(const nanogui::Vector2i& p, int button, bool down, int modifiers) {
    // button左键是0，右键是1
    // down 按下是1，松开是0
    if (!focused()) {
        requestFocus();
    }
    vec2 cursorPos = vec2(p.x(), p.y());
    camera->cursor = cursorPos;
    if (button == GLFW_MOUSE_BUTTON_1) {
        if (down) {
            camera->isOrbiting = true;
            camera->orbitPos = cursorPos;
        }
        else {
            vec2 posDelta = getPosDelta(camera->orbitPos, cursorPos);
            camera->isOrbiting = false;
            camera->orbitDelta += posDelta;
        }
        //std::cout << camera->orbitPos.x << " " << camera->orbitPos.y << std::endl;
        return true;
    }
    else if (button == GLFW_MOUSE_BUTTON_2) {
        if (down) {
            camera->isPanning = true;
            camera->panPos = cursorPos;
        }
        else {
            vec2 posDelta = getPosDelta(camera->panPos, cursorPos);
            camera->isPanning = false;
            camera->panDelta += posDelta;
        }
        return true;
    }
    return false;
}

bool MyGLCanvas::mouseDragEvent(const nanogui::Vector2i& p, const nanogui::Vector2i& rel, int button, int modifiers) {
    //vec2 cursorPos = vec2(p.x(), p.y());
    //camera->cursor = cursorPos;
    //if (button == 1) {
    //    //camera->orbitPos = cursorPos;
    //    //std::cout << camera->orbitPos.x << " " << camera->orbitPos.y << std::endl;
    //    return true;
    //}
    //else if (button == 2) {
    //    //camera->panPos = cursorPos;
    //    return true;
    //}
    camera->cursor.x = p.x();
    camera->cursor.y = p.y();
    return true;
}

bool MyGLCanvas::mouseMotionEvent(const nanogui::Vector2i& p, const nanogui::Vector2i& rel, int button, int modifiers) {
    camera->cursor = vec2(p.x(), p.y());
    return true;
}

vec2 MyGLCanvas::getPosDelta(vec2 old_pos, vec2 new_pos) {
    return (new_pos - old_pos) / 400.0f;
}

void MyGLCanvas::updateCamera() {
    vec2 cursorPos = camera->cursor;
    //std::cout << cursorPos.x << " " << cursorPos.y << std::endl;
    if (camera->isOrbiting) {
        vec2 posDelta = getPosDelta(camera->orbitPos, cursorPos);
        camera->orbitDelta += posDelta;
        camera->orbitPos = cursorPos;
    }
    if (camera->isPanning) {
        vec2 posDelta = getPosDelta(camera->panPos, cursorPos);
        /*std::cout << posDelta.x << " " << posDelta.y << std::endl;*/
        camera->panDelta += posDelta;
        //std::cout << camera->panDelta.x << " " << camera->panDelta.y << std::endl;
        camera->panPos = cursorPos;
    }
    camera->orbit = camera->orbitDelta;
    camera->pan = camera->panDelta;
    //std::cout << camera->pan.x << " " << camera->pan.y << std::endl;
    camera->dolly = camera->dollyDelta;
    //std::cout << camera->dollyDelta << std::endl;
    //if (count1 == 1) {
    //    std::cout << camera->pan.x << " " << camera->pan.y  << std::endl;
    //    std::cout << camera->orbit.x << " " << camera->orbit.y << std::endl;
    //    std::cout << std::endl;
    //    count1++;
    //}
    //if (count1 == 1) {
    //    std::cout << camera->position.x << " ei " << camera->position.y << " " << camera->position.z << std::endl;
    //    std::cout << camera->target.x << " " << camera->target.y << " " << camera->target.z << std::endl;
    //    std::cout << std::endl;
    //    count1++;
    //}
    //std::cout << camera->position.x << " " << camera->position.y << " " << camera->position.z << std::endl;
    //std::cout << camera->target.x << " " << camera->target.y << " " << camera->target.z << std::endl;
    //std::cout << std::endl;
    camera->updateTransform();
    
    camera->dollyDelta = 0;
    camera->orbitDelta.x = 0;
    camera->orbitDelta.y = 0;
    camera->panDelta.x = 0;
    camera->panDelta.y = 0;

    //std::cout << camera->position.x << " " << camera->position.y << " " << camera->position.z << std::endl;
    //std::cout << camera->target.x << " " << camera->target.y << " " << camera->target.z << std::endl;
    //std::cout << std::endl;
}

bool MyGLCanvas::keyboardEvent(int key, int scancode, int action, int modifiers) {
    //std::cout << "debgu2" << std::endl;
    //if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    //    setVisible(false);
    //    return false;
    //}
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        camera->recoverDefaultLocation();
    }
    return true;
}