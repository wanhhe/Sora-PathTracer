#include "canvas.h"

int count1 = 1;

MyGLCanvas::MyGLCanvas(Widget* parent, Camera* _camera) : 
    nanogui::GLCanvas(parent), hikari("../models/sara/sara.obj"), camera(_camera) {
    using namespace nanogui;

    //mShader.init(
    //    /* An identifying name */
    //    "a_simple_shader",

    //    /* Vertex shader */
    //    "#version 330 core\n"
    //    "layout(location = 0) in vec3 aPos;\n"
    //    "layout(location = 1) in vec3 aNormal;\n"
    //    "layout(location = 2) in vec2 aTexCoords;\n"

    //    "out vec2 TexCoords;\n"
    //    "uniform mat4 modelViewProj;\n"

    //    "void main(){\n"
    //        "TexCoords = aTexCoords;\n"
    //        "gl_Position = modelViewProj * vec4(aPos, 1.0);\n"
    //    "}\n",

    //    /* Fragment shader */
    //    "#version 330 core\n"
    //    "out vec4 FragColor;\n"

    //    "in vec2 TexCoords;\n"

    //    "uniform sampler2D texture_diffuse1;\n"

    //    "void main(){\n"
    //        "FragColor = texture(texture_diffuse1, TexCoords);\n"
    //    "}\n"   
    //);

    mShader.init(
        /* An identifying name */
        "a_simple_shader",

        /* Vertex shader */
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

        "void main(){\n"
        "   FragPos = vec3(model * vec4(aPos, 1.0));\n"
        "   Normal = normalize(mat3(transpose(inverse(model))) * aNormal);\n"
        "   TexCoords = aTexCoords;\n"
        "   gl_Position = projection * view * vec4(FragPos, 1.0);\n"
        "}\n",

        /* Fragment shader */
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
        "}\n"
    );

    lightShader.init(
        "light_shader",

        "#version 330 core\n"
        "in vec3 position;\n"
        "uniform mat4 lightMVP;\n"
        "void main() {\n"
        "   gl_Position = lightMVP * vec4(position, 1.0);\n"
        "}\n",

        "#version 330 core\n"
        "out vec4 FragColor;\n"

        "void main() {\n"
        "   FragColor = vec4(1.0);\n"
        "}\n"
    );

    MatrixXu indices(3, 12); /* Draw a cube */
    indices.col(0) << 0, 1, 3;
    indices.col(1) << 3, 2, 1;
    indices.col(2) << 3, 2, 6;
    indices.col(3) << 6, 7, 3;
    indices.col(4) << 7, 6, 5;
    indices.col(5) << 5, 4, 7;
    indices.col(6) << 4, 5, 1;
    indices.col(7) << 1, 0, 4;
    indices.col(8) << 4, 0, 3;
    indices.col(9) << 3, 7, 4;
    indices.col(10) << 5, 6, 2;
    indices.col(11) << 2, 1, 5;

    MatrixXf positions(3, 8);
    positions.col(0) << 2, 3, 3;
    positions.col(1) << 2, 3, 2;
    positions.col(2) << 3, 3, 2;
    positions.col(3) << 3, 3, 3;
    positions.col(4) << 2, 2, 3;
    positions.col(5) << 2, 2, 2;
    positions.col(6) << 3, 2, 2;
    positions.col(7) << 3, 2, 3;

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

    translate = vec3(0.f);
    scale = vec3(1.f);
    model = mat4(1.0f);
    model = glm::translate(model, vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
    //mShader.bind();
 /*   mShader.uploadIndices(indices);
    mShader.uploadAttrib("position", positions);
    mShader.uploadAttrib("color", colors);*/

    mShader.bind();
    mShader.setUniform("viewPos", camera->position);
    mShader.setUniform("lightPos", vec3(2.5, 2.5, 2.5));
    mShader.setUniform("lightColor", vec3(1.0, 1.0, 1.0));

    lightShader.bind();
    lightShader.uploadIndices(indices);
    lightShader.uploadAttrib("position", positions);
}

void MyGLCanvas::drawGL() {
    using namespace nanogui;

    mShader.bind();

    mat4 mvp;
    updateCamera();
    //std::cout << camera->position.x << " " << camera->position.y << " " << camera->position.z << std::endl;
    //std::cout << camera->target.x << " " << camera->target.y << " " << camera->target.z << std::endl;
    //std::cout << std::endl;
    view = lookAt(camera->position, camera->target, camera->up);
    projection = glm::perspective(camera->fov, camera->aspect, 0.1f, 100.0f);
    //view = lookAt(vec3(0.0f, 0.0f, 3.f), vec3(0.f, 0.f, 2.f), vec3(0, 1.f, 0));
    //projection = glm::perspective(radians(camera->fov), 1.0f, 0.1f, 100.0f);
    //projection = glm::perspective(radians(45.f), 1.0f, 0.1f, 100.0f);

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
    model = glm::translate(model, translate);
    model = glm::scale(model, scale);
    mvp = projection * view * model;

    mShader.setUniform("model", model);
    mShader.setUniform("view", view);
    mShader.setUniform("projection", projection);
    glEnable(GL_DEPTH_TEST);
    hikari.draw(mShader);
    glDisable(GL_DEPTH_TEST);

    lightShader.bind();
    lightShader.setUniform("lightMVP", projection * view);
    glEnable(GL_DEPTH_TEST);
    /* Draw 12 triangles starting at index 0 */
    //mShader.drawIndexed(GL_TRIANGLES, 0, 12);
    lightShader.drawIndexed(GL_TRIANGLES, 0, 12);
    glDisable(GL_DEPTH_TEST);
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
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        setVisible(false);
        return false;
    }
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        camera->recoverDefaultLocation();
    }
    return true;
}