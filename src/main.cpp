#include <stdafx.hpp>
#include "imgui_helper.hpp"
#include "raycasting.hpp"

std::vector<Segment> segments =  {

	// Border
	{{0,0}, {1280,0}},
	{{1280,0}, {1280,720}},
	{{1280,720}, {0,720}},
	{{0,720}, {0,0}},

	// Polygon #1
	{{200,300}, {240,100}},
	{{240,100}, {400,160}},
	{{400,160}, {280,420}},
	{{280,420}, {200,300}},

	// Polygon #2
	{{200,400}, {240,500}},
	{{240,500}, {120,600}},
	{{120,600}, {200,400}},

	// Polygon #3
	{{400,520}, {440,300}},
	{{440,300}, {600,400}},
	{{600,400}, {700,640}},
	{{700,640}, {400,520}},

	// Polygon #4
	{{680,120}, {720,80}},
	{{720,80}, {740,140}},
	{{740,140}, {680,120}},

	// Polygon #5
	{{900,360}, {1020,340}},
	{{1020,340}, {1080,540}},
	{{1080,540}, {860,680}},
	{{860,680}, {900,360}},

	// Polygon #6
	{{800,190}, {1160,100}},
	{{1160,100}, {960,300}},
	{{960,300}, {800,190}}

};

std::vector<Circle> circles = {
	{{600, 200}, 100},
};

int main(int argc, char const *argv[]) {

	int w = 1280;
	int h = 720;
	std::string title = "Raycasting demo";
	initImgui(w, h, title);

	// Our state
	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(89.0f/255.0, 88.0f/255.0, 87.0f/255.0, 1.00f);
	ImVec4 seg_color = ImVec4(0.0f, 0.0f, 0.0f, 1.00f);
	ImVec4 ray_color = ImVec4(1.0f, 1.0f, 1.0f, 1.00f);

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

			static int num_ray = 50;
			static int len_ray = 100;
			static bool show_obstacle = true;
			ray.length = double(len_ray);

			for (double angle = 0; angle < M_PI * 2; angle += (M_PI * 2)/double(num_ray) ) {
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

				for (size_t i = 0; i < circles.size(); i++) {
					Vector2D intersection;
					if (!getIntersection(ray, circles[i], intersection)) {
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
			if (show_obstacle) {
				for (size_t i = 0; i < segments.size(); i++) {
					ImVec2 start = {(float)segments[i].begin.x, (float)segments[i].begin.y};
					ImVec2 end = {(float)segments[i].end.x, (float)segments[i].end.y};
					draw_list->AddLine(start, end, ImColor(seg_color), 2.0);
				}

				for (size_t i = 0; i < circles.size(); i++)
				{
					ImVec2 center = {(float)circles[i].center.x, (float)circles[i].center.y};
					draw_list->AddCircle(center, circles[i].radius, ImColor(seg_color), 0, 2.0);
				}
				
			}

			for (size_t i = 0; i < intersections.size(); i++) {
				ImVec2 start = mouse_pos;
				ImVec2 end = {(float)intersections[i].x, (float)intersections[i].y};
				draw_list->AddLine(start, end, ImColor(ray_color), 2.0);

				double dx = end.x - start.x;
				double dy = end.y - start.y;
				double d = dx*dx+dy*dy;
				if (d < ray.length*ray.length*0.99)
					draw_list->AddCircleFilled(end, 3.0, ImColor(ray_color));
				// ImGui::Text("(%f, %f)", end.x, end.y);
			}

			ImGui::PushItemWidth(400);
			ImGui::SliderInt("Num of rays", &num_ray, 1, 360);
			ImGui::SliderInt("Len of rays", &len_ray, 100, 2000);
			ImGui::PopItemWidth();
			ImGui::Checkbox("Show Obstacles", &show_obstacle);

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
