#include "render/frontend/Renderer.h"

#include <glad/glad.h>
#include <algorithm>
#include <cassert>
#include <cstdio>

namespace Engine::Render {

Renderer::Renderer() = default;
Renderer::~Renderer() = default;

void Renderer::Init() {
    assert(!m_Initialized && "Renderer::Init() called twice");

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    m_Initialized = true;
}

void Renderer::SetClearColor(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
}

void Renderer::BeginScene(const Core::Camera& camera) {
    assert(m_Initialized && "Call Renderer::Init() before BeginScene()");

    m_Frame.Clear();
    m_Frame.ViewProjection  = camera.ViewProjection();
    m_Frame.CameraPosition  = camera.Position();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::Submit(uint32_t meshID, uint32_t materialID, const glm::mat4& transform) {
    m_Frame.DrawCalls.push_back({ meshID, materialID, transform });
}

void Renderer::EndScene() {
    SortByMaterial();   // batch by material before issuing any GL calls
    ExecuteDrawCalls();
}

// ── Private helpers ───────────────────────────────────────────────────────────

void Renderer::SortByMaterial() {
    // Sorting by materialID groups draw calls that share shader/texture state,
    // dramatically reducing glUseProgram / glBindTexture calls at scale.
    std::sort(m_Frame.DrawCalls.begin(), m_Frame.DrawCalls.end(),
        [](const DrawCall& a, const DrawCall& b) {
            if (a.MaterialID != b.MaterialID) return a.MaterialID < b.MaterialID;
            return a.MeshID < b.MeshID;
        });
}

void Renderer::ExecuteDrawCalls() {
    // TODO: resolve mesh/material handles through AssetManager, then draw.
    // Stub below shows the expected shape; fill in once AssetManager exists.
    //
    // uint32_t lastMaterial = UINT32_MAX;
    // for (const auto& dc : m_Frame.DrawCalls) {
    //     if (dc.MaterialID != lastMaterial) {
    //         auto& mat = AssetManager::GetMaterial(dc.MaterialID);
    //         mat.Bind();                     // binds shader + textures
    //         mat.SetMat4("uVP", m_Frame.ViewProjection);
    //         lastMaterial = dc.MaterialID;
    //     }
    //     auto& mesh = AssetManager::GetMesh(dc.MeshID);
    //     mesh.GetVAO().Bind();
    //     mat.SetMat4("uModel", dc.Transform);
    //     glDrawElements(GL_TRIANGLES, mesh.IndexCount(), GL_UNSIGNED_INT, nullptr);
    // }
    (void)m_Frame; // suppress unused warning until AssetManager is implemented
}

} // namespace Engine::Render
