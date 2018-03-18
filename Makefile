# Makefile for RayTracer

# *****************************************************
# Variables to control Makefile operation

CXX = g++
CXXFLAGS = -Wall -g -std=c++11

# ****************************************************
# Targets needed to bring the executable up to date

test: testTracer.o Lens.o Raytracer.o Image.o Quad.o Sphere.o Camera.o Vector3.o Plane.o Util.o Ray.o
	$(CXX) $(CXXFLAGS) -o test testTracer.o Lens.o Raytracer.o Image.o Quad.o Sphere.o Camera.o Vector3.o Plane.o Util.o Ray.o

# The main.o target can be written more simply

test.o: testTracer.cpp Camera.h Image.h Lens.h Plane.h Quad.h Raytracer.h Sphere.h Util.h Vector3.h
	$(CXX) $(CXXFLAGS) -c testTracer.cpp

main: main.o Lens.o Raytracer.o Image.o Quad.o Sphere.o Camera.o Vector3.o Plane.o Util.o Ray.o
	$(CXX) $(CXXFLAGS) -o main testTracer.o Lens.o Raytracer.o Image.o Quad.o Sphere.o Camera.o Vector3.o Plane.o Util.o Ray.o

# The main.o target can be written more simply

main.o: main.cpp Camera.h Image.h Lens.h Plane.h Quad.h Raytracer.h Sphere.h Util.h Vector3.h
	$(CXX) $(CXXFLAGS) -c main.cpp

Camera.o: Vector3.h
Image.o:  
Lens.o: Sphere.h Plane.h Ray.h Vector3.h Util.h Ray.h
Plane.o: Vector3.h
Quad.o: Vector3.h Image.h Ray.h
Ray.o: Vector3.h
Raytracer.o: Camera.h Image.h Lens.h Plane.h Quad.h Ray.h Sphere.h Util.h Vector3.h
Sphere.o: Util.h Ray.h Image.h Vector3.h
Util.o: Vector3.h
Vector3.o: Util.h

clean : 
	rm testTracer.o Lens.o Raytracer.o Image.o Quad.o Sphere.o Camera.o Vector3.o Plane.o Util.o Ray.o test main

