#include <engine/pge.h>
#include <runtime/memory.h>
#include "game_camera.h"

namespace game
{
  using namespace pge;

  struct Game
  {
    u64        world;
    GameCamera camera;
    u64        viewport;
    i32        screen_width;
    i32        screen_height;
    u64        package;
  };

  static Game game;

  const f32 DEFAULT_FOV = 45.0f;
  const glm::vec3 IDENTITY_TRANSLATION(0, 0, 0);
  const glm::quat IDENTITY_ROTATION(1.f, 0.f, 0.f, 0.f);
  const u32 PAD_INDEX = 0u;
  const char *FONT_NAME = "fonts/consolas.24/consolas.24";
  const glm::vec2 virtual_resolution(1280, 720);

  void init()
  {
    Allocator &a = memory_globals::default_allocator();

    window::get_resolution(game.screen_width, game.screen_height);

    game.world = application::create_world();

    {
      f32 target_aspect = virtual_resolution.x / virtual_resolution.y;
      i32 width = game.screen_width;
      i32 height = (i32)(width / target_aspect + 0.5f);

      if (height > game.screen_height) {
        height = game.screen_height;
        width = (int)(height * target_aspect + 0.5f);
      }
      game.viewport = application::create_viewport((game.screen_width / 2) - (width / 2), (game.screen_height / 2) - (height / 2), width, height);
    }

    game_camera::init(game.camera, game.world, virtual_resolution, a);	
	
    game.package = application::resource_package("packages/default");
    resource_package::load(game.package);
    resource_package::flush(game.package);

  }

  void update(f64 delta_time)
  {
    world::update(game.world, delta_time);
    
    if ((pad::active(PAD_INDEX) && pad::button(PAD_INDEX, PAD_KEY_7)) ||
        keyboard::pressed(KEYBOARD_KEY_ESCAPE))
      application::quit();
  }

  void render()
  {
    application::render_world(game.world, game.camera.id, game.viewport);
  }

  void shutdown()
  {
    resource_package::unload(game.package);

    application::release_resource_package(game.package);
	
    game_camera::destroy(game.camera);
    application::destroy_world(game.world);
    application::destroy_viewport(game.viewport);
  }
}
