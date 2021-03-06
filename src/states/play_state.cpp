#include "../common.hpp"

PlayState::PlayState(Engine& engine)
	: AbstractState(engine), _map(engine, "data/wil"), _car(engine, "data/car.obj")
{
	SDL_GetMouseState(&_old_mouse_pos.first, &_old_mouse_pos.second);
}

void
PlayState::update()
{
	Uint8* keys = SDL_GetKeyState(NULL);

	float speed = 0.2;

	std::pair<int, int> new_mouse_pos;
	SDL_GetMouseState(&new_mouse_pos.first, &new_mouse_pos.second);

	Degrees y_rot( (_old_mouse_pos.first - new_mouse_pos.first) / 5.0 );
	Degrees x_rot( (_old_mouse_pos.second - new_mouse_pos.second) / 5.0 );
	if(std::fabs(y_rot.value()) > 0.5)
		_camera.rotateY(y_rot);
	if(std::fabs(x_rot.value()) > 0.5)
		_camera.rotateX(x_rot);

	SDL_WarpMouse(_engine.getWidth() / 2, _engine.getHeight() / 2);
	_old_mouse_pos = std::pair<int, int>(_engine.getWidth() / 2, _engine.getHeight() / 2);

	if(keys[SDLK_z]) _camera.move(Vector3( 0.0, 0.0, speed));
	if(keys[SDLK_x]) _camera.move(Vector3( 0.0, 0.0,-speed));

	if(keys[SDLK_w]) _camera.moveForward(speed);
	if(keys[SDLK_s]) _camera.moveForward(-speed);

	if(keys[SDLK_DOWN]) _camera.rotateX(Degrees(2));
	if(keys[SDLK_UP]) _camera.rotateX(Degrees(-2));
	if(keys[SDLK_a]) _camera.strafe(-speed);
	if(keys[SDLK_d]) _camera.strafe(speed);

	_camera.update();
}

void
PlayState::handleInput(const SDL_Event& event)
{
	if(event.type == SDL_KEYDOWN)
	{
		switch(event.key.keysym.sym)
		{
			default:
				break;
		}
	}
}

void
PlayState::draw()
{
	/* I have no idea why the program crashes when or-ing the two parameters
	 * instead of calling the function twice o_O
	 *
	 * #0  0xb72f5720 in ?? () from /usr/lib/dri/i915_dri.so
	 * #1  0xb7271884 in _tnl_run_pipeline () from /usr/lib/dri/i915_dri.so
	 * #2  0xb71c46b9 in ?? () from /usr/lib/dri/i915_dri.so
	 * #3  0xb7271df5 in _tnl_draw_prims () from /usr/lib/dri/i915_dri.so
	 * #4  0xb726a420 in ?? () from /usr/lib/dri/i915_dri.so
	 * #5  0xb7190425 in intel_clear_tris () from /usr/lib/dri/i915_dri.so
	 * #6  0xb71908de in ?? () from /usr/lib/dri/i915_dri.so
	 * #7  0xb71e27a9 in _mesa_Clear () from /usr/lib/dri/i915_dri.so
	 * #8  0x0806bc78 in PlayState::draw (this=0x8fd83c0)
	 *     at /src/states/play_state.cpp:63
	 */
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30.0, 800/600.0, 0.1, 1000.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	/*
	gluLookAt(
			_x_offset, _y_offset, _z_offset,
			_x_offset, _y_offset, 0.0,
			0.0, 1.0, 0.0
	);*/

	_camera.draw();

	glPushMatrix();
		_map.draw();
	glPopMatrix();
	/*
	glPushMatrix();
		glColor3f(1.0, 1.0, 1.0);
		_car.draw();
	glPopMatrix();
	*/
}

