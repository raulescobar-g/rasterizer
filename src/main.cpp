#include <iostream>
#include <string>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#include "Image.h"
#include "Triangle.h"
#include "Zbuffer.h"
#include "utils.h"

using namespace std;

int main(int argc, char **argv) {
  if (argc < 2) {
    cout << "Usage: raster meshfile outfile width height" << endl;
    return 0;
  }
  string meshName(argv[1]);
  string filename(argv[2]);
  int width = atoi(argv[3]);
  int height = atoi(argv[4]);

  // Load geometry
  vector<float> posBuf; // list of vertex positions
  vector<float> norBuf; // list of vertex normals
  vector<float> texBuf; // list of vertex texture coords
  tinyobj::attrib_t attrib;
  std::vector<tinyobj::shape_t> shapes;
  std::vector<tinyobj::material_t> materials;
  string errStr;

  bool rc =
      tinyobj::LoadObj(&attrib, &shapes, &materials, &errStr, meshName.c_str());

  if (!rc) {
    cerr << errStr << endl;
  } else {
    // Loop over shapes
    for (size_t s = 0; s < shapes.size(); s++) {
      // Loop over faces (polygons)
      size_t index_offset = 0;
      for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
        size_t fv = shapes[s].mesh.num_face_vertices[f];
        // Loop over vertices in the face.
        for (size_t v = 0; v < fv; v++) {
          // access to vertex
          tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
          posBuf.push_back(attrib.vertices[3 * idx.vertex_index + 0]);
          posBuf.push_back(attrib.vertices[3 * idx.vertex_index + 1]);
          posBuf.push_back(attrib.vertices[3 * idx.vertex_index + 2]);
          if (!attrib.normals.empty()) {
            norBuf.push_back(attrib.normals[3 * idx.normal_index + 0]);
            norBuf.push_back(attrib.normals[3 * idx.normal_index + 1]);
            norBuf.push_back(attrib.normals[3 * idx.normal_index + 2]);
          }
          if (!attrib.texcoords.empty()) {
            texBuf.push_back(attrib.texcoords[2 * idx.texcoord_index + 0]);
            texBuf.push_back(attrib.texcoords[2 * idx.texcoord_index + 1]);
          }
        }
        index_offset += fv;
        // per-face material (IGNORE)
        shapes[s].mesh.material_ids[f];
      }
    }
  }

  auto image = make_shared<Image>(width, height);

  vector<float> box = inplace_linear_bounding_box_search(posBuf);

  float s = scale(height, width, box);
  float tx = translation(width, s, box[0], box[3]);
  float ty = translation(height, s, box[1], box[4]);

  vector<Triangle> triangles;

  extract_triangles(triangles, posBuf, norBuf, s, tx, ty);
  Zbuff z_buff(width, height);
  raster(triangles, s, tx, ty, image, z_buff);
  image->writeToFile(filename);
  return 0;
}
