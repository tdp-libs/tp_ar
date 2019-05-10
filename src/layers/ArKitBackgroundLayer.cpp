#include "tp_ar/layers/ArKitBackgroundLayer.h"
#include "tp_ar/ar_sessions/ArKitSession.h"

#include "tp_maps/Map.h"
#include "tp_maps/RenderInfo.h"
#include "tp_maps/shaders/YUVImageShader.h"

namespace tp_ar
{

//##################################################################################################
struct ArKitBackgroundLayer::Private
{
  ArKitSession* session;

  bool updateTexture{true};
  GLuint textureID{0};

  std::vector<GLuint> indexes{0,1,2,3};
  std::vector<tp_maps::ImageShader::Vertex> verts;
  glm::vec4 color{1.0f, 1.0f, 1.0f, 1.0f};

  tp_maps::ImageShader::VertexBuffer* vertexBuffer{nullptr};
  bool updateVertexBuffer{true};
  glm::vec2 imageScale{1.0f, 1.0f};

  //################################################################################################
  Private(ArKitSession* session_):
    session(session_)
  {

  }

  //################################################################################################
  ~Private()
  {
    deleteVertexBuffers();
  }

  //################################################################################################
  void deleteVertexBuffers()
  {
    delete vertexBuffer;
    vertexBuffer=nullptr;
  }
};

//##################################################################################################
ArKitBackgroundLayer::ArKitBackgroundLayer(ArKitSession* session):
  d(new Private(session))
{

}

//##################################################################################################
ArKitBackgroundLayer::~ArKitBackgroundLayer()
{
  delete d;
}

//##################################################################################################
void ArKitBackgroundLayer::updateTexture()
{
  d->updateTexture = true;
  if(d->imageScale != d->session->imageScale())
  {
    d->imageScale = d->session->imageScale();
    d->updateVertexBuffer = true;
  }
  update();
}

//##################################################################################################
void ArKitBackgroundLayer::render(tp_maps::RenderInfo& renderInfo)
{
  if(renderInfo.pass != defaultRenderPass())
    return;

  if(d->updateTexture)
  {
    d->updateTexture = false;

    bool valid=false;
    d->session->viewYCbCr([&](size_t w, size_t h, const std::vector<uint8_t>& data)
    {
      if(d->textureID==0)
      {
        glGenTextures(1, &d->textureID);
        if(d->textureID==0)
          return;
      }

      glBindTexture(GL_TEXTURE_2D, d->textureID);

      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, GLsizei(w), GLsizei(h), 0, GL_RGBA, GL_UNSIGNED_BYTE, data.data());

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

      valid = true;
    });

    if(!valid)
    {
      map()->deleteTexture(d->textureID);
      d->textureID=0;
    }
  }

  if(d->textureID==0)
    return;

  auto shader = map()->getShader<tp_maps::YUVImageShader>();
  if(shader->error())
    return;

  if(d->updateVertexBuffer)
  {
    d->verts.clear();

    glm::vec2 t(1.0f, 1.0f);

    auto s = d->session->imageScale();

    float x0 = -1.0f*s.x;
    float y0 = -1.0f*s.y;
    float x1 =  1.0f*s.x;
    float y1 =  1.0f*s.y;

    d->verts.push_back(tp_maps::ImageShader::Vertex({x1,y0,0}, {0,0,1}, { t.x, 0.0f}));
    d->verts.push_back(tp_maps::ImageShader::Vertex({x1,y1,0}, {0,0,1}, {0.0f, 0.0f}));
    d->verts.push_back(tp_maps::ImageShader::Vertex({x0,y1,0}, {0,0,1}, {0.0f,  t.y}));
    d->verts.push_back(tp_maps::ImageShader::Vertex({x0,y0,0}, {0,0,1}, { t.x,  t.y}));

    delete d->vertexBuffer;
    d->vertexBuffer = shader->generateVertexBuffer(map(), d->indexes, d->verts);
    d->updateVertexBuffer=false;
  }

  shader->use();
  shader->setMatrix(glm::mat4(1));
  shader->setTexture(d->textureID);
  shader->draw(GL_TRIANGLE_FAN, d->vertexBuffer, d->color);
}

//##################################################################################################
void ArKitBackgroundLayer::invalidateBuffers()
{
  d->updateTexture = true;
  d->textureID = 0;
  d->deleteVertexBuffers();
  d->updateVertexBuffer=true;
}

}
