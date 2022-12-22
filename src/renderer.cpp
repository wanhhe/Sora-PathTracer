#include "renderer.h"

void Renderer::clear() {

}

void Renderer::render() {

}

void Renderer::setModel(const mat4& _model) { model = _model; }
void Renderer::setView(const mat4& _view) { view = _view; }
void Renderer::setPorjection(const mat4& _projection) { projection = _projection; }