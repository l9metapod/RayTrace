
#include <iostream>
#include<fstream>
#include"hitable_list.h"
#include "sphere.h"
#include "float.h"
#include "camera.h"
#include "random.h"
#include<stdlib.h>
#include "material.h"

using namespace std;
double drand48();
vec3 random_in_unit_sphere();

float hit_sphere(const vec3& center,float radius, const ray& r) {
	vec3 oc = r.origin() - center;
	float a = dot(r.direction(), r.direction());
	float b = 2.0*dot(oc, r.direction());
	float c = dot(oc, oc) - radius * radius;
	float discriminant = b * b - 4 * a*c;
	if (discriminant < 0) {
		return -1.0;
	}
	else {
		return (-b - sqrt(discriminant)) / (2.0*a);//hit·µ»Ø¾àÀë
	}
}

vec3 color(const ray& r,hitable *world,int depth) {
	hit_record rec;
	if (world->hit(r, 0.001, FLT_MAX, rec)) {
		ray scattered;
		vec3 attenuation;
		if (depth < 10 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
			return attenuation * color(scattered, world, depth + 1);
		}
		else {
			return vec3(0, 0, 0);
		}
	}
	else {
		vec3 unit_direction = unit_vector(r.direction());
		float t = 0.5*(unit_direction.y() + 1.0);
		return (1.0 - t)*vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
	}	
}


int main() {
	cout << "ray traceing start" << endl;
	ofstream of("test.ppm");
	int nx = 300;
	int ny = 150;
	int ns = 300;
	of << "P3\n" << nx << " " << ny << "\n255\n";

	hitable *list[5];
	list[0] = new sphere(vec3(0, 0, -1), 0.5, new lambertian(vec3(0.1,0.2,0.5)));
	list[1] = new sphere(vec3(0, -100.5, -1), 100, new lambertian(vec3(0.8, 0.8,0.0)));
	list[2] = new sphere(vec3(1, 0, -1), 0.5, new metal(vec3(0.8, 0.6, 0.2),0.0));
	list[3] = new sphere(vec3(-1, 0, -1), 0.5, new dielectric(1.5));
	list[4] = new sphere(vec3(-1, 0, -1), -0.45, new dielectric(1.5));
	hitable *world = new hitable_list(list, 5);
	camera cam(vec3(-2,2,1),vec3(0,0,-1),vec3(0,1,0),45,float(nx)/float(ny));


	for (int j = ny - 1; j >= 0; j--) {
		for (int i = 0; i < nx; i++) {
			vec3 col(0, 0, 0);
			for (int s = 0; s < ns; s++) { 
				float u = float(i + drand48()) / float(nx);
				float v = float(j + drand48()) / float(ny);
				ray r = cam.get_ray(u,v);
				col += color(r, world,0);
			}
			
			col /= float(ns);
			col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
			int ir = int(255.99*col.r());
			int ig = int(255.99*col.g());
			int ib = int(255.99*col.b());
			of << ir << " " << ig << " " << ib << "\n";
		}
	}
	of.close();
	cout << "\nray traceing end\n";
	return 0;
}