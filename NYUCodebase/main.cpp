
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

SDL_Window* displayWindow;
typedef struct {
	float x;
	float y;

	float r;
	float g;
	float b;
} Vertex2D;

GLuint LoadTexture(const char *image_path) {
	SDL_Surface *surface = IMG_Load(image_path);

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA,
		GL_UNSIGNED_BYTE, surface->pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	SDL_FreeSurface(surface);
	return textureID;
}

void DrawSprite(GLint texture, float x, float y, float rotation) {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	glTranslatef(x, y, 0.0);
	glRotatef(rotation, 0.0, 0.0, 1.0);

	GLfloat quad[] = { -0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f };
	glVertexPointer(2, GL_FLOAT, 0, quad);
	glEnableClientState(GL_VERTEX_ARRAY);

	GLfloat quadUVs[] = { 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0 };
	glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDrawArrays(GL_QUADS, 0, 4);
	glDisable(GL_TEXTURE_2D);
}

void drawSomething()
{
	Vertex2D triangle[3] = { { 0.0, -0.5, 1.0, 0.0, 0.0 },
	{ 1.0, -1.0, 0.0, 1.0, 0.0 },
	{ -1.0, -0.5, 0.0, 0.0, 1.0 } };
	glVertexPointer(2, GL_FLOAT, sizeof(float) * 5, triangle);
	glEnableClientState(GL_VERTEX_ARRAY);
	glColorPointer(3, GL_FLOAT, sizeof(float) * 5, &triangle[0]);
	glEnableClientState(GL_COLOR_ARRAY);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void drawSomething_1()
{
	Vertex2D triangle[3] = { { 0.0, 1.0, 1.0, 0.0, 0.0 },
	{ 1.0, 0.5, 0.0, 1.0, 0.0 },
	{ -1.0, 0.5, 0.0, 0.0, 1.0 } };
	glVertexPointer(2, GL_FLOAT, sizeof(float) * 5, triangle);
	glEnableClientState(GL_VERTEX_ARRAY);
	glColorPointer(3, GL_FLOAT, sizeof(float) * 5, &triangle[0]);
	glEnableClientState(GL_COLOR_ARRAY);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}

int main(int argc, char *argv[])
{
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("My Doge", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);

	bool done = false;
	
	SDL_Event event;
	float angel = 0;
	float lastFrameTicks = 0.0f;
	while (!done) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
				done = true;
			}
		}

		SDL_GL_SwapWindow(displayWindow);
		//glClearColor(0.3f, 0.2f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		float ticks = (float)SDL_GetTicks() / 1000.0f;
		float elapsed = ticks - lastFrameTicks;
		lastFrameTicks = ticks;
		angel += elapsed*100;
		GLuint example = LoadTexture("Doge.png");
		DrawSprite(example, 0.0, 0.0, angel);
		drawSomething();
		drawSomething_1();
		DrawSprite(example, 0.5, 0.0, angel);
		DrawSprite(example, -0.5, 0.0, angel);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisable(GL_BLEND);
		glDisable(GL_TEXTURE_2D);
	}

	SDL_Quit();
	return 0;
}