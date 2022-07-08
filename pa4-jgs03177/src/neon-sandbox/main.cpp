#include "test.hpp"

#include "neon/camera.hpp"
#include "neon/image.hpp"
#include "neon/integrator.hpp"
#include "neon/ray.hpp"
#include "neon/scene.hpp"
#include "neon/sphere.hpp"
#include "neon/utils.hpp"

#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <memory>
#include <taskflow/taskflow.hpp>

int main(int argc, char *argv[]) {
  int nx = 512;
  int ny = 512;
  int spp = 128;

  // create output image
  ne::Image canvas(nx, ny);
  glm::uvec2 tilesize(32, 32);

  // Split images into set of tiles.
  // Each thread render its corresponding tile.
  std::vector<ne::TileIterator> tiles = canvas.toTiles(tilesize);

  // create scene
  std::shared_ptr<ne::Scene> scene = testScene2();

  // spwan camera
  static ne::Camera camera;
  float distToFocus = 4;
  float aperture = 0.1f;
  //glm::vec3 lookfrom(0, 0, 3);
  glm::vec3 lookfrom(0, 2, 3);
  glm::vec3 lookat(0, 0, 0);
  camera = ne::Camera(lookfrom, lookat, glm::vec3(0, 1, 0), 60,
                      float(canvas.width()) / float(canvas.height()), aperture,
                      distToFocus);

  // summon progress bar. this is just eye candy.
  // you can use timer class instead
  ne::utils::Progressbar progressbar(canvas.numPixels());

  // prep to build task graph
  tf::Taskflow tf;
  tf::Task taskRenderStart =
      tf.emplace([&progressbar]() { progressbar.start(); });
  tf::Task taskRenderEnd = tf.emplace([&progressbar]() { progressbar.end(); });

  // build rendering task graph
  for (auto &tile : tiles) {
    tf::Task taskTileRender = tf.emplace([&]() {
      // Iterate pixels in tile
      for (auto &index : tile) {
        glm::vec3 color{0.0f};
        //float u = float(index.x) / float(canvas.width());
        //float v = float(index.y) / float(canvas.height());

		int jitter_n = 32;

		for (int i = 0; i < jitter_n; i++) {
		  for (int j = 0; j < jitter_n; j++) {  
            // epsilon xy
			float ex = glm::linearRand<float>(0.0f, 1.0f);
            float ey = glm::linearRand<float>(0.0f, 1.0f);

			// jittered uv
			float ju = float(index.x * jitter_n + i + ex) / float(jitter_n * canvas.width());
            float jv = float(index.y * jitter_n + j + ey) / float(jitter_n * canvas.height());

            // construct ray
            ne::Ray r = camera.sample_with_blur(ju, jv, 0.05f);
            //ne::Ray r = camera.sample(ju, jv);

            // compute color of ray sample and then add to pixel
            ne::core::Integrator Li;
            //color += Li.integrate(r, scene);
            color += Li.integrate2(r, scene);
		  }
		}

        color = color / float(jitter_n * jitter_n);
        color = glm::clamp(color, 0.0f, 1.0f);

        // record to canvas
        canvas(index) = glm::u8vec4(color * 255.99f, 255.0f);

        // update progressbar and draw it every 10 progress
        if (++progressbar % 20 == 0)
          progressbar.display();
      }
    });

    taskRenderStart.precede(taskTileRender);
    taskTileRender.precede(taskRenderEnd);
  }

  // start rendering
  tf.wait_for_all();

  canvas.save("result.png");
  return 0;
}
