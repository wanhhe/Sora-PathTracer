#include "camera.h"

int count = 1;

//Camera::Camera(vec3 lookFrom, vec3 lookTarget, vec3 up, float fov, float aspectRation) {
//	float theta = radians(fov);
//	auto h = tan(theta / 2.0);
//	auto viewHeight = 2.0 * h;
//	auto viewWidth = viewHeight * aspectRation;
//
//	// ÉãÏñ»úµÄ×ø±ê u,v,w
//	auto w = normalize(lookFrom - lookTarget);
//	auto u = normalize(cross(up, w));
//	auto v = cross(w, u);
//
//	origin = lookFrom;
//	horizontal = (float)viewWidth * u;
//	vertical = (float)viewHeight * v;
//	upperLeftCorner = origin - w - horizontal / 2.f - vertical / 2.f;
//}
//
//Ray Camera::getRayRandom(int row, int col) {
//	auto sample = linearRand(vec2(0.0), vec2(1.0));
//	float rowScale = (float)row + sample.x;
//	float colScale = (float)col + sample.y;
//	auto lookAt = upperLeftCorner + vertical * rowScale / (float)SCREEN_HEIGHT + horizontal * colScale / (float)SCREEN_WIDTH;
//	return Ray(origin, lookAt - origin);
//}

vec3 Camera::calculatePan(vec3 fromCamera) {
    vec3 forward = normalize(fromCamera);
    vec3 left = normalize(cross(up, forward));
    vec3 up = cross(forward, left);

    float distance = glm::length(fromCamera);
    float factor = distance * (float)tan(fov / 2) * 2;
    vec3 deltaX = left * pan.x * factor;
    vec3 deltaY = up * pan.y * factor;
    //std::cout << pan.x << " " << pan.y << std::endl;
    //if (count == 1) {
    //    std::cout << pan.x << " " << pan.y  << std::endl;
    //    std::cout << std::endl;
    //    count++;
    //}
    //std::cout << pan.x << " " << pan.y << std::endl;
    return deltaX + deltaY;
}

vec3 Camera::calculateOffset(vec3 fromTarget) {
    float radius = glm::length(fromTarget);
    float theta = (float)atan2(fromTarget.x, fromTarget.z);  /* azimuth */
    float phi = glm::acos(fromTarget.y / radius);           /* polar */
    vec3 offset;

    radius *= (float)pow(0.95, dolly);
    theta -= orbit.x * TWO_PI;
    phi -= orbit.y * TWO_PI;
    //std::cout << orbit.y << std::endl;
    //std::cout << phi << std::endl;
    phi = phi < EPSILON ? EPSILON : (phi > PI - EPSILON ? PI - EPSILON : phi);
    //std::cout << glm::cos(glm::half_pi<float>()) << std::endl;

    //std::cout << radius << std::endl;
    offset.x = radius * (float)sin(phi) * (float)sin(theta);
    offset.y = radius * (float)cos(phi);
    offset.z = radius * (float)sin(phi) * (float)cos(theta);

    //std::cout << offset.y << std::endl;
    //std::cout << "phi" << phi << " theta" << theta << std::endl;

    return offset;
}

void Camera::updateTransform() {
    vec3 from_target = position - target;
    vec3 from_camera = target - position;
    vec3 _pan = calculatePan(from_camera);
    vec3 offset = calculateOffset(from_target);
    //if (count == 1) {
    //    std::cout << position.x << " " << position.y << " " << position.z << std::endl;
    //    std::cout << target.x << " " << target.y << " " << target.z << std::endl;
    //    std::cout << std::endl;
    //    count++;
    //}
    if (count == 1) {
        std::cout << _pan.x << " " << _pan.y << " " << _pan.z << std::endl;
        std::cout << offset.x << " offset " << offset.y << " " << offset.z << std::endl;
        std::cout << std::endl;
        count++;
    }
    //std::cout << _pan.x << " " << _pan.y << " " << _pan.z << std::endl;
    //std::cout << offset.x << " offset " << offset.y << " " << offset.z << std::endl;
    target += _pan;
    position = target + offset;
    //std::cout << position.z << std::endl;
}

vec3 Camera::getForward() {
    return normalize(target - position);
}

void Camera::setTransform(vec3 _position, vec3 _target) {
    position = _position;
    target = _target;
}

void Camera::setDefaultLocation(vec3 _cameraPosition, vec3 _cameraTarget) {
    cameraPosition = _cameraPosition;
    cameraTarget = _cameraTarget;
}

void Camera::recoverDefaultLocation() {
    position = cameraPosition;
    target = cameraTarget;
}