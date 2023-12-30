#include "pixello.hpp"


inline float square(float a)
{
  float result = a * a;
  return result;
}

union v2
{
  struct
  {
    float x, y;
  };
  float e[2];
};


inline v2 operator*(float a, v2 b)
{
  v2 result;

  result.x = a * b.x;
  result.y = a * b.y;

  return result;
}


inline v2 operator*(v2 b, float a)
{
  v2 result = a * b;
  return result;
}


inline v2& operator*=(v2& b, float a)
{
  b = a * b;

  return b;
}


inline v2 operator+(v2 a, v2 b)
{
  v2 result;

  result.x = a.x + b.x;
  result.y = a.y + b.y;

  return result;
}


inline v2& operator+=(v2& a, v2 b)
{
  a = a + b;

  return a;
}


constexpr int screen_w = 1280;
constexpr int screen_h = 800;

texture_t sprite_sheet;
texture_t tile_sheet;

struct player_t
{
  v2 position;
  rect_t box;
  v2 velocity;  // velocity (first derivate)

  rect_t sprite_crop;
  float speed;
};

player_t player;


class bullet_hell : public pixello
{
public:
  bullet_hell() : pixello(screen_w, screen_h, "Bullet Hell", 60) {}

private:
  void on_init(void*) override
  {
    log("on init");
    sprite_sheet = load_image("assets/sprite_sheet.png");
    tile_sheet = load_image("assets/tile_sheet.png");

    player.position = {screen_w / 2, screen_h / 2};
    player.box = {(int)player.position.x, (int)player.position.y, 64, 64};
    player.sprite_crop = {0, (5 * 16) + 8, 16, 16};
    player.velocity = {.0f, .0f};
    player.speed = 9000.0f;
  }

  void on_update(void*) override
  {
    {  // Useless
      if (mouse_state().left_button.click) { log("Click!"); }

      // Check if we have to quit the game
      if (is_key_pressed(keycap_t::ESC)) { stop(); }

      const float delta_seconds =
          static_cast<float>(delta_time()) / get_performance_freq();
      v2 dd_player = {0.0f, 0.0f};  // player Second derivate

      if (is_key_pressed(keycap_t::LEFT)) { dd_player.x = -1.0f; }
      if (is_key_pressed(keycap_t::RIGHT)) { dd_player.x = 1.0f; }
      if (is_key_pressed(keycap_t::UP)) { dd_player.y = -1.0f; }
      if (is_key_pressed(keycap_t::DOWN)) { dd_player.y = 1.0f; }

      // Normalizing in case diagonal
      if (dd_player.x != 0.0f && dd_player.y != 0.0f) {
        dd_player *= 0.707106781187f;
      }

      dd_player *= player.speed;

      // Deceleration
      dd_player += -5.5f * player.velocity;

      // Getting new player velocity
      v2 player_pos = 0.5f * dd_player * square(delta_seconds) +
                      player.velocity * delta_seconds + player.position;

      if (player_pos.x < 0) {
        player.position.x = 0;
      } else if (player_pos.x > screen_w) {
        player.position.x = screen_w;
      } else {
        player.position.x = player_pos.x;
      }

      if (player_pos.y < 0) {
        player.position.y = 0;
      } else if (player_pos.y > screen_h) {
        player.position.y = screen_h;
      } else {
        player.position.y = player_pos.y;
      }

      player.velocity = (dd_player * delta_seconds) + player.velocity;
      // player.box.x = float(dd_player.x * delta_seconds) + player.box.x;
      // player.box.y = float(dd_player.y * delta_seconds) + player.box.y;

      player.box.x = static_cast<int>(player.position.x) - (player.box.w / 2);
      player.box.y = static_cast<int>(player.position.y) - (player.box.h / 2);

      // draw_rect_outline(player.box, 0xFFFFFFFF);
      draw_texture(sprite_sheet, player.box, player.sprite_crop);
    }
  }
};


int main()
{
  bullet_hell p;

  if (p.run()) { return 0; }

  return 1;
}