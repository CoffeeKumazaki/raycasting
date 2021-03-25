#include <stdafx.hpp>
#include "imgui_helper.hpp"
#include "raycasting.hpp"

std::vector<Segment> segments =  {

	// Border
	{{   0,   0}, { 1280,   0}},
	{{ 1280,   0}, { 1280, 720}},
	{{ 1280, 720}, {   0, 720}},
	{{   0, 720}, {   0,   0}},

	// Polygon #1
	{{100,150}, {120,50}},
	{{120,50}, {200,80}},
	{{200,80}, {140,210}},
	{{140,210}, {100,150}},

	// Polygon #2
	{{100,200}, {120,250}},
	{{120,250}, {60,300}},
	{{60,300}, {100,200}},

	// Polygon #3
	{{200,260}, {220,150}},
	{{220,150}, {300,200}},
	{{300,200}, {350,320}},
	{{350,320}, {200,260}},

	// Polygon #4
	{{340,60}, {360,40}},
	{{360,40}, {370,70}},
	{{370,70}, {340,60}},

	// Polygon #5
	{{450,190}, {560,170}},
	{{560,170}, {540,270}},
	{{540,270}, {430,290}},
	{{430,290}, {450,190}},

	// Polygon #6
	{{400,95}, {580,50}},
	{{580,50}, {480,150}},
	{{480,150}, {400,95}}

};

int main(int argc, char const *argv[]) {

	int w = 1280;
	int h = 720;
	std::string title = "Astar demo";
	initImgui(w, h, title);

	// Our state
	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	ImVec4 seg_color = ImVec4(1.0f, 1.0f, 1.0f, 1.00f);
	ImVec4 ray_color = ImVec4(1.0f, 0.0f, 0.0f, 1.00f);

	// Main loop
	while (!glfwWindowShouldClose(window))
	{
		// Poll and handle events (inputs, window resize, etc.)
		// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
		// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
		// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
		// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
		glfwPollEvents();

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
		{
			ImGuiIO io = ImGui::GetIO();
			ImVec2 mouse_pos = io.MousePos;
			Ray ray;
			ray.pos.x = mouse_pos.x;
			ray.pos.y = mouse_pos.y;
			ray.length = 100.0;
			ImGuiWindowFlags window_flags = 0;
			window_flags |= ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoResize;
			window_flags |= ImGuiWindowFlags_NoCollapse;
			window_flags |= ImGuiWindowFlags_NoBackground;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
			ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver);
			ImGui::SetNextWindowSize(ImVec2(w, h), ImGuiCond_FirstUseEver);

			ImGui::Begin("Ray Casting", nullptr, window_flags);
			std::vector<Vector2D> intersections;

			for (double angle = 0; angle < M_PI * 2; angle += (M_PI * 2)/ 50) {
				ray.dir = angle;
				Vector2D closest_intersection;
				double closest = __DBL_MAX__;
				bool find = false;
				for (size_t i = 0; i < segments.size(); i++) {
					Vector2D intersection;
					if (!getIntersection(ray, segments[i], intersection)) {
						continue;
					}

					double dx = intersection.x - ray.pos.x;
					double dy = intersection.y - ray.pos.y;
					double d = sqrt(dx*dx+dy*dy);
					if (d < closest) {
						closest = d;
						find = true;
						closest_intersection = intersection;
					}
				}
				if (find) {
					intersections.push_back(closest_intersection);
				}
			}

			ImDrawList *draw_list = ImGui::GetWindowDrawList();
			for (size_t i = 0; i < segments.size(); i++) {
				ImVec2 start = {(float)segments[i].begin.x, (float)segments[i].begin.y};
				ImVec2 end = {(float)segments[i].end.x, (float)segments[i].end.y};
				draw_list->AddLine(start, end, ImColor(seg_color), 2.0);
			}

			ImGui::Text("Intersection num: %lu", intersections.size());
			for (size_t i = 0; i < intersections.size(); i++)
			{
				ImVec2 start = mouse_pos;
				ImVec2 end = {(float)intersections[i].x, (float)intersections[i].y};
				draw_list->AddLine(start, end, ImColor(ray_color), 2.0);
				// ImGui::Text("(%f, %f)", end.x, end.y);
			}

			ImGui::End();
		}

		// Rendering
		ImGui::Render();
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
	}

	termImgui();
	return 0;
}
